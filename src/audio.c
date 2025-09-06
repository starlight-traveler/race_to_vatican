#include "audio.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>

static int audio_callback(const void *input, void *output,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo *ti,
                          PaStreamCallbackFlags flags,
                          void *userData)
{
    (void)output;
    (void)ti;
    (void)flags;
    AudioSystem *as = (AudioSystem *)userData;
    const float *in = (const float *)input;
    if (!in || !as)
        return paContinue;

    AudioRing *r = &as->ring;
    pthread_mutex_lock(&r->mtx);
    for (unsigned long i = 0; i < frameCount; ++i)
    {
        r->ring[r->write_idx] = in[i];
        r->write_idx = (r->write_idx + 1) % WINDOW_SIZE;
        if (r->write_idx == 0)
            r->filled_once = true;
    }
    pthread_mutex_unlock(&r->mtx);
    return paContinue;
}

bool audio_init(AudioSystem *as)
{
    if (!as)
        return false;
    memset(as, 0, sizeof(*as));
    as->ring.write_idx = 0;
    as->ring.filled_once = false;
    pthread_mutex_init(&as->ring.mtx, NULL);

    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        fprintf(stderr, "PortAudio init error: %s\n", Pa_GetErrorText(err));
        return false;
    }

    PaStreamParameters inputParams;
    inputParams.device = Pa_GetDefaultInputDevice();
    if (inputParams.device == paNoDevice)
    {
        fprintf(stderr, "No default input device.\n");
        Pa_Terminate();
        return false;
    }
    const PaDeviceInfo *dinfo = Pa_GetDeviceInfo(inputParams.device);
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = dinfo->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    PaError e2 = Pa_OpenStream(&as->stream,
                               &inputParams, NULL,
                               SAMPLE_RATE,
                               FRAMES_PER_BUFFER,
                               paNoFlag,
                               audio_callback,
                               as);
    if (e2 != paNoError)
    {
        fprintf(stderr, "Pa_OpenStream error: %s\n", Pa_GetErrorText(e2));
        Pa_Terminate();
        return false;
    }
    as->initialized = true;
    return true;
}

bool audio_start(AudioSystem *as)
{
    if (!as || !as->initialized)
        return false;
    PaError err = Pa_StartStream(as->stream);
    if (err != paNoError)
    {
        fprintf(stderr, "Pa_StartStream error: %s\n", Pa_GetErrorText(err));
        return false;
    }
    return true;
}

void audio_shutdown(AudioSystem *as)
{
    if (!as)
        return;
    if (as->stream)
    {
        Pa_StopStream(as->stream);
        Pa_CloseStream(as->stream);
        as->stream = NULL;
    }
    Pa_Terminate();
    pthread_mutex_destroy(&as->ring.mtx);
    as->initialized = false;
}

bool audio_copy_latest_window(AudioSystem *as, float *dst, size_t n)
{
    if (!as || !dst || n > WINDOW_SIZE)
        return false;
    AudioRing *r = &as->ring;

    pthread_mutex_lock(&r->mtx);
    bool ready = r->filled_once;
    size_t end = r->write_idx;
    size_t start = (end + WINDOW_SIZE - n) % WINDOW_SIZE;
    if (ready)
    {
        if (start + n <= WINDOW_SIZE)
        {
            memcpy(dst, &r->ring[start], n * sizeof(float));
        }
        else
        {
            size_t first = WINDOW_SIZE - start;
            memcpy(dst, &r->ring[start], first * sizeof(float));
            memcpy(dst + first, &r->ring[0], (n - first) * sizeof(float));
        }
    }
    pthread_mutex_unlock(&r->mtx);
    return ready;
}
