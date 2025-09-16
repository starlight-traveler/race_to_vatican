#include "config.h"
#include <time.h>

// table for different expected tones
const float tone_table[TONE_COUNT] = {100.0f, 200.0f, 300.0f};

const Level g_levels[SCENE_COUNT] = {
    // SCENE_MENU (dummy slot)
    {
        .name = "MENU",
        .blurb = "",
        .target_hz = 0.0f,
        .tolerance_hz = 0.0f,
        .hold_ms = 0,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_MENU,0,0}},

    // start scene: home
    {
        .name = "Home: St. Louis, Missouri",
        .blurb =
            "It's a warm, windy summer day in St. Louis, Missouri.\n"
            "You and a friend just got blue boxes from the phone phreaking hacker across the street.\n"
            "You were told you could call anywhere in the world if you could figure out the right tones.\n"
            "Your friend asks,\"Wouldn't it be funny if we somehow called the Pope?\"\n"
            "\"Thats not a bad idea, wait ..., lets race to see who can get there first!\"\n"
            "The Race to the Vatican Begins!\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_LEVEL_PRACTICE_440, 0, SCENE_LEVEL_BLUEBOX}},

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
        .next_scene_id = {SCENE_VICTORY,0,0}},

    // SCENE_LEVEL_PRACTICE_440
    {
        .name = "Practice: Concert A",
        .blurb =
            "A tuning fork rests on the desk.\n"
            "Match its pitch to prove your ear.",
        .target_hz = 100.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_LEVEL_PENTAGON,0,SCENE_LEVEL_BLUEBOX}},

    // Pentagon scene
    {
        .name = "The Pentagon",
        .blurb =
            "You hear workers bustling around.\n"
            "\"Who is on this line! State your ID and Clearance\"\n"
            "You have five seconds to leave before the government traces your call.",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_LEVEL_PRACTICE_440, 0, SCENE_LEVEL_BLUEBOX}},

    // SCENE_VICTORY (dummy level slot)
    {
        .name = "CONNECTED",
        .blurb = "",
        .target_hz = 0.0f,
        .tolerance_hz = 0.0f,
        .hold_ms = 0,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_MENU,0,0}}};

uint64_t cfg_now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)(ts.tv_nsec / 1000000ULL);
}
