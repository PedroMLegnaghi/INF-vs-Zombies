#include "raylib.h"
//refers to the screens of the game
    //LOGO(made by pedro)
    //HOMEPAGE(START,LEADERBOARD,CONFIGURATIONS,EXIT)
    //CONFIGURATIONS
    //LEADERBOARD
    //EXIT(isnt necessarily an screen, but let's consider it for the moment)
    //START(asking name and gender)
        //->GAMEOPTIONS (deck of plants)?
            //GAMEPLAY
typedef enum GameScreen {LOGO = 0, HOMEPAGE, GAMEPLAY, ENDING } GameScreen;

int main (void){
    //initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight,"raylib [core] example - basic screen manager");
    
    GameScreen currentScreen = LOGO;

       // TODO: Initialize all required variables and load all required data here!

    int framesCounter = 0;          // Useful to count frames

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
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

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
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
                    DrawRectangle((screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30, 110, 240, 40, WHITE);
                    DrawRectangle((screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30, 170, 240, 40, WHITE);
                    DrawRectangle((screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30, 230, 240, 40, WHITE);
                    DrawRectangle((screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30, 290, 240, 40, WHITE);
                    DrawRectangle((screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30, 350, 240, 40, WHITE);
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
                case ENDING:
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