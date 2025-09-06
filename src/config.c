#include "config.h"
#include <time.h>

const Level g_levels[SCENE_COUNT] = {
    // SCENE_MENU (dummy slot)
    {
        .name = "MENU",
        .blurb = "",
        .target_hz = 0.0f,
        .tolerance_hz = 0.0f,
        .hold_ms = 0,
        .time_limit_ms = 0,
        .next_scene_id = SCENE_MENU},

    // SCENE_LEVEL_BLUEBOX
    {
        .name = "Blue Box: Vatican Trunk",
        .blurb =
            "A dusty switchroom. A priest whispers: \"The old trunk line still listens...\"\n"
            "Produce the control tone to seize the line.\n"
            "Hint: ~2600 Hz. Hold it steady to connect.",
        .target_hz = 2600.0f,
        .tolerance_hz = 60.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = SCENE_VICTORY},

    // SCENE_LEVEL_PRACTICE_440
    {
        .name = "Practice: Concert A",
        .blurb =
            "A tuning fork rests on the desk.\n"
            "Match its pitch to prove your ear.",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1200,
        .time_limit_ms = 0,
        .next_scene_id = SCENE_LEVEL_BLUEBOX},

    // SCENE_VICTORY (dummy level slot)
    {
        .name = "CONNECTED",
        .blurb = "",
        .target_hz = 0.0f,
        .tolerance_hz = 0.0f,
        .hold_ms = 0,
        .time_limit_ms = 0,
        .next_scene_id = SCENE_MENU}};

uint64_t cfg_now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)(ts.tv_nsec / 1000000ULL);
}
