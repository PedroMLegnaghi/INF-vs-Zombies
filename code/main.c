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
#include "lawn.h"
#include "configMenu.h"
#include "gamingMenu.h"
#include "deck.h"
#include "Player.h"
#include "sun.h"
#include "zombies.h"
#include "menu.h"
#include "gameplay.h"
#include "file.h"
//CONSTANTS=======================================================================================================================================

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

//--screen
srand(time(NULL));
InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,"raylib [core] example - basic screen manager");
    //tracks the screen of the user
    GAME_SCREEN currentScreen = LOGO;
    GAME_SCREEN previousScreen = HOMEPAGE;

//--audio
InitAudioDevice();
LoadAllSounds();
    
//--textures
LoadAllTextures();

//--structs
InitGameStructs();

//--buttons
InitButtons();

//--HomePage
InitHomePage();

//Configurations Menu
InitConfigurationsMenu();

//--Gaming Menu
InitGamingMenu();

//--exiting application
SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works
bool exitWindow = false;    // Flag to set window to exit
    
//--frameManagement
int framesCounter = 0;          // Useful to count frames
SetTargetFPS(TARGET_FPS);               // Set desired framerate (frames-per-second)


//--DECK
InitDeckOfPlants();

//--PLANT
InitPlantArr();

//--LAWN
InitLawnRectangles();

//--ZOMBIES
InitZombiesArrs(zombieArr);

