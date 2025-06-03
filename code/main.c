#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // necessary for mmemov
#include "Enums.h"
#include "Plants.h"
#include "Structs.h"
#include "main.h"
#include "Textures.h"
#include "Sound.h"
#include "Buttons.h"
#include "Homepage.h"
//================================================================================================================================================
//CONSTANTS=======================================================================================================================================
//================================================================================================================================================

//Constants of lawn display
const int DISTANCE_LAWN_RIGHT_EDGE = (0.025)*SCREEN_WIDTH;
const int DISTANCE_LAWN_LEFT_EDGE = DISTANCE_LAWN_RIGHT_EDGE;
const int DISTANCE_LAWN_INFERIOR_EDGE = DISTANCE_LAWN_RIGHT_EDGE;
const int DISTANCE_LAWN_SUPERIOR_EDGE = DISTANCE_LAWN_RIGHT_EDGE*2;
const int LAWN_WIDTH_VALUE = (SCREEN_WIDTH-DISTANCE_LAWN_RIGHT_EDGE-DISTANCE_LAWN_LEFT_EDGE)/NUMBER_COLUMN_LAWN-12;
const int LAWN_HEIGHT_VALUE = (SCREEN_HEIGHT-DISTANCE_LAWN_INFERIOR_EDGE-DISTANCE_LAWN_SUPERIOR_EDGE)/NUMBER_ROWS_LAWN-8;
const int LAWN_X_VALUE = LAWN_WIDTH_VALUE;
const int LAWN_Y_VALUE = LAWN_HEIGHT_VALUE;

//Constants of deck display
const int DECK_RECTANGLE_X_VALUE = 10;
const int DECK_RECTANGLE_Y_VALUE = 10;
const int DECK_ELEMENT_WIDTH_VALUE =LAWN_WIDTH_VALUE;
const int DECK_ELEMENT_HEIGHT_VALUE = 70;

//User's Mouse
Vector2 mousePoint = { 0.0f, 0.0f }; 

//ScreenRectangle(used to display textures)
const Rectangle SCREEN_RECTANGLE={
    .height=SCREEN_HEIGHT,
    .width=SCREEN_WIDTH,
    .x=0,
    .y=0
};



int main (void){
    //INITIALIZATION
//----------------------------------------------------------------------------------------------------------------------------------------------


//--screen
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,"raylib [core] example - basic screen manager");
    GAME_SCREEN currentScreen = LOGO;
    GAME_SCREEN previousScreen = HOMEPAGE;
//--------

//--audio
    InitAudioDevice();
    LoadAllSounds();
    


//--textures
LoadAllTextures();

InitGameStructs();

InitButtons();

