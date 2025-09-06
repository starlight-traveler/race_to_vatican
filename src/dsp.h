#ifndef DSP_H
#define DSP_H

#include <stddef.h>

typedef struct
{
    float freq_hz;   // <0 if invalid
    float peak_norm; // normalized autocorr peak
} PitchResult;

// Estimate pitch via autocorrelation; WINDOW_SIZE max
PitchResult dsp_estimate_pitch_autocorr(const float *x, size_t n, float sr);

// Utility: clamp; absf
float dsp_absf(float v);
float dsp_clampf(float v, float lo, float hi);

#endif
