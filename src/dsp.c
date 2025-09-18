#include "dsp.h"
#include "config.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct
{
    float b0, b1, b2;
    float a1, a2;
    float z1, z2;
} Biquad;

/*------------------------------------------------------------------------
    Tunables for voice rejection & band selection
  ------------------------------------------------------------------------*/
#define TELEMETRY_BAND_LO_HZ 300.0f
#define TELEMETRY_BAND_HI_HZ 2400.0f

/* Pre/post guard to make the transition a bit steeper than the hard band */
#define HPF_EDGE_HZ 150.0f /* high-pass edge a bit below band */
#define LPF_EDGE_HZ 2500.0f /* low-pass edge a bit above band  */

/* Butterworth-like Q for 2nd-order HP/LP sections (~-3 dB at cutoff) */
#define BIQUAD_Q_BUTTER 0.70710678f

/* In-band dominance requirement (fraction of total frame energy in band) */
#define INBAND_ENERGY_MIN_FRAC 0.15f

/* Quality thresholds for autocorrelation peak */
#ifndef PEAK_NORM_THRESH
#define PEAK_NORM_THRESH 0.30f /* minimum normalized peak */
#endif
#define PEAK_MARGIN_OVER_MEAN 2.0f /* best peak must exceed mean by 2x */

/* If not provided by config.h/dsp.h, define pitch search limits here */
#ifndef FREQ_MIN_HZ
#define FREQ_MIN_HZ TELEMETRY_BAND_LO_HZ
#endif
#ifndef FREQ_MAX_HZ
#define FREQ_MAX_HZ TELEMETRY_BAND_HI_HZ
#endif

/*------------------------------------------------------------------------
    Basic DSP helpers
  ------------------------------------------------------------------------*/
static float compute_mean(const float *x, size_t n)
{
    double s = 0.0;
    for (size_t i = 0; i < n; ++i)
        s += x[i];
    return (float)(s / (double)n);
}
static float compute_energy(const float *x, size_t n)
{
    double e = 0.0;
    for (size_t i = 0; i < n; ++i)
        e += (double)x[i] * (double)x[i];
    return (float)e;
}

float dsp_absf(float v) { return v < 0.0f ? -v : v; }
float dsp_clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

/*------------------------------------------------------------------------
    Biquad design (RBJ cookbook) and processing
  ------------------------------------------------------------------------*/
static void biquad_init_lowpass(Biquad *bq, float sr, float fc, float q)
{
    float w0 = 2.0f * (float)M_PI * (fc / sr);
    float sinw0 = sinf(w0), cosw0 = cosf(w0);
    float alpha = sinw0 / (2.0f * q);

    float b0 = (1.0f - cosw0) * 0.5f;
    float b1 = 1.0f - cosw0;
    float b2 = (1.0f - cosw0) * 0.5f;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cosw0;
    float a2 = 1.0f - alpha;

    bq->b0 = b0 / a0;
    bq->b1 = b1 / a0;
    bq->b2 = b2 / a0;
    bq->a1 = a1 / a0;
    bq->a2 = a2 / a0;
    bq->z1 = bq->z2 = 0.0f;
}

static void biquad_init_highpass(Biquad *bq, float sr, float fc, float q)
{
    float w0 = 2.0f * (float)M_PI * (fc / sr);
    float sinw0 = sinf(w0), cosw0 = cosf(w0);
    float alpha = sinw0 / (2.0f * q);

    float b0 = (1.0f + cosw0) * 0.5f;
    float b1 = -(1.0f + cosw0);
    float b2 = (1.0f + cosw0) * 0.5f;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cosw0;
    float a2 = 1.0f - alpha;

    bq->b0 = b0 / a0;
    bq->b1 = b1 / a0;
    bq->b2 = b2 / a0;
    bq->a1 = a1 / a0;
    bq->a2 = a2 / a0;
    bq->z1 = bq->z2 = 0.0f;
}

static void biquad_process_inplace(Biquad *bq, float *data, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        float in = data[i];
        float out = (bq->b0 * in) + bq->z1;
        bq->z1 = (bq->b1 * in) + bq->z2 - (bq->a1 * out);
        bq->z2 = (bq->b2 * in) - (bq->a2 * out);
        data[i] = out;
    }
}

/*------------------------------------------------------------------------
    Main estimator with stronger voice rejection
  ------------------------------------------------------------------------*/