//--HomePage
InitHomePage();
//---------
//Configurations Menu

    Rectangle configurationsOptionsRec[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool configurationsOptionsRecHover[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered
    bool mutedSounds[CONFIGURATIONS_OPTIONS_QUANTITY]={0}; 
    // bool configurationsOptionsSoundHover[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//array that tells if an option outputed its sound
    
    CONFIGURATIONS_OPTIONS configurationsOptions[CONFIGURATIONS_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
    //Filling the configurationsOptions
    //backgroundMusic ->mute/unmute
    //zombies ->mute/unmute
    //plants ->mute/unmute
    //peashots ->mute/unmute
    //sun ->mute/unmute
    //soundEffects ->mute/unmute
    
    configurationsOptions[CONFIGURATIONS_SOUND_BACKGROUND]= CONFIGURATIONS_SOUND_BACKGROUND;
    configurationsOptions[CONFIGURATIONS_BTN_BACKGROUND]= CONFIGURATIONS_BTN_BACKGROUND;
    configurationsOptions[CONFIGURATIONS_SOUND_ZOMBIES]= CONFIGURATIONS_SOUND_ZOMBIES;
    configurationsOptions[CONFIGURATIONS_BTN_ZOMBIES]= CONFIGURATIONS_BTN_ZOMBIES;
    configurationsOptions[CONFIGURATIONS_SOUND_PLANTS]= CONFIGURATIONS_SOUND_PLANTS;
    configurationsOptions[CONFIGURATIONS_BTN_PLANTS]= CONFIGURATIONS_BTN_PLANTS;
    configurationsOptions[CONFIGURATIONS_SOUND_PEASHOTS]= CONFIGURATIONS_SOUND_PEASHOTS;
    configurationsOptions[CONFIGURATIONS_BTN_PEASHOTS]= CONFIGURATIONS_BTN_PEASHOTS;
    configurationsOptions[CONFIGURATIONS_SOUND_SUNS]= CONFIGURATIONS_SOUND_SUNS;
    configurationsOptions[CONFIGURATIONS_BTN_SUNS]= CONFIGURATIONS_BTN_SUNS;
    configurationsOptions[CONFIGURATIONS_SOUND_SOUNDEFFECTS]= CONFIGURATIONS_SOUND_SOUNDEFFECTS;
    configurationsOptions[CONFIGURATIONS_BTN_SOUNDEFFECTS]= CONFIGURATIONS_BTN_SOUNDEFFECTS;
    configurationsOptions[CONFIGURATIONS_BTN_GOBACK]= CONFIGURATIONS_BTN_GOBACK;

    //Filling the configurationsOptionsRec
    BTN_DYNAMIC_Y=BTN_INITIAL_Y;
        //If index is odd, option goes to the right, if index is even, option to the left
        for (int i=0;i<CONFIGURATIONS_OPTIONS_QUANTITY;i++){
            //if its even
            if(!(i&1)){
                configurationsOptionsRec[i].height = BTN_HEIGHT;
                configurationsOptionsRec[i].width = BTN_WIDTH;
                configurationsOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS;
                configurationsOptionsRec[i].y = BTN_DYNAMIC_Y;
            }else{
                //if its odd
                configurationsOptionsRec[i].height = BTN_HEIGHT;
                configurationsOptionsRec[i].width = BTN_WIDTH;
                configurationsOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
                configurationsOptionsRec[i].y =BTN_DYNAMIC_Y;
                BTN_DYNAMIC_Y+=BTN_HEIGHT;
                // (((SCREEN_HEIGHT-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY))*i
            }
        }
        configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK]=BTN_GOBACK;
        configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK].x=0;


//--Gaming Menu
    Rectangle gamingMenuOptionsRec[GAMING_MENU_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool gamingMenuOptionsRecHover[GAMING_MENU_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    GAME_SCREEN gamingMenuOptions[GAMING_MENU_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
        //Filling the homePageOptions
        gamingMenuOptions[0]= RESUME;
        gamingMenuOptions[1]= CONFIGURATIONS;
        gamingMenuOptions[2]= EXIT;

        //Filling the gamingMenuOptionsRec 
        for (int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
            gamingMenuOptionsRec[i].height = BTN_HEIGHT;
            gamingMenuOptionsRec[i].width = BTN_WIDTH;

gamingMenuOptionsRec[i].x=BTN_ALONE_CENTERED_X;

gamingMenuOptionsRec[i].y= marginFromTitle+((SCREEN_HEIGHT-marginFromTitle)/GAMING_MENU_OPTIONS_QUANTITY)*i;
        }

//---------------------------



//--exiting application
    //--exit confirmation
        Rectangle GOBACK_BTN_CPY=BTN_GOBACK;
            GOBACK_BTN_CPY.x = BTN_CENTERED_X_FOR_TWO_BUTTONS;
            GOBACK_BTN_CPY.y =(SCREEN_HEIGHT-BTN_HEIGHT*2)/2;
            bool GOBACK_BTN_CPY_HOVER =0;

        Rectangle EXIT_BTN_CPY=BTN_GOBACK;
            EXIT_BTN_CPY.x = BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
            EXIT_BTN_CPY.y =(SCREEN_HEIGHT-BTN_HEIGHT*2)/2;
            bool EXIT_BTN_CPY_HOVER =0;
    SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works
    bool exitWindow = false;    // Flag to set window to exit
    
//---------

//--frameManagement
    int framesCounter = 0;          // Useful to count frames
    SetTargetFPS(TARGET_FPS);               // Set desired framerate (frames-per-second)
//--------------------------------


//--PLAYER 

    char playerName[MAX_SIZE_OF_NAME]={0};

    unsigned int sizeOfName = 0;//variable used to track the size of the name of the user

//-----------------


//DECK-----
    Plant DeckOfPlants [SIZE_OF_DECK]={0};
    DeckOfPlants[0] = PLANT_SUNFLOWER;
    DeckOfPlants[1] = PLANT_GREEN_PEASHOOTER;
    DeckOfPlants[2] = PLANT_WALLNUT;
    DeckOfPlants[3] = SHOVEL_REMOVE_PLANTS;

    //used to track which card is selected. If card is all nulled, then there's no card selected
    Plant cardSelected = {0};

//---------------------------------------------

//PLANT ---------------------
    //used to track which plants are deployed in the field(lawn)
    Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]={0};
    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
            plantArr[i][j].type=TYPE_NULL_PLANT;
        }
    }
//----------------------



//LAWN--------------
//lawns of the game
    //used to verify if a plant already exists in a spot
    bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]={0};
    bool lawnRectanglesHover[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
    Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
        for(int i=0;i<NUMBER_ROWS_LAWN;i++){
            for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                lawnRectangles[i][j].x = 30+LAWN_X_VALUE*j;
                lawnRectangles[i][j].y = 80+LAWN_Y_VALUE*i;
                lawnRectangles[i][j].width = LAWN_WIDTH_VALUE;
                lawnRectangles[i][j].height = LAWN_HEIGHT_VALUE;
            }
        }
