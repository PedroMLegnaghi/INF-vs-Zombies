#include "raylib.h"
#include <stdbool.h>
//refers to the screens of the game
    //LOGO(made by pedro)
    //HOMEPAGE(START,LEADERBOARD,CONFIGURATIONS,ABOUT, EXIT)
    //CONFIGURATIONS
    //LEADERBOARD
    //EXIT(isnt necessarily an screen, but let's consider it for the moment)
    //START(asking name and gender)
        //->GAMEOPTIONS (deck of plants)?
            //GAMEPLAY
typedef enum GameScreen {LOGO = 0, HOMEPAGE, GAMEPLAY, LEADERBOARD, ABOUT, CONFIGURATIONS, EXIT } GameScreen;
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
int main (void){
    //initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight,"raylib [core] example - basic screen manager");
    
    GameScreen currentScreen = LOGO;

       // TODO: Initialize all required variables and load all required data here!

//HOMEPAGE================================================================================================================================================
//========================================================================================================================================================
    Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(gameplay, leaderboard, about, configurations and exit)
    bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    GameScreen homePageOptions[HOME_PAGE_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
        //Filling the homePageOptions
            homePageOptions[0]= GAMEPLAY;
            homePageOptions[1]= LEADERBOARD;
            homePageOptions[2]= ABOUT;
            homePageOptions[3]= CONFIGURATIONS;
            homePageOptions[4]= EXIT;

            //Filling the homePageOptionsRec 
            for (int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
                homePageOptionsRec[i].height = 40;
                homePageOptionsRec[i].width = 240;
                homePageOptionsRec[i].x=(screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30;
                homePageOptionsRec[i].y = 110+60*i;
            }
//========================================================================================================================================================
    
    int framesCounter = 0;          // Useful to count frames
    Vector2 mousePoint = { 0.0f, 0.0f }; //useful to track the user's mouse
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();//tracking, effectively, the user's mouse
        switch (currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!

                framesCounter++;    // Count frames

                // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
                if (framesCounter > 420)
                {
                    currentScreen = HOMEPAGE;
                }
            } break;
            case HOMEPAGE:
            {
                // TODO: Update HOMEPAGE screen variables here!
                //checks if an rectangle(option) is hovered, so that we can highlight that ractangle
                 for (int i = 0; i < HOME_PAGE_OPTIONS_QUANTITY; i++)
        {
            if (CheckCollisionPointRec(mousePoint, homePageOptionsRec[i])) {
                homePageOptionsRecHover[i] = 1;
                if(IsGestureDetected(GESTURE_TAP)){
                    currentScreen = homePageOptions[i];
                }
            }
            else homePageOptionsRecHover[i] = 0;
        }

            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ABOUT;
                }
            } break;
            case ABOUT:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to HOMEPAGE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = HOMEPAGE;
                }
            } break;
             case CONFIGURATIONS:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to HOMEPAGE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = HOMEPAGE;
                }
            } break;
             case EXIT:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to HOMEPAGE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = HOMEPAGE;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

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
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    for(int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(homePageOptionsRec[i], WHITE);
                        //tracking hover over the options
                        if(homePageOptionsRecHover[i]==true){
                            DrawRectangleRec(homePageOptionsRec[i], RED);
                        }
                    }
                    DrawText("INF vs ZOMBIES", (screenWidth-MeasureText("INF vs ZOMBIES", 40))/2, 20, 40, DARKGREEN);
                    DrawText("PLAY", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 120, 20, DARKGREEN);
                    DrawText("LEADERBOARD", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 180, 20, DARKGREEN);
                    DrawText("ABOUT", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 240, 20, DARKGREEN);
                    DrawText("CONFIGURATIONS", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 300, 20, DARKGREEN);
                    DrawText("EXIT", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 360, 20, DARKGREEN);
                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

                } break;
                case ABOUT:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                 case CONFIGURATIONS:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                 case EXIT:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;

}