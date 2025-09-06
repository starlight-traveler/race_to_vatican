#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include <stddef.h>
#include <portaudio.h>
#include <pthread.h>
#include "config.h"

typedef struct
{
    float ring[WINDOW_SIZE];
    size_t write_idx;
    bool filled_once;
    pthread_mutex_t mtx;
} AudioRing;

typedef struct
{
    PaStream *stream;
    AudioRing ring;
    bool initialized;
} AudioSystem;

// Initialize PortAudio and open default input
bool audio_init(AudioSystem *as);

// Start stream
bool audio_start(AudioSystem *as);

// Stop/term
void audio_shutdown(AudioSystem *as);

// Copy latest N samples into dst; returns true if ready
bool audio_copy_latest_window(AudioSystem *as, float *dst, size_t n);

#endif
