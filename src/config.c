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
            "Your friend's mom: Hello?\n"
            "You realize the voice sounds like your friends mom.\n"
            "You: uhhhhh, Mrs. ######?\n"
            "Your friend's mom: Yes this is she. Wait ..., is this #####.\n"
            "You: Yea sorry I just got your number from ##### and was wondering if I could talk to him.\n"
            "Your friend's mom: Aren't you guys together right now? Why would you even call?\n"
            "You do not want her to find out about the blue boxes, jump to another line\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_HOME,SCENE_HOUSTON,SCENE_NEBRASKA_FARM}},

    // SCENE_NEBRASKA_FARM
    {
        .name = "Some random farm in Nebraska",
        .blurb =
            "A cow farm in Nebraska:\n"
            "You hear the sound of cows mooing\n"
            "Farmer: Do you need some milk?\n"
            "You: You do not sound like the Pope.\n"
            "Farmer: Why would I sound like the Pope\n"
            "You: Nevermind, What kind of milk do you have?\n"
            "After being distracted by milk, you decide to move on.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_YELLOWSTONE, SCENE_HOME, SCENE_MOM}},

    // SCENE_BEAN

    {
        .name = "Chicago, Illinois",
        .blurb = 
            "Near Cloud Gate (the chicago bean):\n"
            "Random Passerby: Look, I can see my reflection in the bean!\n"
            "It seems you called a telephone box in Chicago.\n"
            "You: This might take longer than I thought.\n"
            "Keep calling.\n",
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
            "Tour Guide: Thanks for calling Yellowstone National Park. How can I help you today?\n"
            "You: Would you happen to know the Vatican's number?\n"
            "Tour Guide: I think you have the wrong number.\n"
            "You notice you are continuing to travel west, maybe you will be able to cross the Pacific soon.\n",
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
            "Student: Well, looks like our season is over.\n"
            "It seems you intercepted someone trying to call their friend\n"
            "You: What season?\n"
            "Student: Don't act stupid, our entire senior football season is ruined! What happened to our defense from last season?\n"
            "You: I'm sorry, that really sucks. Maybe you will still make the playoffs if you win out!\n"
            "Student: Don't get my hopes up.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_BOSTON,SCENE_BEAN,SCENE_OHIO}},

     // SCENE_ALASKA
    {
        .name = "Alaska",
        .blurb =
            "It's a cold, dark day in Alaska:\n"
            "Random Guy: Why did you wake me up, it's the middle of the night!\n"
            "You: Well it's the middle of the day in Missouri!\n"
            "Random Guy: Oh... I have not seen the sun in weeks up here in Alaska.\n"
            "You: Sounds kind of like going to Notre Dame.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_JAPAN,SCENE_YELLOWSTONE,SCENE_KOREA}},
    
    // SCENE_HOUSTON
    {
        .name = "Houston, Texas",
        .blurb =
            "The NASA Mission Control Center in Houston is active, supporting a mission to the ISS:\n"
            "Astronaut: Houston, we have a problem.\n"
            "You: Not again!\n"
            "Mission Control Operator: WHO ELSE IS ON THIS LINE!\n"
            "You should probably get off this line so NASA does not get mad.\n",
            
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_MEXICO,SCENE_MOM,SCENE_YELLOWSTONE}},

    // SCENE_OHIO

    {
        .name = "Ohio",
        .blurb =
            "The phone rings at a house in the middle of Ohio:\n"
            "You: Where are you? I'm looking for the Vatican!\n"
            "Ohio Resident: Huh, this is Ohio. Isn't that in like Europe somewhere?\n"
            "You: Ohio? That must suck. Isn't that the worst state?\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA\n"
            "You: HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHA.\n"
            "You: thats so funny please laugh.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_NEW_YORK,SCENE_BEAN,SCENE_NOTRE_DAME}},
    
    // SCENE_BOSTON

    {
        .name = "Boston, Massachusetts",
        .blurb =
            "In Boston Harbor, people prepare to have the second Boston Tea Party:\n"
            "Tea Activist: The ship just landed, get over here!\n"
            "You: What ship? What is going on?\n"
            "Tea Activist: Today's the day, were going to throw all of Bigelow's tea off their cargo ship!\n"
            "You: Why? Bigelow's tea is pretty good.\n"
            "Tea Activist: So we don't have to pay taxes anymore!\n"
            "You: Last time we did that it caused a revolutionary war.\n"
            "Tea Activist: Whatever it takes!\n"
            "Confused, you continue to look for the Vatican.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_NOTRE_DAME,SCENE_NEW_YORK,SCENE_LONDON}},

    // SCENE_NEW_YORK

    {
        .name = "New York City",
        .blurb =
            "The skies pour on top of the Statue of Liberty, thunder booms in the distance:\n"
            "New Yorker: Ahh, the sound of freedom\n"
            "You: What's going on?\n"
            "New Yorker: Oh nothing, just admiring the beautiful Statue of Libery on this rainy day?\n"
            "You: Why are you out in the rain?\n"
            "New Yorker: I just love the Statue of Liberty!\n" 
            "You: Alright man, have fun with that.\n"
            "You certainly meet some interesting people while using the blue box.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_OHIO,SCENE_BOSTON,SCENE_PORTUGAL}},

    // SCENE_LONDON

    {
        .name = "London, UK",
        .blurb =
            "The bells of Big Ben ring out over the City, it is lunctime in the big city:\n"
            "Restaurant Employee: What kind of fish and chips do you want?\n"
            "You: Are you in the UK?\n"
            "Restaurant Employee: Of course, who else would only serve fish and chips?\n"
            "You: Is that all you guys have?\n"
            "Restaurant Employee: We also have pudding made from mainly pork blood if you want that?\n"
            "You: Thank goodness we did not lose,\n",
            
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_POLAND,SCENE_PARIS,SCENE_BOSTON}},
    
    // SCENE_PORTUGAL

    {
        .name = "Lisbon, Portugal",
        .blurb =
            "The sun sets over the scenic city of Lisbon, the capital of Portugal:\n"
            "You: This wouldn't happen to be the Vatican would it?\n"
            "Lisbon Resident: Vaticano? Você deveria estar ligando para Roma, não para Portugal.\n"
            "You: Portugal? Is that close to the Vatican? I hope it is not much further.\n"
            "Your friend may be close, hurry up!\n",

        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_SAHARA,SCENE_ROME,SCENE_NEW_YORK}},

    // SCENE_PARIS

    {
        .name = "Paris, France",
        .blurb =
            "The Eiffel Tower glistens above the Paris skyline:\n"
            "French Jesuit: Bonjour, vous souhaitez devenir jésuite?\n"
            "You: Jesuits? Like the order of Priests founded by St. Ignatius of Loyola in 1540?\n"
            "You: The same Jesuits who also contributed to Catholic education, intellectual thought, and ultramontane ideals?\n"
            "You: If thats true, then we could be in France where the order was founded.\n"
            "You: He also said bonjour, which I know is a French greeting so it must be France.\n"
            "You: Merci beaucoup (you learned this from a show and its the only french you know).\n"
            "This call has proven your great deductive skills.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_FLORIDA,SCENE_LONDON,SCENE_ROME}},


    // SCENE_POLAND

    {
        .name = "Poland",
        .blurb =
            "Students walk around The Pontifical University of John Paul II in Krakow, Poland:\n"
            "You: Hello, I am looking for the Pope.\n"
            "Professor: Szukasz papieża?\n"
            "Professor: Czy Ty również jesteś fanem JPII?\n"
            "You: JPII? yea was an awesome Pope! Finally someone who knows something\n"
            "Professor: Jeśli szukasz Papieża, zadzwoń do Rzymu.\n"
            "You: I do not know what that means but thank you.\n"
            "He knows about the Pope, maybe you are getting closer.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_LONDON,SCENE_RUSSIA,SCENE_ROME}},
    // SCENE_ROME

    {
        .name = "Rome, Italy",
        .blurb =
            "The ancient city of Rome remains stunning after 2777 years:\n"
            "You: Please let this be the Vatican.\n"
            "Italian: Il Vaticano è proprio dietro l'angolo, non puoi perderlo.\n"
            "You: You know something about the Vatican?\n"
            "You: WHY CAN'T I UNDERSTAND ANY OTHER LANGUAGES\n"
            "Italian: Basta camminare verso sinistra, 2,5 miglia dopo il Colosseo ed è sulla destran.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_VATICAN,SCENE_PORTUGAL,SCENE_POLAND}},
    
    // SCENE_SAHARA

    {
        .name = "The Sahara Desert",
        .blurb =
            "It is blisteringly hot on the sand dunes of the Sahara at midday:\n"
            "Sand Researcher: What's up, you got any more sand for me?\n"
            "You: Sand?\n"
            "Sand Researcher: Yea I have been walking the desert for weeks but I can't find any cool sand.\n"
            "You: What makes sand cool?\n"
            "Sand Researcher: You know, its just sand but cooler than normal sand.\n"
            "You: I think I am going insane.\n"
            "There are certainly interesting people to be found with the blue box.\n",
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
            "The sun shines over St. Peter's Basilica, you did it, you reached the Vatican:\n"
            "You: Helloooooooo, would you happen to be the Pope?\n"
            "Pope Leo: Who are you, how did you get my private line?\n"
            "You: No way, is it actually you Pope Leo.\n"
            "Pope Leo: Yes, I am Pope Leo. Who are you?\n"
            "You: YOU HAVE NO IDEA HOW LONG I HAVE BEEN SEARCHING FOR YOU\n"
            "You turn to your friend and shout, \"TAKE THAT I GOT HERE FIRST\"\n"
            "Pope Leo: This child clearly needs many prayers.\n"
            "You: Wait your from Chicago right? LETS GO CUBS!\n"
            "Pope Leo: You do realize I am a White Sox fan, right?\n"
            "You: oh\n"
            "Congratulations on completing the game, use any tone to take you back to the menu.\n",
            
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_VICTORY,SCENE_VICTORY,SCENE_VICTORY}},

    // SCENE_FLORIDA

    {
        .name = "Florida",
        .blurb =
            "The line reaches a small home in suburban Florida:\n"
            "Distant TV Noise: A Florida man orders at a fast-food drive-thru. Everything seems normal until he tosses a 3.5-foot live alligator through the window.\n"
            "Distant TV Noise: Florida Man tried to evade deputies on a John Deere riding lawn mower.\n"
            "Distant TV Noise: A 51-year-old ultra-marathon runner and Florida man, attempted to cross the Atlantic Ocean to London in a homemade “hamster wheel” vessel.\n"
            "You: Always weird stuff in Florida.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_PARIS,SCENE_SAHARA,SCENE_MEXICO}},

    // SCENE_RUSSIA

    {
        .name = "Russia",
        .blurb =
            "The Spires of St. Basil's Cathedral gleam in the heart of Moscow:\n"
            "Russian Hockey Player: Вы готовитесь к хоккейной тренировке?\n"
            "You: I heard the word hockey in there?\n"
            "Russian Hockey Player: кто ты?\n"
            "You: I think that language sounds like Russian.\n"
            "You: Russia, Hockey... Hey remember the Miracle on Ice buddy?\n"
            "You: Great moments are born from great opportunity. And that's what you have here, tonight, boys.\n" 
            "That's what you've earned here tonight. One game. If we played 'em ten times, they might win nine.\n" 
            "But not this game. Not tonight. Tonight, we skate with them. Tonight, we stay with them. And we shut\n" 
            "them down because we can! Tonight, we are the greatest hockey team in the world.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_KOREA,SCENE_POLAND,SCENE_SIBERIA}},

    // SCENE_JAPAN

    {
        .name = "Japan",
        .blurb =
            "The vibrant city of Tokyo sparkles in the night, bustling with activity:\n"
            "Japanese College Student: 今夜出かけたいですか？\n"
            "You: Sorry I do not understand.\n"
            "Japanese College Student: 今夜パーティーに行く予定じゃなかったの？\n"
            "You: I still have no idea what you are saying.\n",

        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_ALASKA,SCENE_KOREA,SCENE_CHINA}},

    // SCENE_MEXICO

    {
        .name = "Mexico",
        .blurb =
            "Pilgrims from all over the world gather at the Basilica of Our Lady of Guadalupe to see the Tilma:\n"
            "Office Attendant: Hola, esta es la Basílica de Guadalupe.\n"
            "Congregation: Dios te salve, María, Llena eres de gracia, el Señor es contigo.\n"
            "Congregation: Bendita tú eres entre todas las mujeres, y bendito es el fruto de tu vientre, Jesús.\n" 
            "Congregation: Santa María, Madre de Dios, ruega por nosotros, pecadores, ahora y en la hora de nuestra muerte\n"
            "Congregation: Amen.\n"
            "You: Oh I've been there before, what a beautiful church. Not very close to the Vatican though.\n"
            "You may have found a Catholic place, but you wonder how long will it take to get to the Vatican from Mexico.\n",

        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_HOUSTON,SCENE_PERU,SCENE_FLORIDA}},

    // SCENE_PERU

    {
        .name = "Peru",
        .blurb =
            "The old city of Machu Picchu rests on the Andes mountains:\n"
            "Guide: Hola, ¿estás buscando visitar Machu Picchu?\n"
            "You: Machu Picchu? Isn't that in Peru? That's no where near the Vatican.\n"
            "Guide: ¿Tienes alguna pregunta sobre Machu Picchu?\n"
            "You: Lo siento, no comprendo?\n"
            "You realize it may be helpful to learn some new languages if you are going to use the blue box more often.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_RIO,SCENE_MEXICO,SCENE_EASTER_ISLAND}},
 

    // SCENE_RIO

    {
        .name = "Rio de Janeiro, Brazil",
        .blurb =
            "Sunlight gleams off of the imposing figure of Christ the Redeemer, staring over Rio de Janeiro:\n"
            "Tour Guide: Você chegou ao posto de turismo do Cristo Redentor, quer comprar ingresso? \n"
            "You: Hello, is this the Vatican?\n"
            "Tour Guide: O Vaticano?, você terá que atravessar o oceano para isso?\n"
            "You heard something about an ocean, maybe you can cross it from wherever you are.\n",
            
           
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_PERU,SCENE_KENYA,SCENE_SAHARA}},

    
    // SCENE_EASTER_ISLAND

    {
        .name = "Rapa Nui (Easter Island)",
        .blurb =
            "The great stone Moai cover the island, still shrouded in mystery:\n"
            "You: Where are you?\n"
            "Tourist: I am on Easter Island, who is this?\n"
            "You: Easter Island... Easter, maybe there is some connection to the Vatican here?\n"
            "Tourist: You're trying to connect to the Vatican? That's on the other side of the world!\n"
            "Surely there is no way to do that quickly from here.\n"
            "You: That's what I'm thinking, guess I will just have to keep calling.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_LEVEL_BLUEBOX,SCENE_AUSTRALIA,SCENE_PERU}},

    // SCENE_AUSTRALIA

    {
        .name = "Australia",
        .blurb =
            "The phone is answered by a young man in Austrailia:\n"
            "You: I am looking for the Vatican, is that around you?\n"
            "Random Australian Guy Who Likes Spiders: I do not know about that, but I do know about some cool spiders!\n"
            "This piques your interest, as you have a pet tarantula.\n"
            "You: Really? Do you have any as pets? I have a Rose Hair tarantula named Chunky?\n"
            "Random Australian Guy Who Likes Spiders: Woah is that a new world species? That is, a species from the Americas\n"
            "that uses urticating hairs on their abdomen for defense, unlike old world tarantulas from the rest of the world\n"
            "that rely more heavily on venom?\n"
            "You: EXACTLY, I did not expect to find a fellow tarantula fan on here!\n"
            "Random Australian Guy Who Likes Spiders: I actually have a Queensland Whistling tarantula, an old world species native to Australia.\n"
            "You: That's awesome!\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_CHINA,SCENE_EASTER_ISLAND,SCENE_INDIA}},

    // SCENE_KENYA

    {
        .name = "Kenya",
        .blurb =
            "The line is picked up by a man in Kenya:\n"
            "You: Hello, who is this?\n"
            "Eliud Kipchoge: This is Eliud Kipchoge.\n"
            "You: The Eliud Kipchoge? I watched you break 2 hours in the marathon in Austria a few years ago, it was incredible!\n"
            "You: I can't believe you ran 26 miles at 4 minutes and 34 seconds per mile pace. I've never even did that for one mile in my career.\n"
            "You: I hope you win the New York City Marathon this year! Then you will be the first person to win the Six star medal!\n"
            "Eliud Kipchoge: I have no idea who you are, but I appreciate the compliments.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_EGYPT,SCENE_RIO,SCENE_MADAGASCAR}},

    // SCENE_INDIA

    {
        .name = "India",
        .blurb =
            "A masterpiece of Mughal archicture, the beautiful Taj Mahal glimmers in the sunlight:\n"
            "Tour Guide: क्या आप ताजमहल का भ्रमण करना चाहते हैं?\n"
            "You: I am looking for the Vatican, is that around you somewhere?\n"
            "Tour Guide: मैं आपको बता सकता हूँ कि वेटिकन भारत में नहीं है।.\n"
            "You: I don't know if I will ever make it.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_MADAGASCAR,SCENE_CHINA,SCENE_AUSTRALIA}},

    // SCENE_MADAGASCAR

    {
        .name = "Madagascar",
        .blurb =
            "An old man picks up the phone from Madagascar:\n"
            "Madagascar Resident: ka henoko hoe mitady vatican ianao?\n"
            "Madagascar Resident: Raha mitondra an'io tsipika io mankany Ejipta ianao dia mety ho akaiky kokoa.\n"
            "You: You know something about the Vatican?\n"
            "Maybe there is a clue here somewhere, if only you understood Malagasy.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_INDIA,SCENE_EGYPT,SCENE_KENYA}},

    // SCENE_EGYPT

    {
        .name = "Egypt",
        .blurb =
            "Tourists wander around the Great Pyramids of Giza on hot day:\n"
            "Travel Consultant: Hello! Are you looking to travel to Giza to see the Pyramids?\n"
            "You: No sorry I am actually trying to go to the Vatican.\n"
            "Travel Consultant: Oh, you will have to call someone else for that.\n"
            "Travel Consultant: Do you want me to connect you to someone else?\n"
            "You: No thanks, I can't afford to wait.\n",

        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_KENYA,SCENE_MADAGASCAR,SCENE_LEVEL_BLUEBOX}},

    // SCENE_CHINA

    {
        .name = "China",
        .blurb =
            "The crowd roars at Shougang Park in Beijing, China. The China Smash Finals are in full swing:\n"
            "Announcer: Ma Long's signature forehand blows by Lin Shidong to take the third game of the finals 11-5! What a match!\n"
            "It seems you tapped into a line at the China Smash, a premier international table tennis event.\n"
            "You: No way! I was hoping Hugo Calderano would make the finals this year.\n" 
            "As much as you love table tennis and want to keep listening to the match, your friend may be getting closer to the Vatican.\n"
            "Table Tennis Fun Fact: Ma long loses this event in real life as he was upset by the 19-year-old Lin Shidong at China Smash 2024.\n"
            "This was one of the last international events Ma Long, the consensus greatest of all time, would play in before retiring.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_AUSTRALIA,SCENE_INDIA,SCENE_JAPAN}},

    // SCENE_KOREA

    {
        .name = "Seoul, South Korea",
        .blurb =
            "The City of Seoul is bustling on a beautiful Thursday afternoon. People are beginning to leave work for the day:\n"
            "IT Department: 또 전화야? 곧 떠나고 싶어.\n"
            "You: Sorry I do not understand.\n"
            "IT Department: 왜 미국인이 전화선에 서있나요?\n"
            "IT Department: 장난전화가 아니길 바라. 집에 가야 해.\n"
            "You: Any Idea where the Vatican is?\n"
            "The attendant hangs up the phone, annoyed, but happy they can finally get off work.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_RUSSIA,SCENE_JAPAN,SCENE_ALASKA}},

     // SCENE_SIBERIA

    {
        .name = "Siberia",
        .blurb =
            "The sun shines over the cold, windswept plains of West Siberia:\n"
            "You: Hello?\n"
            "Mysterious Siberian Guy: What brings you here traveler? Nobody ever calls anymore.\n"
            "You: Where are you?\n"
            "Mysterious Siberian Guy: Somewhere in the plains.\n"
            "You: I'm looking for the Vatican.\n"
            "Mysterious Siberian Guy: Ah, I think the Polish might know something about that.\n"
            "Mysterious Siberian Guy: Sorry to say this line only goes to back to the parts of Russia where people actually live\.n"
            "You: Well, thanks for the tip. Goodluck out there!\n"
            "Weird guy, but maybe getting to Poland will get you close to the Vatican.\n",
        .target_hz = 440.0f,
        .tolerance_hz = 15.0f,
        .hold_ms = 1500,
        .time_limit_ms = 0,
        .next_scene_id = {SCENE_RUSSIA,SCENE_RUSSIA,SCENE_RUSSIA}},


    // SCENE_LEVEL_BLUEBOX
    {
        .name = "Vatican Trunk",
        .blurb =
            "A dusty switchroom:\n"
            "A priest whispers: The old trunk line still listens...\n",
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
