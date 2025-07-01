#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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

// CONSTANTS=================================================================================================

// User's Mouse
Vector2 mousePoint = {0.0f, 0.0f};

// ScreenRectangle(used to display textures)
const Rectangle SCREEN_RECTANGLE = {
    .height = SCREEN_HEIGHT,
    .width = SCREEN_WIDTH,
    .x = 0,
    .y = 0};

// track the screen of the user
GAME_SCREEN currentScreen = LOGO;
GAME_SCREEN previousScreen = HOMEPAGE;

int main(void)
{

    //--screen
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic screen manager");

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

    // Configurations Menu
    InitConfigurationsMenu();

    //--Gaming Menu
    InitGamingMenu();

    //--exiting application
    SetExitKey(KEY_NULL);    // Disable KEY_ESCAPE to close window, X-button still works
    bool exitWindow = false; // Flag to set window to exit

    //--frame Management
    int gameFramesCounter = 0;
    SetTargetFPS(TARGET_FPS);

    //--DECK
    InitDeckOfPlants();

    //--PLANT
    InitPlantArr();

    //--LAWN
    InitLawnRectangles();

    //--ZOMBIES
    InitZombiesArrs(zombieArr);

    //--FILES
    if (!(leaderBoardFile = fopen("leaderboard.bin", "rb+")))
    {
        leaderBoardFile = fopen("leaderboard.bin", "wb+");
    }
    importPlayersFromFile(leaderBoardFile, leaderBoardTop5Players);
    zombiesHordesText = fopen("zombies.txt", "r");
    importZombiesFromFile(zombiesHordesText, zombiesQuantityPerHorde, &quantityOfHordes);

    // MAIN LOOP GAME====================================================================================================================================================================================
    while (!exitWindow) // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition(); // tracking, effectively, the user's mouse each iteration

        // if user wants to end applications, redirect him to an exit confirmation
        if (WindowShouldClose())
        {
            currentScreen = EXIT;
        }

        // Switch that updates the variables of the current screen
        switch (currentScreen)
        {
        case LOGO:
        {

            gameFramesCounter++;
            // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
            if (gameFramesCounter > 420)
            {
                currentScreen = HOMEPAGE;
            }
        }
        break;

        case HOMEPAGE:
        {
            // updating the top5 leaderboard and resetting the previous gameplay (if it happened)
            if (gameHasEnded)
            {
                reorderTop5(player, leaderBoardTop5Players);
                StopSound(SOUND_GAMEPLAY);
                resetGameplay();
            }

            PlaySoundInLoop(SOUND_HOMEPAGE_MENU);
            HandleHomePageButtons();
        }
        break;

        case PLAY:
        {
            currentScreen = USER_DATA; // first thing to be done, is to ask for the User data
        }
        break;

        case USER_DATA:
        {
            previousScreen = currentScreen;

            // when user presses enter, the functions return 1, and we can start the gameplay
            if (handlePlayerNameInput())
            {
                // initializate variables used to track spawn of zombies and sun
                timeOfLastZombie = GetTime();
                timeOfLastSun = GetTime();
                currentScreen = GAMEPLAY;
            }
        }
        break;

        case GAMEPLAY:
        {
            StopSound(SOUND_HOMEPAGE_MENU);
            PlaySoundInLoop(SOUND_GAMEPLAY);

            previousScreen = currentScreen;

            updateSunsPosition(sunArray, indexOfNextSun, groundOfTheSuns);

            ManageLawnHover();

            // if menu was not actioned, spawn things normaly
            if (!menuWasACTIONED)
            {

                // used to spawn sun appropriately
                timeSpawnSunTracking = GetTime();

                // used to spawn zombies appropriately
                timeSpawnZombieTracking = GetTime();

                UpdateExistanceTime(plantArr, 0);
            }
            // if menu was actioned, then deals with spawn considering time spent at menu
            else
            {
                // used to spawn sun appropriately
                timeSpawnSunTracking = GetTime() - timeSpentAtMenu;

                // used to spawn zombies appropriately
                timeSpawnZombieTracking = GetTime() - timeSpentAtMenu;

                UpdateExistanceTime(plantArr, timeSpentAtMenu);
            }

            // spawn of the suns
            if ((timeSpawnSunTracking - timeOfLastSun > spawnRateSun) && indexOfNextSun < SIZE_OF_SUN_ARR)
            {
                AddRandomlySunToArr(sunArray, &indexOfNextSun, lawnRectangles, groundOfTheSuns);
                timeOfLastSun = GetTime();
            }

            // quantityOfHordes
            if (firstZombieSpawn)
            {
                // if it is the first zombie spawn, then spawn it after (timeForFirstSpawnZombie) seconds
                if ((timeSpawnZombieTracking - timeOfLastZombie > timeForFirstSpawnZombie))
                {
                    PlaySound(SOUND_ZOMBIES_COMING);
                    // zombie to be spawned this time
                    Zombie zombieToBeSpawned = chooseZombieToSpawn(0);
                    AddZombieToZombiesArrRandomly(zombieArr, zombieToBeSpawned, &indexOfNextZombie, lawnRectangles);
                    zombiesCreatedSinceLastHorde++;
                    timeOfLastZombie = GetTime();
                    firstZombieSpawn = 0;
                    menuWasACTIONED = 0;
                }
            }
            // else, normal zombie spawn
            else
            {
                if (
                    (timeSpawnZombieTracking - timeOfLastZombie > spawnRateZombie) &&
                    (indexOfNextZombie < SIZE_OF_ZOMBIES_ARR) &&
                    (!LastZombieOfHordeSpawned()))
                {
                    PlaySound(SOUND_ZOMBIE_SPAWN);
                    Zombie zombieToBeSpawned = chooseZombieToSpawn(isLastZombieOfGame());
                    AddZombieToZombiesArrRandomly(zombieArr, zombieToBeSpawned, &indexOfNextZombie, lawnRectangles);
                    zombiesCreatedSinceLastHorde++;
                    timeOfLastZombie = GetTime();
                    menuWasACTIONED = 0;
                }
            }

            // reset horde or win
            if (LastZombieOfHordeDied())
            {
                // win match
                if ((indexOfCurrentHorde + 1) == quantityOfHordes)
                {
                    currentScreen = WIN;
                    StopSound(SOUND_GAMEPLAY);
                    PlaySound(SOUND_WIN);
                }
                ResetZombieHorde();
            }

            // if zombie has gone out of the screen
            if (updatePlantsAndZombiesGameplay(plantArr, peaShotsArr, zombieArr, occupationOfLawn, &indexOfNextPea, &indexOfNextZombie, SOUND_PEASHOT_IMPACT, SOUND_ZOMBIE_EAT_PLANT))
            {
                PlaySound(SOUND_LOST_MUSIC);
                StopSound(SOUND_GAMEPLAY);
                currentScreen = END_GAME;
            }

            collectSun(sunArray, &indexOfNextSun, groundOfTheSuns);

            PutPlantToField();

            GenerateSunSunflower(plantArr, lawnRectangles, groundOfTheSuns, sunArray, &indexOfNextSun);

            // Menu, if esc pressed
            if (IsKeyPressed(KEY_ESCAPE))
            {
                PlaySound(SOUND_PAUSE);
                currentScreen = MENU;
                menuWasACTIONED = 1;
            }
        }
        break;

        case MENU:
        {

            HandleGamingMenuButtons();
        }
        break;

        case END_GAME:
        {
            HandleGameplayEnd();
        }
        break;

        case WIN:
        {
            HandleGameplayWin();
        }
        break;

        case RESUME:
        {
            currentScreen = GAMEPLAY;
        }
        break;

        case ABOUT:
        {
            if (CheckCollisionPointRec(mousePoint, BTN_GOBACK))
            {
                if (!BTN_GOBACK_HOVER)
                {
                    PlaySound(SOUND_BTN_HOVER);
                }
                BTN_GOBACK_HOVER = 1;
                if (IsGestureDetected(GESTURE_TAP))
                {
                    PlaySound(SOUND_BTN_CLICK);
                    previousScreen = currentScreen;
                    currentScreen = HOMEPAGE;
                }
            }
            else
            {
                BTN_GOBACK_HOVER = 0;
            }
        }
        break;

        case LEADERBOARD:
        {
            // if the user wants to go back
            if (CheckCollisionPointRec(mousePoint, BTN_GOBACK))
            {
                if (!BTN_GOBACK_HOVER)
                {
                    PlaySound(SOUND_BTN_HOVER);
                }
                BTN_GOBACK_HOVER = 1;
                if (IsGestureDetected(GESTURE_TAP))
                {
                    PlaySound(SOUND_BTN_CLICK);
                    previousScreen = currentScreen;
                    currentScreen = HOMEPAGE;
                }
            }
            else
            {
                BTN_GOBACK_HOVER = 0;
            }
        }
        break;

        case CONFIGURATIONS:
        {
            // playing intromusic once and in loops of its lasting size
            if (previousScreen == HOMEPAGE)
            {
                PlaySoundInLoop(SOUND_HOMEPAGE_MENU);
            }
            else if (previousScreen == GAMEPLAY)
            {
                PlaySoundInLoop(SOUND_GAMEPLAY);
            }

            for (int i = 0; i < CONFIGURATIONS_OPTIONS_QUANTITY; i++)
            {
                if (CheckCollisionPointRec(mousePoint, configurationsOptionsRec[i]))
                {
                    if (!configurationsOptionsRecHover[i])
                    {
                        PlaySound(SOUND_BTN_HOVER);
                    }
                    configurationsOptionsRecHover[i] = 1;
                    if (IsGestureDetected(GESTURE_TAP))
                    {
                        PlaySound(SOUND_BTN_CLICK);
                        if (configurationsOptions[i] == CONFIGURATIONS_BTN_GOBACK)
                        {
                            timeSpentAtMenu = GetTime() - timeSpawnSunTracking;
                            currentScreen = previousScreen;
                            previousScreen = currentScreen;
                        }
                        // if the sound isn't muted, then mute it
                        else if (!mutedSounds[i])
                        {
                            // mute the respective sound according to the type of the button
                            switch (configurationsOptions[i])
                            {
                            case CONFIGURATIONS_SOUND_ZOMBIES:
                            {
                                SetSoundVolume(SOUND_ZOMBIE_SPAWN, 0.0f);
                                SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT, 0.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_PLANTS:
                            {
                                SetSoundVolume(SOUND_PLANTING_PLANT, 0.0f);
                                SetSoundVolume(SOUND_SHOVEL, 0.0f);
                                SetSoundVolume(SOUND_PEASHOT_IMPACT, 0.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_BACKGROUND:
                            {
                                SetSoundVolume(SOUND_HOMEPAGE_MENU, 0.0f);
                                SetSoundVolume(SOUND_GAMEPLAY, 0.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_PEASHOTS:
                            {
                                SetSoundVolume(SOUND_PEASHOT_IMPACT, 0.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_SUNS:
                            {
                                SetSoundVolume(SOUND_COLLECTING_SUN, 0.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_SOUNDEFFECTS:
                            {
                                SetSoundVolume(SOUND_ZOMBIE_SPAWN, 0.0f);
                                SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT, 0.0f);
                                SetSoundVolume(SOUND_PLANTING_PLANT, 0.0f);
                                SetSoundVolume(SOUND_SHOVEL, 0.0f);
                                SetSoundVolume(SOUND_PEASHOT_IMPACT, 0.0f);
                                SetSoundVolume(SOUND_HOMEPAGE_MENU, 0.0f);
                                SetSoundVolume(SOUND_GAMEPLAY, 0.0f);
                                SetSoundVolume(SOUND_COLLECTING_SUN, 0.0f);
                                SetSoundVolume(SOUND_POTATO_MINE_EXPLOSION, 0.0f);
                                SetSoundVolume(SOUND_BTN_CLICK, 0.0f);
                                SetSoundVolume(SOUND_BTN_HOVER, 0.0f);
                                SetSoundVolume(SOUND_LOST_MUSIC, 0.0f);
                                SetSoundVolume(SOUND_PAUSE, 0.0f);
                                SetSoundVolume(SOUND_ZOMBIES_COMING, 0.0f);
                                SetSoundVolume(SOUND_WIN, 0.0f);
                            }
                            break;
                            default:
                                break;
                            }
                            // update the variable
                            mutedSounds[i] = 1;

                            // if the sound is muted, unmute it
                        }
                        else if (mutedSounds[i])
                        {
                            switch (configurationsOptions[i])
                            {
                            case CONFIGURATIONS_SOUND_ZOMBIES:
                            {
                                SetSoundVolume(SOUND_ZOMBIE_SPAWN, 1.0f);
                                SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT, 1.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_PLANTS:
                            {
                                SetSoundVolume(SOUND_PLANTING_PLANT, 1.0f);
                                SetSoundVolume(SOUND_SHOVEL, 1.0f);
                                SetSoundVolume(SOUND_PEASHOT_IMPACT, 1.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_BACKGROUND:
                            {
                                SetSoundVolume(SOUND_HOMEPAGE_MENU, 0.4f);
                                SetSoundVolume(SOUND_GAMEPLAY, 0.4f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_PEASHOTS:
                            {
                                SetSoundVolume(SOUND_PEASHOT_IMPACT, 1.0f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_SUNS:
                            {
                                SetSoundVolume(SOUND_COLLECTING_SUN, 0.6f);
                            }
                            break;

                            case CONFIGURATIONS_SOUND_SOUNDEFFECTS:
                            {
                                SetSoundVolume(SOUND_ZOMBIE_SPAWN, 1.0f);
                                SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT, 1.0f);
                                SetSoundVolume(SOUND_PLANTING_PLANT, 1.0f);
                                SetSoundVolume(SOUND_SHOVEL, 1.0f);
                                SetSoundVolume(SOUND_PEASHOT_IMPACT, 1.0f);
                                SetSoundVolume(SOUND_HOMEPAGE_MENU, 0.4f);
                                SetSoundVolume(SOUND_GAMEPLAY, 0.4f);
                                SetSoundVolume(SOUND_COLLECTING_SUN, 0.6f);
                                SetSoundVolume(SOUND_BTN_HOVER, 1.0f);
                                SetSoundVolume(SOUND_BTN_CLICK, 1.0f);
                                SetSoundVolume(SOUND_LOST_MUSIC, 1.0f);
                                SetSoundVolume(SOUND_PAUSE, 1.0f);
                                SetSoundVolume(SOUND_ZOMBIES_COMING, 1.0f);
                                SetSoundVolume(SOUND_WIN, 1.0f);
                                SetSoundVolume(SOUND_POTATO_MINE_EXPLOSION, 1.0f);
                            }
                            break;

                            default:
                                break;
                            }
                            mutedSounds[i] = 0;
                        }
                    }
                }
                else
                {
                    configurationsOptionsRecHover[i] = 0;
                }
            }
        }
        break;

        case EXIT:
        {
            // Confirming exit

            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            if (CheckCollisionPointRec(mousePoint, GOBACK_BTN_CPY))
            {
                if (!GOBACK_BTN_CPY_HOVER)
                {
                    PlaySound(SOUND_BTN_HOVER);
                }
                GOBACK_BTN_CPY_HOVER = 1;

                if (IsGestureDetected(GESTURE_TAP))
                {
                    PlaySound(SOUND_BTN_CLICK);
                    currentScreen = previousScreen;
                    previousScreen = currentScreen;
                }
            }
            else if (CheckCollisionPointRec(mousePoint, EXIT_BTN_CPY))
            {
                // if the btn is not already being hovered
                if (!EXIT_BTN_CPY_HOVER)
                {
                    PlaySound(SOUND_BTN_HOVER);
                }
                EXIT_BTN_CPY_HOVER = 1;

                if (IsGestureDetected(GESTURE_TAP))
                {
                    PlaySound(SOUND_BTN_CLICK);
                    exitWindow = true;
                }
            }
            else
            {
                GOBACK_BTN_CPY_HOVER = 0;
                EXIT_BTN_CPY_HOVER = 0;
            }

            if (IsKeyPressed(KEY_Y))
                exitWindow = true;
            else if (IsKeyPressed(KEY_N))
                currentScreen = previousScreen;
        }
        break;

        default:
            break;
        }

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            DrawTexturePro(TEXTURE_LOGO_BACKGROUBND_IMG, TEXTURE_LOGO_BACKGROUBND_IMG_SOURCE_REC, SCREEN_RECTANGLE, (Vector2){0, 0}, 0.0f, WHITE);
        }
        break;
        case HOMEPAGE:
        {

            // Background
            DrawTexturePro(TEXTURE_BACKGROUND_IMG, TEXTURE_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_PLAY_BTN_IMG, TEXTURE_PLAY_BTN_IMG_SOURCE_REC, homePageOptionsRec[0], origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_LEADERBOARD_BTN_IMG, TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC, homePageOptionsRec[1], origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_ABOUT_BTN_IMG, TEXTURE_ABOUT_BTN_IMG_SOURCE_REC, homePageOptionsRec[2], origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG, TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC, homePageOptionsRec[3], origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_EXIT_BTN_IMG, TEXTURE_EXIT_BTN_IMG_SOURCE_REC, homePageOptionsRec[4], origin, 0.0f, WHITE);
        }
        break;
        case USER_DATA:
        {
            int textBoxWidth = MeasureText("A", 40) * 8 + (SCREEN_WIDTH / 30);
            int textBoxHeight = SCREEN_HEIGHT / 20;
            // Draw a fixed rectangle that fits 8 words
            Rectangle textBox = {(SCREEN_WIDTH - textBoxWidth) / 2, (SCREEN_HEIGHT - textBoxHeight) / 2, textBoxWidth, textBoxHeight};
            // if username more than 8 words, update the box
            if (strlen(player.playerName) > 8)
            {
                textBox.width = strlen(player.playerName) * MeasureText("A", 40) + (SCREEN_WIDTH / 90);
            }

            // Background
            DrawTexturePro(TEXTURE_USERDATA_BACKGROUND_IMG, TEXTURE_USERDATA_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, RAYWHITE);

            // TextInput
            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, GREEN);
            DrawText(player.playerName, textBox.x + 5, textBox.y + 8, 40, GREEN);

            // Text alone
            DrawText(TextFormat("Press enter to confirm: %i/%i", strlen(player.playerName), MAX_SIZE_OF_NAME - 1), textBox.x, textBox.y - 40, 20, GREEN);

            // Draw blinking underscore char
            if (strlen(player.playerName) < MAX_SIZE_OF_NAME)
            {
                DrawText("_", (textBox.x + 8 + MeasureText(player.playerName, 40)), (textBox.y + 12), 40, GREEN);
            }
        }
        break;

        case GAMEPLAY:
        {

            // Lawn drawing
            DrawTexturePro(TEXTURE_GAMING_BACKGROUND_IMG, TEXTURE_GAMING_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);
            for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
            {
                for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
                {
                    // // tracking the hover over the options
                    // if (lawnRectanglesHover[i][j] == true)
                    // {
                    //     DrawRectangleLines(lawnRectangles[i][j].x, lawnRectangles[i][j].y, lawnRectangles[i][j].width, lawnRectangles[i][j].height, BLACK);
                    // }
                }
            }

            // Plants
            DrawPlants(plantArr);

            // Peashots
            DrawPeaShots(peaShotsArr, indexOfNextPea);

            // Zombies
            DrawZombieArr(zombieArr, indexOfNextZombie);

            // Suns
            DrawSuns(sunArray, indexOfNextSun, TEXTURE_SUN_IMG);

            // Gaming deck
            DrawGamingDeck_SelectCard();
            DrawMoldureOfSelectedCard();
            if (IsKeyPressed(KEY_W) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
            {
                RemoveSelectedCard();
            }
        }
        break;

        case END_GAME:
        {

            DrawTexturePro(TEXTURE_ENDGAME_BACKGROUND_IMG, TEXTURE_ENDGAME_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_GOBACK_BTN_IMG, TEXTURE_GOBACK_BTN_IMG_SOURCE_REC, BTN_ENDGAME_GOBACK, origin, 0.0f, WHITE);
        }
        break;

        case WIN:
        {
            DrawTexturePro(TEXTURE_WIN_BACKGROUND_IMG, TEXTURE_WIN_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_GOBACK_BTN_IMG, TEXTURE_GOBACK_BTN_IMG_SOURCE_REC, BTN_ENDGAME_GOBACK, origin, 0.0f, WHITE);
        }
        break;

        case MENU:
        {
            // Background
            DrawTexturePro(TEXTURE_BACKGROUND_IMG, TEXTURE_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);
            // Drawing Menu options
            DrawTexturePro(TEXTURE_RESUME_BTN_IMG, TEXTURE_RESUME_BTN_IMG_SOURCE_REC, gamingMenuOptionsRec[0], origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG, TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC, gamingMenuOptionsRec[1], origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_EXIT_BTN_IMG, TEXTURE_EXIT_BTN_IMG_SOURCE_REC, gamingMenuOptionsRec[2], origin, 0.0f, WHITE);
        }
        break;

        case LEADERBOARD:
        {
            drawTopFivePlayers(leaderBoardTop5Players);
            DrawTexturePro(TEXTURE_GOBACK_BTN_IMG, TEXTURE_GOBACK_BTN_IMG_SOURCE_REC, BTN_GOBACK, origin, 0.0f, WHITE);
        }
        break;

        case ABOUT:
        {

            ClearBackground(RAYWHITE);
            DrawAboutScreen();
            DrawTexturePro(TEXTURE_GOBACK_BTN_IMG, TEXTURE_GOBACK_BTN_IMG_SOURCE_REC, BTN_GOBACK, origin, 0.0f, WHITE);
        }
        break;

        case CONFIGURATIONS:
        {
            // Background
            DrawTexturePro(TEXTURE_CONFIGURATIONS_BACKGROUND_IMG, TEXTURE_CONFIGURATIONS_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);

            DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND], origin, 0.0f, WHITE);
            if (mutedSounds[CONFIGURATIONS_SOUND_BACKGROUND])
            {
                DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG, TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND], origin, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND], origin, 0.0f, WHITE);
            }
            DrawTexturePro(TEXTURE_BACKGROUND_BTN_IMG, TEXTURE_BACKGROUND_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_BACKGROUND], origin, 0.0f, WHITE);

            if (mutedSounds[CONFIGURATIONS_SOUND_ZOMBIES])
            {
                DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG, TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_ZOMBIES], origin, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_ZOMBIES], origin, 0.0f, WHITE);
            }
            DrawTexturePro(TEXTURE_ZOMBIES_BTN_IMG, TEXTURE_ZOMBIES_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_ZOMBIES], origin, 0.0f, WHITE);

            if (mutedSounds[CONFIGURATIONS_SOUND_PLANTS])
            {
                DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG, TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_PLANTS], origin, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_PLANTS], origin, 0.0f, WHITE);
            }
            DrawTexturePro(TEXTURE_PLANTS_BTN_IMG, TEXTURE_PLANTS_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_PLANTS], origin, 0.0f, WHITE);

            if (mutedSounds[CONFIGURATIONS_SOUND_PEASHOTS])
            {
                DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG, TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_PEASHOTS], origin, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_PEASHOTS], origin, 0.0f, WHITE);
            }
            DrawTexturePro(TEXTURE_PEASHOTS_BTN_IMG, TEXTURE_PEASHOTS_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_PEASHOTS], origin, 0.0f, WHITE);

            if (mutedSounds[CONFIGURATIONS_SOUND_SUNS])
            {
                DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG, TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_SUNS], origin, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_SUNS], origin, 0.0f, WHITE);
            }
            DrawTexturePro(TEXTURE_SUNS_BTN_IMG, TEXTURE_SUNS_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_SUNS], origin, 0.0f, WHITE);

            if (mutedSounds[CONFIGURATIONS_SOUND_SOUNDEFFECTS])
            {
                DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG, TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_SOUNDEFFECTS], origin, 0.0f, WHITE);
            }
            else
            {
                DrawTexturePro(TEXTURE_MUTE_BTN_IMG, TEXTURE_MUTE_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_SOUND_SOUNDEFFECTS], origin, 0.0f, WHITE);
            }
            DrawTexturePro(TEXTURE_SOUNDEFFECTS_BTN_IMG, TEXTURE_SOUNDEFFECTS_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_SOUNDEFFECTS], origin, 0.0f, WHITE);

            DrawTexturePro(TEXTURE_GOBACK_BTN_IMG, TEXTURE_GOBACK_BTN_IMG_SOURCE_REC, configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK], origin, 0.0f, WHITE);
        }
        break;

        case EXIT:
        {
            DrawTexturePro(TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG, TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_GOBACK_BTN_IMG, TEXTURE_GOBACK_BTN_IMG_SOURCE_REC, GOBACK_BTN_CPY, origin, 0.0f, WHITE);
            DrawTexturePro(TEXTURE_EXIT_BTN_IMG, TEXTURE_EXIT_BTN_IMG_SOURCE_REC, EXIT_BTN_CPY, origin, 0.0f, WHITE);
        }
        break;
        default:
            break;
        }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadAllTextures();
    UnloadAllSounds();
    recordPlayersToFile(leaderBoardFile, leaderBoardTop5Players);
    fclose(leaderBoardFile);
    fclose(zombiesHordesText);
    CloseAudioDevice();
    CloseWindow();
    //-------------------------------------------------------------------------------------

    return 0;
}