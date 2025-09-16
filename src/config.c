#include "config.h"
#include <time.h>

// table for different expected tones
const float tone_table[TONE_COUNT] = {1400.0f, 1600.0f, 1800.0f};

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
        .next_scene_id = {SCENE_HOME,SCENE_HOUSTON,SCENE_NEBRASKA_FARM}},

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
        .next_scene_id = {SCENE_NOTRE_DAME, SCENE_OHIO, SCENE_HOME}},

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
        .next_scene_id = {SCENE_NEBRASKA_FARM,SCENE_ALASKA,SCENE_HOUSTON}},
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
        .next_scene_id = {SCENE_BOSTON,SCENE_BEAN,SCENE_OHIO}},

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
        .next_scene_id = {SCENE_JAPAN,SCENE_YELLOWSTONE,SCENE_RUSSIA}},
    
    // SCENE_HOUSTON
    {
        .name = "Houston",
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
        .next_scene_id = {SCENE_MEXICO,SCENE_MOM,SCENE_YELLOWSTONE}},

    // SCENE_OHIO

    {
        .name = "Ohio",
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
        .next_scene_id = {SCENE_NEW_YORK,SCENE_BEAN,SCENE_NOTRE_DAME}},
    
    // SCENE_BOSTON

    {
        .name = "Boston, Massachusetts",
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
        .next_scene_id = {SCENE_NOTRE_DAME,SCENE_NEW_YORK,SCENE_LONDON}},

    // SCENE_NEW_YORK

    {
        .name = "The Statue of Libery: New York City",
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
        .next_scene_id = {SCENE_OHIO,SCENE_BOSTON,SCENE_PORTUGAL}},

    // SCENE_LONDON

    {
        .name = "London",
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
        .next_scene_id = {SCENE_POLAND,SCENE_PARIS,SCENE_BOSTON}},
    
    // SCENE_PORTUGAL

    {
        .name = "Lisbon, Portugal",
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
        .next_scene_id = {SCENE_SAHARA,SCENE_ROME,SCENE_NEW_YORK}},

    // SCENE_PARIS

    {
        .name = "Paris",
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
        .next_scene_id = {SCENE_FLORIDA,SCENE_LONDON,SCENE_VATICAN}},


    // SCENE_POLAND

    {
        .name = "Poland",
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
        .next_scene_id = {SCENE_LONDON,SCENE_RUSSIA,SCENE_ROME}},
    // SCENE_ROME

    {
        .name = "Rome",
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
        .next_scene_id = {SCENE_VATICAN,SCENE_PORTUGAL,SCENE_POLAND}},
    
    // SCENE_SAHARA

    {
        .name = "The Sahara Desert",
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
        .next_scene_id = {SCENE_PORTUGAL,SCENE_FLORIDA,SCENE_RIO}},

    // SCENE_VATICAN

    {
        .name = "The Vatican",
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
        .next_scene_id = {SCENE_ROME,SCENE_LEVEL_BLUEBOX,SCENE_PARIS}},

    // SCENE_FLORIDA

    {
        .name = "Florida",
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
        .next_scene_id = {SCENE_PARIS,SCENE_SAHARA,SCENE_MEXICO}},

    // SCENE_RUSSIA

    {
        .name = "Russia",
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
        .next_scene_id = {SCENE_ALASKA,SCENE_CHINA,SCENE_}},

    // SCENE_JAPAN

    {
        .name = "JAPAN",
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
        .next_scene_id = {SCENE_CHINA,SCENE_POLAND,SCENE_ALASKA}},

    // SCENE_MEXICO

    {
        .name = "Mexico",
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
        .next_scene_id = {SCENE_HOUSTON,SCENE_PERU,SCENE_FLORIDA}},

    // SCENE_PERU

    {
        .name = "Peru",
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
        .next_scene_id = {SCENE_RIO,SCENE_MEXICO,SCENE_EASTER_ISLAND},


    // SCENE_RIO

    {
        .name = "Rio",
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
        .next_scene_id = {SCENE_PERU,SCENE_SOUTH_AFRICA,SCENE_SAHARA}},

    
    // SCENE_EASTER_ISLAND

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_LEVEL_BLUEBOX,SCENE_AUSTRALIA,SCENE_PERU}},

    // SCENE_AUSTRALIA

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_CHINA,SCENE_EASTER_ISLAND,SCENE_INDIA}},

    // SCENE_SOUTH_AFRICA

    {
        .name = "South Africa",
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
        .next_scene_id = {SCENE_EGYPT,SCENE_RIO,SCENE_MADAGASCAR}},

    // SCENE_INDIA

    {
        .name = "India",
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
        .next_scene_id = {SCENE_MADAGASCAR,SCENE_CHINA,SCENE_AUSTRALIA}},

    // SCENE_MADAGASCAR

    {
        .name = "Madagascar",
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
        .next_scene_id = {SCENE_INDIA,SCENE_EGYPT,SCENE_SOUTH_AFRICA}},

    // SCENE_EGYPT

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_SOUTH_AFRICA,SCENE_MADAGASCAR,SCENE_LEVEL_BLUEBOX}},

    // SCENE_CHINA

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_AUSTRALIA,SCENE_INDIA,SCENE_JAPAN}},
    
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
        .next_scene_id = {SCENE_HOME,SCENE_HOUSTON,SCENE_NEBRASKA_FARM}},

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
        .next_scene_id = {SCENE_NOTRE_DAME, SCENE_OHIO, SCENE_HOME}},

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
        .next_scene_id = {SCENE_NEBRASKA_FARM,SCENE_ALASKA,SCENE_HOUSTON}},
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
        .next_scene_id = {SCENE_BOSTON,SCENE_BEAN,SCENE_OHIO}},

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
        .next_scene_id = {SCENE_JAPAN,SCENE_YELLOWSTONE,SCENE_KOREA}},
    
    // SCENE_HOUSTON
    {
        .name = "Houston",
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
        .next_scene_id = {SCENE_MEXICO,SCENE_MOM,SCENE_YELLOWSTONE}},

    // SCENE_OHIO

    {
        .name = "Ohio",
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
        .next_scene_id = {SCENE_NEW_YORK,SCENE_BEAN,SCENE_NOTRE_DAME}},
    
    // SCENE_BOSTON

    {
        .name = "Boston, Massachusetts",
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
        .next_scene_id = {SCENE_NOTRE_DAME,SCENE_NEW_YORK,SCENE_LONDON}},

    // SCENE_NEW_YORK

    {
        .name = "The Statue of Libery: New York City",
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
        .next_scene_id = {SCENE_OHIO,SCENE_BOSTON,SCENE_PORTUGAL}},

    // SCENE_LONDON

    {
        .name = "London",
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
        .next_scene_id = {SCENE_POLAND,SCENE_PARIS,SCENE_BOSTON}},
    
    // SCENE_PORTUGAL

    {
        .name = "Lisbon, Portugal",
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
        .next_scene_id = {SCENE_SAHARA,SCENE_ROME,SCENE_NEW_YORK}},

    // SCENE_PARIS

    {
        .name = "Paris",
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
        .next_scene_id = {SCENE_FLORIDA,SCENE_LONDON,SCENE_ROME}},


    // SCENE_POLAND

    {
        .name = "Poland",
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
        .next_scene_id = {SCENE_LONDON,SCENE_RUSSIA,SCENE_ROME}},
    // SCENE_ROME

    {
        .name = "Rome",
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
        .next_scene_id = {SCENE_VATICAN,SCENE_PORTUGAL,SCENE_POLAND}},
    
    // SCENE_SAHARA

    {
        .name = "The Sahara Desert",
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
        .next_scene_id = {SCENE_PORTUGAL,SCENE_FLORIDA,SCENE_RIO}},

    // SCENE_VATICAN

    {
        .name = "The Vatican",
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
        .next_scene_id = {SCENE_ROME,SCENE_LEVEL_BLUEBOX,SCENE_PARIS}},

    // SCENE_FLORIDA

    {
        .name = "Florida",
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
        .next_scene_id = {SCENE_PARIS,SCENE_SAHARA,SCENE_MEXICO}},

    // SCENE_RUSSIA

    {
        .name = "Russia",
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
        .next_scene_id = {SCENE_KOREA,SCENE_POLAND,SCENE_SIBERIA}},

    // SCENE_JAPAN

    {
        .name = "JAPAN",
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
        .next_scene_id = {SCENE_ALASKA,SCENE_KOREA,SCENE_CHINA}},

    // SCENE_MEXICO

    {
        .name = "Mexico",
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
        .next_scene_id = {SCENE_HOUSTON,SCENE_PERU,SCENE_FLORIDA}},

    // SCENE_PERU

    {
        .name = "Peru",
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
        .next_scene_id = {SCENE_RIO,SCENE_MEXICO,SCENE_EASTER_ISLAND},


    // SCENE_RIO

    {
        .name = "Rio",
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
        .next_scene_id = {SCENE_PERU,SCENE_SOUTH_AFRICA,SCENE_SAHARA}},

    
    // SCENE_EASTER_ISLAND

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_LEVEL_BLUEBOX,SCENE_AUSTRALIA,SCENE_PERU}},

    // SCENE_AUSTRALIA

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_CHINA,SCENE_EASTER_ISLAND,SCENE_INDIA}},

    // SCENE_SOUTH_AFRICA

    {
        .name = "South Africa",
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
        .next_scene_id = {SCENE_EGYPT,SCENE_RIO,SCENE_MADAGASCAR}},

    // SCENE_INDIA

    {
        .name = "India",
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
        .next_scene_id = {SCENE_MADAGASCAR,SCENE_CHINA,SCENE_AUSTRALIA}},

    // SCENE_MADAGASCAR

    {
        .name = "Madagascar",
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
        .next_scene_id = {SCENE_INDIA,SCENE_EGYPT,SCENE_SOUTH_AFRICA}},

    // SCENE_EGYPT

    {
        .name = "Egypt",
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
        .next_scene_id = {SCENE_SOUTH_AFRICA,SCENE_MADAGASCAR,SCENE_LEVEL_BLUEBOX}},

    // SCENE_CHINA

    {
        .name = "China",
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
        .next_scene_id = {SCENE_AUSTRALIA,SCENE_INDIA,SCENE_JAPAN}},

    // SCENE_KOREA

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_RUSSIA,SCENE_JAPAN,SCENE_ALASKA}},

     // SCENE_SIBERIA

    {
        .name = "Easter Island",
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
        .next_scene_id = {SCENE_RUSSIA,SCENE_RUSSIA,SCENE_RUSSIA}},


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
        .next_scene_id = {SCENE_EASTER_ISLAND,SCENE_VATICAN,SCENE_EGYPT}},

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
        .next_scene_id = {SCENE_EASTER_ISLAND,SCENE_VATICAN,SCENE_EGYPT}},

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