//--FILES 
//criar se nao existir
if(!(leaderBoardFile = fopen("leaderboard.bin","rb+"))){
    leaderBoardFile=fopen("leaderboard.bin", "wb+");
}
importPlayersFromFile(leaderBoardFile,leaderBoardTop5Players);
zombiesHordesText = fopen("zombies.txt","r");
importZombiesFromFile(zombiesHordesText,zombiesQuantityPerHorde,&quantityOfHordes);
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
                framesCounter++;    // Count frames

                // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
                if (framesCounter > 2)
                {
                    currentScreen = HOMEPAGE;
                }
            } break;

            
            case HOMEPAGE:
            {
                //updating the top5 leaderboard and resetting (if had happened) the previous gameplay
                if(gameHasEnded){
                    reorderTop5(player,leaderBoardTop5Players);
                    StopSound(SOUND_GAMEPLAY);
                    resetGameplay();
                }

                //playing intromusic once and in loops of its lasting size
                if(!IsSoundPlaying(SOUND_HOMEPAGE_MENU)){
                    PlaySound(SOUND_HOMEPAGE_MENU);
                }

                
                 for (int i = 0; i < HOME_PAGE_OPTIONS_QUANTITY; i++)
                    {
                    //if user hovered the button
                    if (CheckCollisionPointRec(mousePoint, homePageOptionsRec[i])) {
                        //and the button is not already hovered
                        if(!homePageOptionsRecHover[i]){
                            //play sound
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        //activate the flag that it's being hovered
                        homePageOptionsRecHover[i] = 1;
                        //if user clicked the button
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            currentScreen = homePageOptions[i];
                        }
                    }
                    //else, turn off flag btn is being hovered (it isn't)
                    else {homePageOptionsRecHover[i] = 0;}
                    }
                    
                } break;
                
                
                case PLAY:
                {
                    currentScreen = USER_DATA;//first thing to be done, is to ask for the User data          
            } break;



              case USER_DATA:
            {
                previousScreen=currentScreen;
                

                // Get char pressed (unicode character) on the queue
                int key = GetCharPressed();
                
                // Check if more characters have been pressed on the same frame
                while (key > 0) {
                    // NOTE: Only allow keys in range [32..125]
                    //MAX_SIZE_OF_NAME-1 to garantee the space for '\0'
                    if ((key >= 32) && (key <= 125) && (strlen(player.playerName) < MAX_SIZE_OF_NAME-1)) {
                        player.playerName[strlen(player.playerName)] = (char)key;
                    }
                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    // if (strlen(player.playerName) == 0){
                    //     strlen(player.playerName) = 0;
                    // }else{
                    //     strlen(player.playerName)--;
                    // }
                    player.playerName[strlen(player.playerName)-1] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && strlen(player.playerName) > 0) {
                    currentScreen = GAMEPLAY; 
                    
                    //initializate variables used to track spawn of zombies and sun
                    timeOfLastZombie=GetTime();
                    timeOfLastSun=GetTime();
                
                }
            } break;
            
            
            case GAMEPLAY:
            {   
                previousScreen=currentScreen;
                if(!IsSoundPlaying(SOUND_GAMEPLAY)){
                    
                    PlaySound(SOUND_GAMEPLAY);
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
                
                //if menu was not actioned, spawn things normaly
                if(!menuWasACTIONED){
                    
                    //used to spawn sun appropriately
                    timeSpawnSunTracking =GetTime();
                    
                    //used to spawn zombies appropriately
                    timeSpawnZombieTracking =GetTime();
                    
                    UpdateExistanceTime(plantArr,0);
                    
                    //if menu was actioned, then deals with spawn considering time spent at menu    
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
                
                //quantityOfHordes
                //flag to control if it is the last zombie (se o zombiesCreated for igual ao hordesZombies[i]), nao spawn ate
                //flag to control if the last zombie has died (se o zombie[0] é invalido!!!!!)
                //morrer o ultimo zombie, quando finalmente comecaremos a outra horda (i++, zombiesCreated=0)
                if(firstZombieSpawn){    
                    //if it is the first zombie spawn, then spawn it after (timeForFirstSpawnZombie) seconds
                    if((timeSpawnZombieTracking-timeOfLastZombie>timeForFirstSpawnZombie)){
                        PlaySound(SOUND_ZOMBIES_COMING);
                        AddZombieToZombiesArrRandomly(zombieArr,NORMAL_ZOMBIE,&indexOfNextZombie,lawnRectangles);
                        zombiesCreatedSinceLastHorde++;
                        timeOfLastZombie=GetTime();
                        firstZombieSpawn=0;
                        menuWasACTIONED=0;
                    }
                    //normal zombie spawn
                }else{
                    if(
                        (timeSpawnZombieTracking-timeOfLastZombie>spawnRateZombie)&&
                        (indexOfNextZombie<SIZE_OF_ZOMBIES_ARR)&&
                        (!LastZombieOfHordeSpawned())
                    ){
                        PlaySound(SOUND_ZOMBIE_SPAWN);
                        AddZombieToZombiesArrRandomly(zombieArr,NORMAL_ZOMBIE,&indexOfNextZombie,lawnRectangles);
                        zombiesCreatedSinceLastHorde++;
                        timeOfLastZombie=GetTime();
                        menuWasACTIONED=0;
                    }
                }

                //reset horde or win
                if(LastZombieOfHordeDied()){
                    //win match
                    if((indexOfCurrentHorde+1)==quantityOfHordes){
                        currentScreen=WIN;
                        StopSound(SOUND_GAMEPLAY);
                        PlaySound(SOUND_WIN);
                    }
                    ResetZombieHorde();
                }
                
                //if zombie has gone out of the screen
                if(updatePlantsAndZombiesGameplay(plantArr,peaShotsArr,zombieArr,occupationOfLawn,&indexOfNextPea,&indexOfNextZombie,SOUND_PEASHOT_IMPACT,SOUND_ZOMBIE_EAT_PLANT)){
                    PlaySound(SOUND_LOST_MUSIC);
                    StopSound(SOUND_GAMEPLAY);
                    currentScreen=END_GAME;
                }
                
               
                collectSun(sunArray,&indexOfNextSun,groundOfTheSuns);
                
                
                PutPlantToField(plantArr,&cardSelected,&sunGamingStorage,occupationOfLawn,lawnRectangles,SOUND_PLANTING_PLANT,SOUND_SHOVEL);

                
                GenerateSunSunflower(plantArr,lawnRectangles,groundOfTheSuns,sunArray,&indexOfNextSun);

                //Menu, if esc pressed
                if(IsKeyPressed(KEY_ESCAPE)){
                    PlaySound(SOUND_PAUSE);
                    currentScreen = MENU;
                    menuWasACTIONED=1;
                }
  
            } break;



            case MENU:{

                for (int i = 0; i < GAMING_MENU_OPTIONS_QUANTITY; i++)
                {
                    if (CheckCollisionPointRec(mousePoint, gamingMenuOptionsRec[i])) 
                    {
                        if(!gamingMenuOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        gamingMenuOptionsRecHover[i] = 1;
                        
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            //here the time of the last sun spawned was used as reference to update the timeSpentAtMenu
                            timeSpentAtMenu=GetTime()-timeSpawnSunTracking;
                            currentScreen = gamingMenuOptions[i];
                        }
                    }
                    else gamingMenuOptionsRecHover[i] = 0;
                }
            }break;

            case END_GAME:{
                if (CheckCollisionPointRec(mousePoint, BTN_ENDGAME_GOBACK)) 
                    {
                        if(!BTN_ENDGAME_GOBACK_HOVER)
                        {
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        BTN_ENDGAME_GOBACK_HOVER = 1;

                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            currentScreen = HOMEPAGE;
                        }
                    }else {
                        BTN_ENDGAME_GOBACK_HOVER = 0;
                    }
                    gameHasEnded=true;
                    

            }break;

            case WIN:{
                if (CheckCollisionPointRec(mousePoint, BTN_ENDGAME_GOBACK)) 
                    {
                        if(!BTN_ENDGAME_GOBACK_HOVER)
                        {
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        BTN_ENDGAME_GOBACK_HOVER = 1;

                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            currentScreen = HOMEPAGE;
                        }
                    }else {
                        BTN_ENDGAME_GOBACK_HOVER = 0;
                    }
                    gameHasEnded=true;
                    

            }break;


            case RESUME:{
                currentScreen = GAMEPLAY;
            }break;

            case ABOUT:
            {
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

            case LEADERBOARD: {
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
                                timeSpentAtMenu=GetTime()-timeSpawnSunTracking;
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
                    if(strlen(player.playerName)>8){
                        textBox.width = strlen(player.playerName)*MeasureText("A",40)+10;
                    }
                    
                    //Title
                    DrawText("Tell me your name!", 240, 140, 20, GRAY);

                    //TextInput
                    DrawRectangleRec(textBox, LIGHTGRAY);
                    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
                    DrawText(player.playerName, textBox.x + 5, textBox.y + 8, 40, MAROON);

                    //Text alone
                    DrawText(TextFormat("Press enter to confirm: %i/%i", strlen(player.playerName), MAX_SIZE_OF_NAME-1), 315, 250, 20, DARKGRAY);


                    // Draw blinking underscore char
                    if (strlen(player.playerName) < MAX_SIZE_OF_NAME) {
                     DrawText("_",( textBox.x + 8 + MeasureText(player.playerName, 40)),(textBox.y + 12), 40, MAROON);
                    } 
                  

                } break;

                case GAMEPLAY:
                {
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


                 case END_GAME:{
                     
                    DrawTexturePro(TEXTURE_ENDGAME_BACKGROUND_IMG,TEXTURE_ENDGAME_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,BTN_ENDGAME_GOBACK,origin,0.0f,WHITE);
                        }break;

                case WIN:{
                    DrawTexturePro(TEXTURE_WIN_BACKGROUND_IMG,TEXTURE_WIN_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,BTN_ENDGAME_GOBACK,origin,0.0f,WHITE);
                        }break;


                case MENU:{
                    //Background 
                    DrawTexturePro(TEXTURE_BACKGROUND_IMG,TEXTURE_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    //Drawing Menu options
                    DrawTexturePro(TEXTURE_RESUME_BTN_IMG,TEXTURE_RESUME_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[2],origin,0.0f,WHITE);
                }break;

                case LEADERBOARD:{
                    drawTopFivePlayers(leaderBoardTop5Players);
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,BTN_GOBACK,origin,0.0f,WHITE);
                }break;

                case ABOUT:
                {
                    
                    ClearBackground(RAYWHITE);
                    DrawAboutScreen();
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,BTN_GOBACK,origin,0.0f,WHITE);
                  

                } break;


                 case CONFIGURATIONS:
                {
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
    UnloadAllTextures();
    UnloadAllSounds();
    recordPlayersToFile(leaderBoardFile,leaderBoardTop5Players);
    fclose(leaderBoardFile);
    fclose(zombiesHordesText);
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //-------------------------------------------------------------------------------------

    return 0;

}