//-------------------------------------

//SUN----------------------

    double timeOfLastSun = 0;
    //array to track the suns, if the x and y coordinates are "-1", then we consider it an empty sun
    Rectangle sunArray[SIZE_OF_SUN_ARR]={0};
    //array to track the quantity of sun that the player has
    unsigned int sunGamingStorage=800;
    //array to track when a sun hits the ground
    float groundOfTheSuns[SIZE_OF_SUN_ARR]={0};
    //indexToTrack the end of the array
    int indexOfNextSun = 0;
    //time of spawn of suns = 15s
    double spawnRateSun = 8.0;   
    //used to spawn sun appropriately
    double timeSpawnSunTracking =0;
   


//------------------------

//PEASHOOTER AND PEASHOT
    PeaShot peaShotsArr [SIZE_OF_PEASHOT_ARR];
    int indexOfNextPea = 0;
//---------------------

//ZOMBIE-----------------


    Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]={0};
    //initializing zombie array not to conflict with shooting peas mechanic
        for(int i=0;i<SIZE_OF_ZOMBIES_ARR;i++){
            zombieArr[i].rowOfZombie=-1;
        }
    int indexOfNextZombie=0;
    double spawnRateZombie = 5.0;
    bool firstZombieSpawn =1;
    double timeOfLastZombie = 0;  //saves the actualTime
    double timeForFirstSpawnZombie=30.0;
    //used to spawn zombies appropriately
    double timeSpawnZombieTracking =0;


//------------------------



