#include "dsp.h"
#include "config.h"
#include <math.h>
#include <string.h>

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

PitchResult dsp_estimate_pitch_autocorr(const float *x_in, size_t n, float sr)
{
    PitchResult pr = {.freq_hz = -1.0f, .peak_norm = 0.0f};
    if (!x_in || n == 0)
        return pr;

    static float x[WINDOW_SIZE];
    if (n > WINDOW_SIZE)
        n = WINDOW_SIZE;
    memcpy(x, x_in, n * sizeof(float));

    // DC removal
    float mu = compute_mean(x, n);
    for (size_t i = 0; i < n; ++i)
        x[i] -= mu;

    // Silence check
    float energy = compute_energy(x, n);
    if (energy < 1e-6f)
        return pr;

    int minLag = (int)floorf(sr / FREQ_MAX_HZ);
    int maxLag = (int)ceilf(sr / FREQ_MIN_HZ);
    if (minLag < 2)
        minLag = 2;
    if (maxLag > (int)(n / 2))
        maxLag = (int)(n / 2);

    // r0
    double r0 = 0.0;
    for (size_t i = 0; i < n; ++i)
        r0 += (double)x[i] * (double)x[i];
    if (r0 <= 0.0)
        return pr;

    int bestLag = -1;
    double bestVal = 0.0;

    for (int lag = minLag; lag <= maxLag; ++lag)
    {
        double r = 0.0;
        size_t lim = n - (size_t)lag;
        for (size_t i = 0; i < lim; ++i)
        {
            r += (double)x[i] * (double)x[i + (size_t)lag];
        }
        double norm = r / r0;
        if (norm > bestVal)
        {
            bestVal = norm;
            bestLag = lag;
        }
    }

    if (bestLag <= 0 || bestVal < PEAK_NORM_THRESH)
    {
        pr.peak_norm = (float)bestVal;
        return pr;
    }

    // Parabolic refine
    int lag = bestLag;
    int lagm1 = lag - 1 < minLag ? lag : lag - 1;
    int lagp1 = lag + 1 > maxLag ? lag : lag + 1;

    double rm1 = 0.0, r0c = 0.0, rp1 = 0.0;
    for (size_t i = 0; i + (size_t)lagm1 < n; ++i)
        rm1 += (double)x[i] * (double)x[i + (size_t)lagm1];
    for (size_t i = 0; i + (size_t)lag < n; ++i)
        r0c += (double)x[i] * (double)x[i + (size_t)lag];
    for (size_t i = 0; i + (size_t)lagp1 < n; ++i)
        rp1 += (double)x[i] * (double)x[i + (size_t)lagp1];
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
