#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// -------- Global Config --------
#define SAMPLE_RATE        48000
#define FRAMES_PER_BUFFER  1024
#define WINDOW_SIZE        4096           // >= FRAMES_PER_BUFFER
#define HOP_INTERVAL_MS    50             // UI update cadence

// Pitch detection frequency band
#define FREQ_MIN_HZ        70.0f
#define FREQ_MAX_HZ        3000.0f        // extended for blue-box tones

// Autocorr acceptance
#define PEAK_NORM_THRESH   0.30f

// Movement frequency bins (for the free-move scenes)
#define BIN_LEFT_MAX       250.0f
#define BIN_UP_MAX         500.0f
#define BIN_RIGHT_MAX      800.0f

// Tone Count and Tolerance

#define TONE_COUNT         3
#define TONE_TOLERANCE     50.0f


typedef struct {
    const char *name;
    const char *blurb;           // story text shown in scene
    float target_hz;             // target frequency
    float tolerance_hz;          // +/- tolerance
    uint32_t hold_ms;            // time to hold target continuously
    uint32_t time_limit_ms;      // 0 for no limit
    int next_scene_id;           // scene index after success
} Level;

// Scene IDs for global routing
enum
{
    SCENE_MENU = 0,
    SCENE_LEVEL_BLUEBOX,
    SCENE_LEVEL_PRACTICE_440,
    SCENE_LEVEL_PENTAGON,
    SCENE_VICTORY,
    SCENE_DEFEAT,
    SCENE_COUNT
};

// Level table (indexed by scene IDs that are level scenes)
extern const Level g_levels[];

extern const float tone_table[];

// Utility
uint64_t cfg_now_ms(void);

#endif