// ====================================================================================================================================================================================================
//MAIN LOOP GAME====================================================================================================================================================================================
// ====================================================================================================================================================================================================
    


    while (!exitWindow)    // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();//tracking, effectively, the user's mouse
        
        //if user wants to end applications, redirect him to an exit confirmation
        if(WindowShouldClose()){
            currentScreen=  EXIT;
        }

        //Switch that UPDATES THE VARIABLES
        switch (currentScreen)
        {


            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                
                framesCounter++;    // Count frames

                // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
                if (framesCounter > 240)
                {
                    currentScreen = HOMEPAGE;
                }
            } break;


            case HOMEPAGE:
            {

                // TODO: Update HOMEPAGE screen variables here!

                //playing intromusic once and in loops of its lasting size
                if(!IsSoundPlaying(SOUND_HOMEPAGE_MENU)){
                    PlaySound(SOUND_HOMEPAGE_MENU);
                }


                 for (int i = 0; i < HOME_PAGE_OPTIONS_QUANTITY; i++)
        {
                    if (CheckCollisionPointRec(mousePoint, homePageOptionsRec[i])) {
                        if(!homePageOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        homePageOptionsRecHover[i] = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            currentScreen = homePageOptions[i];
                        }
                    }
                    else {homePageOptionsRecHover[i] = 0;}
        }
            } break;


            case PLAY:
            {
                // TODO: Update PLAY screen variables here!
                currentScreen = USER_DATA;//first thing to be done, is to ask for the User data          
            } break;



              case USER_DATA:
            {
                // TODO: Update USERDATA screen variables here!
                previousScreen=currentScreen;

                // Get char pressed (unicode character) on the queue
                int key = GetCharPressed();
                
                // Check if more characters have been pressed on the same frame
                while (key > 0) {
                    // NOTE: Only allow keys in range [32..125]
                    //MAX_SIZE_OF_NAME-1 to garantee the space for '\0'
                    if ((key >= 32) && (key <= 125) && (sizeOfName < MAX_SIZE_OF_NAME-1)) {
                        playerName[sizeOfName] = (char)key;
                        sizeOfName++;
                    }
                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (sizeOfName == 0){
                        sizeOfName = 0;
                    }else{
                        sizeOfName--;
                    }
                    playerName[sizeOfName] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && sizeOfName > 0) {
                    currentScreen = GAMEPLAY; 
                    timeOfLastZombie=GetTime();
                    timeOfLastSun=GetTime();
                
                }
            } break;
            
            bool menuWasACTIONED=0;
            float timeSpentAtMenu=0;

            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                
                previousScreen=currentScreen;
                if(!IsSoundPlaying(SOUND_GAMEPLAY)){
                    
                    PlaySound(SOUND_GAMEPLAY);
                }
                //if zombie has gone out of the screen
                if(updatePlantsAndZombiesGameplay(plantArr,peaShotsArr,zombieArr,occupationOfLawn,&indexOfNextPea,&indexOfNextZombie,SOUND_PEASHOT_IMPACT,SOUND_ZOMBIE_EAT_PLANT)){
                    PlaySound(SOUND_LOST_MUSIC);
                    StopSound(SOUND_GAMEPLAY);
                    currentScreen=END_GAME;

                }


                updateSunsPosition(sunArray,indexOfNextSun,groundOfTheSuns);

                for(int i=0;i<NUMBER_ROWS_LAWN;i++){
                    for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                        if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                            lawnRectanglesHover[i][j] = 1;
                        }   
                        else lawnRectanglesHover[i][j] = 0;
                    }
                }
                if(!menuWasACTIONED){
                //used to spawn sun appropriately
                 timeSpawnSunTracking =GetTime();

                //used to spawn zombies appropriately
                 timeSpawnZombieTracking =GetTime();

                UpdateExistanceTime(plantArr,0);
                }else{
                //used to spawn sun appropriately
                 timeSpawnSunTracking =GetTime()-timeSpentAtMenu;

                //used to spawn zombies appropriately
                 timeSpawnZombieTracking =GetTime()-timeSpentAtMenu;

                UpdateExistanceTime(plantArr,timeSpentAtMenu);
                }

                //spawn of the suns
                if((timeSpawnSunTracking-timeOfLastSun>spawnRateSun)&&indexOfNextSun<SIZE_OF_SUN_ARR){
                    AddRandomlySunToArr(sunArray, &indexOfNextSun,lawnRectangles,groundOfTheSuns);
                    timeOfLastSun=GetTime();
                }
                
                //wait more for the first zombie spawn
                if(firstZombieSpawn){
                    if((timeSpawnZombieTracking-timeOfLastZombie>timeForFirstSpawnZombie)){
                        PlaySound(SOUND_ZOMBIES_COMING);
                        AddZombieToZombiesArrRandomly(zombieArr,NORMAL_ZOMBIE,&indexOfNextZombie,lawnRectangles);
                        timeOfLastZombie=GetTime();
                        firstZombieSpawn=0;
                        menuWasACTIONED=0;
                    }
                
                //normal zombie spawn
                }else{
                    if((timeSpawnZombieTracking-timeOfLastZombie>spawnRateZombie)&&indexOfNextZombie<SIZE_OF_ZOMBIES_ARR){
                        PlaySound(SOUND_ZOMBIE_SPAWN);
                        AddZombieToZombiesArrRandomly(zombieArr,NORMAL_ZOMBIE,&indexOfNextZombie,lawnRectangles);
                        timeOfLastZombie=GetTime();
                        menuWasACTIONED=0;
                    }
                }


                if(collectSun(sunArray,&indexOfNextSun,groundOfTheSuns)){
                    PlaySound(SOUND_COLLECTING_SUN);
                    addSunToStorage(&sunGamingStorage);
                }


                PutPlantToField(plantArr,&cardSelected,&sunGamingStorage,occupationOfLawn,lawnRectangles,SOUND_PLANTING_PLANT,SOUND_SHOVEL);


                GenerateSunSunflower(plantArr,lawnRectangles,groundOfTheSuns,sunArray,&indexOfNextSun);

                //Menu, if esc pressed
                if(IsKeyPressed(KEY_ESCAPE)){
                    PlaySound(SOUND_PAUSE);
                    currentScreen = MENU;
                    menuWasACTIONED=1;
                }
            } break;

           //CASE ENDGAME?


            case MENU:{

                for (int i = 0; i < GAMING_MENU_OPTIONS_QUANTITY; i++)
                {
                    if (CheckCollisionPointRec(mousePoint, gamingMenuOptionsRec[i])) {
                         if(!gamingMenuOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        gamingMenuOptionsRecHover[i] = 1;
                        
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            timeSpentAtMenu=GetTime()-timeSpawnSunTracking;
                            currentScreen = gamingMenuOptions[i];
                        }
                    }
                    else gamingMenuOptionsRecHover[i] = 0;
                }
            }break;


            case RESUME:{
                currentScreen = GAMEPLAY;
            }break;


            case ABOUT:
            {
                
                // TODO: Update ENDING screen variables here
                if (CheckCollisionPointRec(mousePoint, BTN_GOBACK)) {
                        if(!BTN_GOBACK_HOVER){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        BTN_GOBACK_HOVER = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            currentScreen = HOMEPAGE;
                        }
                    }
                    else {BTN_GOBACK_HOVER = 0;}
            } break;


             case CONFIGURATIONS:
            {

                //playing intromusic once and in loops of its lasting size
                if(previousScreen==HOMEPAGE){
                    if(!IsSoundPlaying(SOUND_HOMEPAGE_MENU)){
                        PlaySound(SOUND_HOMEPAGE_MENU);
                    }
                }else if(previousScreen==GAMEPLAY){
                    if(!IsSoundPlaying(SOUND_GAMEPLAY)){
                        PlaySound(SOUND_GAMEPLAY);
                    }
                }


                 for (int i = 0; i < CONFIGURATIONS_OPTIONS_QUANTITY; i++)
        {
                    if (CheckCollisionPointRec(mousePoint, configurationsOptionsRec[i])) {
                        if(!configurationsOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        configurationsOptionsRecHover[i] = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            if(configurationsOptions[i]==CONFIGURATIONS_BTN_GOBACK){
                                StopSound(SOUND_HOMEPAGE_MENU);
                                currentScreen = previousScreen;
                                previousScreen=currentScreen;

                                //if the sound isn't muted, then mute it
                            }else if(!mutedSounds[i]){
                                // TODO: Handle other configuration options
                                //mute the respective sound according to the type of the button
                                switch (configurationsOptions[i])
                                {
                                case CONFIGURATIONS_SOUND_ZOMBIES:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,0.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_PLANTS:{
                                    SetSoundVolume(SOUND_PLANTING_PLANT,0.0f);
                                    SetSoundVolume(SOUND_SHOVEL,0.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,0.0f);
                                }break;

                                 case CONFIGURATIONS_SOUND_BACKGROUND:{
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.0f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_PEASHOTS:{
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_SUNS:{
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_SOUNDEFFECTS:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,0.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,0.0f);
                                    SetSoundVolume(SOUND_PLANTING_PLANT,0.0f);
                                    SetSoundVolume(SOUND_SHOVEL,0.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,0.0f);
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.0f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.0f);
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.0f);
                                }break;
                                default:
                                    break;
                                }
                                //update the variable
                                mutedSounds[i]=1;

                                //if the sound is muted, unmute it
                            } else if(mutedSounds[i]){
                                switch (configurationsOptions[i])
                                {
                                case CONFIGURATIONS_SOUND_ZOMBIES:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,1.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,1.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_PLANTS:{
                                    SetSoundVolume(SOUND_PLANTING_PLANT,1.0f);
                                    SetSoundVolume(SOUND_SHOVEL,1.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,1.0f);
                                }break;

                                 case CONFIGURATIONS_SOUND_BACKGROUND:{
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.4f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.4f);
                                }break;

                                case CONFIGURATIONS_SOUND_PEASHOTS:{
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,1.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_SUNS:{
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.6f);
                                }break;

                                case CONFIGURATIONS_SOUND_SOUNDEFFECTS:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,1.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,1.0f);
                                    SetSoundVolume(SOUND_PLANTING_PLANT,1.0f);
                                    SetSoundVolume(SOUND_SHOVEL,1.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,1.0f);
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.4f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.4f);
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.6f);
                                }break;
                                
                                default:
                                    break;
                                }
                                mutedSounds[i]=0;
                            }
                            //and update the soundsmuted variavel
                        }
                    }
                    else {configurationsOptionsRecHover[i] = 0;}
            }} break;


             case EXIT:
            {
            //Confirming exit                
               
            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            if (CheckCollisionPointRec(mousePoint, GOBACK_BTN_CPY)) {
                        if(!GOBACK_BTN_CPY_HOVER){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        GOBACK_BTN_CPY_HOVER = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            currentScreen = previousScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            previousScreen=currentScreen;
                        }
            }
            else if (CheckCollisionPointRec(mousePoint, EXIT_BTN_CPY)) {
                        if(!EXIT_BTN_CPY_HOVER){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        EXIT_BTN_CPY_HOVER = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            StopSound(SOUND_HOMEPAGE_MENU);
                           exitWindow = true;
                        }
                        }
              else{  GOBACK_BTN_CPY_HOVER = 0;
            EXIT_BTN_CPY_HOVER=0;}
                
            if (IsKeyPressed(KEY_Y)) exitWindow = true;
            else if (IsKeyPressed(KEY_N)) {
                currentScreen=previousScreen;
            };
        
            } break;


            default: break;
        }
       

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("Pedrokas's gaming enterprise® \n\t\t\t\t\t\t\t\tpresents... ", 70, 150, 40, DARKGREEN);
                   

                } break;
                case HOMEPAGE:
                {
                    // TODO: Draw HOMEPAGE screen here!

                    //Background
                    
                    DrawTexturePro(TEXTURE_BACKGROUND_IMG,TEXTURE_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    
                    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
                    // DrawTexturePro(NORMAL_GREEN_PEASHOT.texture,TEXTURE_GREEN_PEASHOT_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);

                    
                    DrawTexturePro(TEXTURE_PLAY_BTN_IMG,TEXTURE_PLAY_BTN_IMG_SOURCE_REC,homePageOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_LEADERBOARD_BTN_IMG,TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC,homePageOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_ABOUT_BTN_IMG,TEXTURE_ABOUT_BTN_IMG_SOURCE_REC,homePageOptionsRec[2],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,homePageOptionsRec[3],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,homePageOptionsRec[4],origin,0.0f,WHITE);

                } break;
                case USER_DATA:
                {
                    // TODO: Draw USERDATA screen here!
                  
                    //Draw a fixed rectangle that fits 8 words
                    Rectangle textBox = { SCREEN_WIDTH/2 - 150, 180,  MeasureText("A",40)*8+30, 50 };
                    //if username more than 8 words, update the box
                    if(sizeOfName>8){
                        textBox.width = sizeOfName*MeasureText("A",40)+10;
                    }
                    
                    //Title
                    DrawText("Tell me your name!", 240, 140, 20, GRAY);

                    //TextInput
                    DrawRectangleRec(textBox, LIGHTGRAY);
                    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
                    DrawText(playerName, textBox.x + 5, textBox.y + 8, 40, MAROON);

                    //Text alone
                    DrawText(TextFormat("Press enter to confirm: %i/%i", sizeOfName, MAX_SIZE_OF_NAME-1), 315, 250, 20, DARKGRAY);


                    // Draw blinking underscore char
                    if (sizeOfName < MAX_SIZE_OF_NAME) {
                     DrawText("_",( textBox.x + 8 + MeasureText(playerName, 40)),(textBox.y + 12), 40, MAROON);
                    } 
                  

                } break;


                 case DECK_SELECTION:
                {
                    
                    // // TODO: Draw ENDING screen here!
                    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
                    // DrawText(TextFormat("Nice to meet you, %s :)", playerName), 20, 20, 40, DARKBLUE);
                    // DrawText("Select you deck", 120, 220, 20, DARKBLUE);
                    currentScreen = GAMEPLAY;

                } break;


                case GAMEPLAY:
                {
                    //TODO:Draw GAMEPLAY screen here!
                    DrawTexturePro(TEXTURE_GAMING_BACKGROUND_IMG,TEXTURE_GAMING_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    
                    //Lawn drawing
                    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
                        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                            //if "i" is odd, if "j" is odd, dirt, else, grass
                            if(i&1){
                                if(j&1){
                                    DrawTexturePro(TEXTURE_DIRT_IMG,TEXTURE_DIRT_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                    // DrawRectangleRec( lawnRectangles[i][j], dirt);
                                }else{
                                    // DrawRectangleRec( lawnRectangles[i][j], grass);
                                    DrawTexturePro(TEXTURE_GRASS_IMG,TEXTURE_GRASS_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                }
                            }else{
                                if(j&1){
                                    DrawTexturePro(TEXTURE_GRASS_IMG,TEXTURE_GRASS_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                }else{
                                    DrawTexturePro(TEXTURE_DIRT_IMG,TEXTURE_DIRT_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                }
                            }
                            //tracking hover over the options
                            if(lawnRectanglesHover[i][j]==true){
                                DrawRectangleLines( lawnRectangles[i][j].x,  lawnRectangles[i][j].y,  lawnRectangles[i][j].width,  lawnRectangles[i][j].height, BLACK);
                            }
                        }
                    }

                    //Plants
                    DrawPlants(plantArr);

                    //Peashots
                    DrawPeaShots(peaShotsArr,indexOfNextPea);

                    //Zombies
                    DrawZombieArr(zombieArr,indexOfNextZombie);

                    //Suns
                    DrawSuns(sunArray,indexOfNextSun,TEXTURE_SUN_IMG);

                    //Gaming deck
                    DrawGamingDeck(DeckOfPlants,sunGamingStorage, &cardSelected);
                    DrawMoldureOfSelectedCard(cardSelected);
                    RemoveSelectedCard(&cardSelected);

                }break;


                //  case END_GAME:{
                //      
                //      DrawText("YOU LOST!", (SCREEN_WIDTH-50)/2, 20, 50, DARKGREEN);
                //     DrawRectangle((SCREEN_WIDTH-50)/2+,, 20,, WHITE);
                //     if(gamingMenuOptionsRecHover[i]==true){
                //              DrawRectangleRec(gamingMenuOptionsRec[i], RED);
                //             }
                //         }break;


                case MENU:{
                    //Background 
                    DrawTexturePro(TEXTURE_BACKGROUND_IMG,TEXTURE_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    //Drawing Menu options
                    DrawTexturePro(TEXTURE_RESUME_BTN_IMG,TEXTURE_RESUME_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[2],origin,0.0f,WHITE);
                }break;


                case ABOUT:
                {
                    
                    // TODO: Draw ABOUT screen here!
                    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
                    ClearBackground(RAYWHITE);
                    DrawAboutScreen();
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,BTN_GOBACK,origin,0.0f,WHITE);
                    // DrawText("INF vs ZOMBIES is a fun and strategic tower defense game inspired by Plants vs Zombies, developed as a project for the Algorithms and Programming course at the Federal University of Rio Grande do Sul (UFRGS).\nPlayers must defend their base from waves of zombies by placing different plants, each with unique abilities, along a grid. Strategy and quick thinking are key to surviving the endless undead hordes!\nBuilt from scratch in C using the Raylib library, this project was designed to recreate the fun of the original game while strengthening my C programming skills, problem-solving ability, and proactive development approach. Every line of code was crafted to deepen my understanding of efficient logic, clean structure, and real-time game mechanics.\nWhether you're a fan of tower defense games or just curious about game development in C, INF vs ZOMBIES offers a mix of challenge and creativity—with a little programming twist!\nReady to stop the zombie invasion? Let the battle begin!\n(Educational project for UFRGS - Instituto de Informática)", 20, 20, 40, BLACK);
                  

                } break;


                 case CONFIGURATIONS:
                {
                    // TODO: Draw CONFIGURATIONS screen here!
                    //Background
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BACKGROUND_IMG,TEXTURE_CONFIGURATIONS_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);

                    
                    DrawTexturePro(TEXTURE_PLAY_BTN_IMG,TEXTURE_PLAY_BTN_IMG_SOURCE_REC,homePageOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_LEADERBOARD_BTN_IMG,TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC,homePageOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_ABOUT_BTN_IMG,TEXTURE_ABOUT_BTN_IMG_SOURCE_REC,homePageOptionsRec[2],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,homePageOptionsRec[3],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,homePageOptionsRec[4],origin,0.0f,WHITE);

                    DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND],origin,0.0f,WHITE);
                    if(mutedSounds[CONFIGURATIONS_SOUND_BACKGROUND]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_BACKGROUND_BTN_IMG,TEXTURE_BACKGROUND_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_BACKGROUND],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_ZOMBIES]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_ZOMBIES],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_ZOMBIES],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_ZOMBIES_BTN_IMG,TEXTURE_ZOMBIES_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_ZOMBIES],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_PLANTS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PLANTS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PLANTS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_PLANTS_BTN_IMG,TEXTURE_PLANTS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_PLANTS],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_PEASHOTS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PEASHOTS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PEASHOTS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_PEASHOTS_BTN_IMG,TEXTURE_PEASHOTS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_PEASHOTS],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_SUNS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SUNS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SUNS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_SUNS_BTN_IMG,TEXTURE_SUNS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_SUNS],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_SOUNDEFFECTS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SOUNDEFFECTS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SOUNDEFFECTS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_SOUNDEFFECTS_BTN_IMG,TEXTURE_SOUNDEFFECTS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_SOUNDEFFECTS],origin,0.0f,WHITE);
      
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK],origin,0.0f,WHITE);                      
                } break;


                 case EXIT:
                {
                    // TODO: Draw EXIT screen here!
                    DrawTexturePro(TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG,TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,GOBACK_BTN_CPY,origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,EXIT_BTN_CPY,origin,0.0f,WHITE);
            
                } break;
                default: break;
            }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadAllTextures();
    UnloadAllSounds();
    
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //-------------------------------------------------------------------------------------

    return 0;

}