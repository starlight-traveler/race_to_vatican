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

    // SCENE_HOME
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
        .next_scene_id = {SCENE_MOM, SCENE_NEBRASKA_FARM, SCENE_BEAN}},

    // SCENE_MOM
    {
        .name = "Your Friend's Mom: Somewhere in St. Louis",
        .blurb =
            "A suburban home, nestled in somewhere in St.Louis County:\n"
            "\"Hello?\"\n"
            "You realize the voice sounds like Mrs. ######\n"
            "\"uhhhhh, Mrs. ######?\"\n"
            "\"Yes this is she. Wait ..., is this #####\"\n"
            "\"Yea sorry I just got your number from ##### and was wondering if I could talk to him\"\n"
            "\"Aren't you guys together right now? Why would you even call?\"\n"
            "You do not want her to find out about the blue boxes, jump to another line",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_HOME,SCENE_BEAN,SCENE_NEBRASKA_FARM}},

    // SCENE_NEBRASKA_FARM
    {
        .name = "Some random farm in Nebraska",
        .blurb =
            "A cow farm in Nebraska:"
            "You hear the sound of cows mooing\n"
            "Farmer: Do you need some milk?\n"
            "You: You do not sound like the Pope\n"
            "Farmer: Why would I sound like the Pope\n"
            "You: Nevermind, What kind of milk do you have?\n"
            "After being distracted by milk, you decide to move on\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_YELLOWSTONE, SCENE_HOME, SCENE_MOM}},

    // SCENE_CHICAGO

    {
        .name = "Chicago City",
        .blurb = 
            "Near Cloud Gate (the chicago bean)\n"
            "Random Passerby: Look, I can see my reflection in the bean!\n"
            "It seems you called a telephone box in Chicago\n"
            "You: This might take longer than I thought\n"
            "Keep calling\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_NOTRE_DAME, SCENE_MOM, SCENE_HOME}},

    // SCENE_YELLOWSTONE
    {
        .name = "Yellowstone National Park",
        .blurb =
            "Steam rises over hot springs, geysers erupt, the landscape is covered by a lush forest:\n"
            "Tour Guide: Thanks for calling Yellowstone National Park. How can I help you today?"
            "You: Would you happen to know the Vatican's number?\n"
            "Tour Guide: I think you have the wrong number\n"
            "You notice you are continuing to travel west, maybe you will be able to cross the Pacific soon\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_HOME,0,SCENE_NEBRASKA_FARM}},
    // SCENE_NOTRE_DAME
    {
        .name = "Notre Dame Stadium",
        .blurb =
            "Disappointed students leave the stands, it seems they just lost in heartbreaking game in the fourth quarter\n"
            "Student: Well, looks like our season is over?\n"
            "It seems you intercepted someone trying to call their friend\n"
            "You: What season?\n"
            "Student: Don't act stupid, our entire senior football season is ruined! What happend to our defense from last season?\n"
            "You: Chill out man, its just football\n"
            "The student hangs up on the line\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {0,SCENE_BEAN,0}},

     // SCENE_ALASKA
    {
        .name = "ALASKA",
        .blurb =
            "\n"
            "Student: Well, looks like our season is over?\n"
            "It seems you intercepted someone trying to call their friend\n"
            "You: What season?\n"
            "Student: Don't act stupid, our entire senior football season is ruined! What happend to our defense from last season?\n"
            "You: Chill out man, its just football\n"
            "The student hangs up on the line\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {0,SCENE_BEAN,0}},

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
