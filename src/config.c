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
            "Disappointed students leave the stands, it seems they just lost in heartbreaking game in the fourth quarter:\n"
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
            "It's a cold, dark day in Alaska:\n"
            "Random Guy: Why did you wake me up, its the middle of the night!\n"
            "You: Well its the middle of the day in Missouri!\n"
            "Random Guy: Oh... I have not seen the sun in weeks up here in Alaska\n"
            "You: Sounds kind of like going to Notre Dame\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_JAPAN,SCENE_YELLOWSTONE,SCENE_KOREA}},
    
    // SCENE_HOUSTON
    {
        .name = "Houston",
        .blurb =
            "The NASA Mission Control Center in Houston is active, supporting a mission to the ISS\n"
            "Astronaut: Houston, we have a problem\n"
            "You: Not again!\n"
            "Mission Control Operator: WHO ELSE IS ON THIS LINE\n"
            "You should probably get off this line so NASA does not get mad\n",
            
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_MEXICO,SCENE_MOM,SCENE_YELLOWSTONE}},

    // SCENE_OHIO

    {
        .name = "Ohio",
        .blurb =
            "The phone rings at a house in the middle of Ohio"
            "You: Where are you? I'm looking for the Vatican!\n"
            "Ohio Resident: Huh, this is Ohio. Isn't that in like Europe somewhere?"
            "You: Ohio? That must suck. Isn't that the worst state?\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: thats so funny please laugh\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_NEW_YORK,SCENE_BEAN,SCENE_NOTRE_DAME}},
    
    // SCENE_BOSTON

    {
        .name = "Boston, Massachusetts",
        .blurb =
            "In Boston Harbor, people prepare to have the second Boston Tea Party\n"
            "Tea Activist: The ship just landed, get over here?\n"
            "You: What ship? What is going on?\n"
            "Tea Activist: Today's the day, were going to throw all of Bigelow's tea off their cargo ship\n"
            "You: Why? Bigelow tea is pretty good\n"
            "Tea Activist: So we don't have to pay taxes anymore!"
            "You: Last time we did that it caused a revolutionary war\n"
            "Tea Activist: Whatever it takes\n"
            "Confused, you continue to look for the Vatican\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_NOTRE_DAME,SCENE_NEW_YORK,SCENE_LONDON}},

    // SCENE_NEW_YORK

    {
        .name = "New York City",
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
            "Student walk around The Pontifical University of John Paul II in Krakow, Poland\n"
            "You: Hello, I am looking for the Pope\n"
            "Professor: Szukasz papieża?\n"
            "Professor: Czy Ty również jesteś fanem JPII\n?"
            "You: JPII? yea was an awesome Pope! Finally someone who knows something\n"
            "Professor: Jeśli szukasz Papieża, zadzwoń do Rzymu\n"
            "You: I do not know what that means but thank you\n"
            "He knows about the Pope, maybe you are getting closer\n",
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
        .name = "Japan",
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
            "Pilgrims from all over the world gather at the Basilica of Our Lady of Guadalupe to see the Tilma\n"
            "Office Attendant: Hola, esta es la Basílica de Guadalupe.\n"
            "Congregation: Dios te salve, María, Llena eres de gracia, el Señor es contigo.\n"
            "Congregation: Bendita tú eres entre todas las mujeres, y bendito es el fruto de tu vientre, Jesús.\n" 
            "Congregation: Santa María, Madre de Dios, ruega por nosotros, pecadores, ahora y en la hora de nuestra muerte\n"
            "Congregation: Amen\n"
            "You: Oh I've been there before, what a beautiful church. Not very close to the Vatican though\n"
            "You may have found a Catholic place, but you wonder how long will it take to get to the Vatican from Mexico?\n",

        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_HOUSTON,SCENE_PERU,SCENE_FLORIDA}},

    // SCENE_PERU

    {
        .name = "Peru",
        .blurb =
            "The old city of Machu Picchu rests on the Andes mountains\n"
            "Guide: Hola, ¿estás buscando visitar Machu Picchu?\n"
            "You: Machu Picchu? isn't that in Peru? That's no where near the Vatican\n"
            "Guide: ¿Tienes alguna pregunta sobre Machu Picchu?\n"
            "You: Lo siento, no comprendo?\n"
            "You realize it may be helpful to learn so new languages if you are going to use the blue box more often\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_RIO,SCENE_MEXICO,SCENE_EASTER_ISLAND}},
 

    // SCENE_RIO

    {
        .name = "Rio",
        .blurb =
            "Sunlight gleams off of the imposing figure of Christ the Redeemer, staring over Rio de Janeiro\n"
            "Tour Guide: Você chegou ao posto de turismo do Cristo Redentor, quer comprar ingresso? \n"
            "You: Hello, is this the Vatican?\n"
            "Tour Guide: O Vaticano?, você terá que atravessar o oceano para isso?\n"
            "You heard something about an ocean, maybe you can cross it from wherever you are\n",
            
           
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
        .name = "Australia",
        .blurb =
            "Random Australian Guy: "
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
            "An old man picks up the phone from Madagascar\n"
            "Madagascar Resident: ka henoko hoe mitady vatican ianao?\n"
            "Madagascar Resident: Raha mitondra an'io tsipika io mankany Ejipta ianao dia mety ho akaiky kokoa\n"
            "You: You know something about the Vatican?\n"
            "Maybe there is a clue here somewhere, if only you understood Malagasy\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_INDIA,SCENE_EGYPT,SCENE_SOUTH_AFRICA}},

    // SCENE_EGYPT

    {
        .name = "Egypt",
        .blurb =
            "Tourists wander around the Great Pyramids of Giza on hot day\n"
            "Travel Consultant: Hello! Are you looking to travel to Giza to see the Pyramids?\n"
            "You: No sorry I am actually trying to go to the Vatican\n"
            "Travel Consultant: Oh, you will have to call someone else for that\n"
            "Travel Consultant: Do you want me to connect you to someone else?\n"
            "You: No thanks, I can't afford to wait\n",

        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_SOUTH_AFRICA,SCENE_MADAGASCAR,SCENE_LEVEL_BLUEBOX}},

    // SCENE_CHINA

    {
        .name = "China",
        .blurb =
            "The crowd roars at Shougang Park in Beijing, China. The China Smash Finals are in full swing\n"
            "Announcer: Ma Long's signature forehand blows by Lin Shidong to take the third game of the finals 11-5! What a match!?\n"
            "Table Tennis Fun Fact: Ma long loses this event in real life as he was upset by the 19-year-old Lin Shidong at China Smash 2024)"
            "It seems you tapped into a line at the China Smash, a premier international table tennis event\n"
            "You: No way! I was hoping Hugo Calderano would make the finals this year.\n" 
            "As much as you love table tennis and want to keep listening to the match, your friend may be getting\n "
            "closer to the Vatican\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_AUSTRALIA,SCENE_INDIA,SCENE_JAPAN}},

    // SCENE_KOREA

    {
        .name = "South Korea",
        .blurb =
            "The City of Seoul is bustling on a beautiful Thursday afternoon, People are beginning to leave work for the day\n"
            "IT Department: 또 전화야? 곧 떠나고 싶어\n"
            "You: Sorry I do not understand\n"
            "IT Department: 왜 미국인이 전화선에 서있나요?\n"
            "IT Department: 장난전화가 아니길 바라. 집에 가야 해.\n"
            "You: Any Idea where the Vatican is?\n"
            "The attendant hangs up the phone, annoyed, but happy they can finally get off work\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_RUSSIA,SCENE_JAPAN,SCENE_ALASKA}},

     // SCENE_SIBERIA

    {
        .name = "Siberia",
        .blurb =
            "The sun shines over the cold, windswept plains of West Siberia\n"
            "You: Hello?\n"
            "Mysterious Siberian Guy: What brings you here traveller? Nobody every calls anymore.\n"
            "You: Where are you?\n"
            "Mysterious Siberian Guy: Somewhere in the plains.\n"
            "You: I'm looking for the Vatican\n"
            "Mysterious Siberian Guy: Ah, I think the Poles might know something about that.\n"
            "Mysterious Siberian Guy: Sorry to say this line only goes to back to the parts of Russia where people actually live\n"
            "You: Well, thanks for the tip. Goodluck out there\n"
            "Weird guy, but maybe getting to Poland will get you close to the Vatican\n",
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