PitchResult dsp_estimate_pitch_autocorr(const float *x_in, size_t n, float sr)
{
    PitchResult pr = {.freq_hz = -1.0f, .peak_norm = 0.0f};
    if (!x_in || n == 0)
        return pr;

    /* Work buffer (clamped to compile-time window size if provided) */
#ifndef WINDOW_SIZE
#define WINDOW_SIZE 4096
#endif
    static float x[WINDOW_SIZE];
    if (n > WINDOW_SIZE)
        n = WINDOW_SIZE;
    memcpy(x, x_in, n * sizeof(float));

    /* Remove DC on the raw frame (helps energy math be meaningful) */
    {
        float mu = compute_mean(x, n);
        for (size_t i = 0; i < n; ++i)
            x[i] -= mu;
    }

    /* Early silence check (on raw, DC-removed) */
    float energy_total = compute_energy(x, n);
    if (energy_total < 1e-9f)
        return pr;

    /* Persistent 4th-order bandpass (HP -> LP cascade) */
    static float last_sr = 0.0f;
    static Biquad hpf1, lpf1, hpf2, lpf2; /* two cascades for extra rejection */

    if (sr != last_sr && sr > 0.0f)
    {
        /* Two identical HP and LP stages for steeper skirts */
        biquad_init_highpass(&hpf1, sr, HPF_EDGE_HZ, BIQUAD_Q_BUTTER);
        biquad_init_lowpass(&lpf1, sr, LPF_EDGE_HZ, BIQUAD_Q_BUTTER);
        biquad_init_highpass(&hpf2, sr, HPF_EDGE_HZ, BIQUAD_Q_BUTTER);
        biquad_init_lowpass(&lpf2, sr, LPF_EDGE_HZ, BIQUAD_Q_BUTTER);
        last_sr = sr;
    }

    /* Copy a second buffer for in-band filtering (to measure its energy) */
    static float x_band[WINDOW_SIZE];
    memcpy(x_band, x, n * sizeof(float));

    /* Apply 4th-order bandpass via HP->LP (twice) */
    biquad_process_inplace(&hpf1, x_band, n);
    biquad_process_inplace(&lpf1, x_band, n);
    biquad_process_inplace(&hpf2, x_band, n);
    biquad_process_inplace(&lpf2, x_band, n);

    /* In-band dominance gate: reject frames not dominated by 1.1–1.8 kHz */
    float energy_inband = compute_energy(x_band, n);
    float frac_inband = (energy_total > 0.0f) ? (energy_inband / energy_total) : 0.0f;
    if (frac_inband < INBAND_ENERGY_MIN_FRAC)
    {
        /* Frame is likely speech or noise spread outside the telemetry band */
        pr.freq_hz = -1.0f;
        pr.peak_norm = 0.0f;
        return pr;
    }

    /* From this point, operate on the *in-band* signal only */
    float *xw = x_band;

    /* Define lag search range from allowed frequency band */
    int minLag = (int)floorf(sr / FREQ_MAX_HZ);
    int maxLag = (int)ceilf(sr / FREQ_MIN_HZ);
    if (minLag < 2)
        minLag = 2;
    if (maxLag > (int)(n / 2))
        maxLag = (int)(n / 2);

    /* r0 for normalization */
    double r0 = 0.0;
    for (size_t i = 0; i < n; ++i)
        r0 += (double)xw[i] * (double)xw[i];
    if (r0 <= 0.0)
        return pr;

    /* Scan autocorrelation; also accumulate a simple baseline/mean */
    int bestLag = -1;
    double bestVal = 0.0;
    double accMean = 0.0;
    int count = 0;

    for (int lag = minLag; lag <= maxLag; ++lag)
    {
        double r = 0.0;
        size_t lim = n - (size_t)lag;
        for (size_t i = 0; i < lim; ++i)
            r += (double)xw[i] * (double)xw[i + (size_t)lag];

        double norm = r / r0;
        accMean += norm;
        ++count;

        if (norm > bestVal)
        {
            bestVal = norm;
            bestLag = lag;
        }
    }

    /* Baseline quality check: best peak must exceed mean sufficiently */
    double meanVal = (count > 0) ? (accMean / (double)count) : 0.0;
    if (bestLag <= 0 || bestVal < PEAK_NORM_THRESH || bestVal < (PEAK_MARGIN_OVER_MEAN * meanVal))
    {
        pr.peak_norm = (float)bestVal;
        pr.freq_hz = -1.0f;
        return pr;
    }

    /* Parabolic refinement around bestLag (guarding edges) */
    int lag = bestLag;
    int lagm1 = (lag - 1 < minLag) ? lag : (lag - 1);
    int lagp1 = (lag + 1 > maxLag) ? lag : (lag + 1);

    double rm1 = 0.0, r0c = 0.0, rp1 = 0.0;
    for (size_t i = 0; i + (size_t)lagm1 < n; ++i)
        rm1 += (double)xw[i] * (double)xw[i + (size_t)lagm1];
    for (size_t i = 0; i + (size_t)lag < n; ++i)
        r0c += (double)xw[i] * (double)xw[i + (size_t)lag];
    for (size_t i = 0; i + (size_t)lagp1 < n; ++i)
        rp1 += (double)xw[i] * (double)xw[i + (size_t)lagp1];

    rm1 /= r0;
    r0c /= r0;
    rp1 /= r0;

    double denom = (2.0 * r0c) - rm1 - rp1;
    double delta = 0.0;
    if (fabs(denom) > 1e-12)
    {
        delta = 0.5 * (rm1 - rp1) / denom;
        if (delta < -1.0)
            delta = -1.0;
        if (delta > 1.0)
            delta = 1.0;
    }

    double refinedLag = (double)lag + delta;
    pr.freq_hz = (float)(sr / refinedLag);
    pr.peak_norm = (float)r0c;
    return pr;
}
