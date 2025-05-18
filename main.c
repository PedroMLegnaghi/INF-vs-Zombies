#include "raylib.h"
#include <stdbool.h>
//refers to the screens of the game
    //LOGO(made by pedro)
    //HOMEPAGE(PLAY,LEADERBOARD,CONFIGURATIONS,ABOUT, EXIT)
        //PLAY
            //(asking name and gender)
            //->GAMEOPTIONS (deck of plants)?
        //CONFIGURATIONS
        //LEADERBOARD
        //EXIT(isnt necessarily an screen, but let's consider it for the moment)
typedef enum GameScreen {LOGO = 0, HOMEPAGE, PLAY, LEADERBOARD, ABOUT, CONFIGURATIONS, EXIT //HOMEPAGE OPTIONS
                         ,USER_DATA, DECK_SELECTION, GAMEPLAY} GameScreen;//GAMEPLAY OPTIONS
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
int main (void){
    //initialization
    // TODO: Initialize all required variables and load all required data here!
//--screen
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight,"raylib [core] example - basic screen manager");
    GameScreen currentScreen = LOGO;
    GameScreen previousScreen = HOMEPAGE;
//--------

//--HomePage
Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
GameScreen homePageOptions[HOME_PAGE_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
    //Filling the homePageOptions
    homePageOptions[0]= PLAY;
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
//---------
//--exiting application
    SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works
    bool exitWindow = false;    // Flag to set window to exit
//---------
    int framesCounter = 0;          // Useful to count frames
    Vector2 mousePoint = { 0.0f, 0.0f }; //useful to track the user's mouse
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)

//--PLAY variables
const unsigned int MAX_SIZE_OF_NAME = 14+1;
char playerName[MAX_SIZE_OF_NAME];
    for (int i=0;i<MAX_SIZE_OF_NAME;i++)
        playerName[i]='\0';
    unsigned int sizeOfName = 0;//variable used to track the size of the name of the user
    //LAWN--------------
    //lawns of the game
    const unsigned int numberLawnColums =9;
    const unsigned int numberLawnRows =5;
    bool lawnRectanglesHover[numberLawnRows][numberLawnColums];
    Rectangle lawnRectangles[numberLawnRows][numberLawnColums];
    int initialLawnXValue = (screenWidth-35*2)/numberLawnColums;
	int initialLawnYValue = (screenHeight-(60+40))/numberLawnRows;
	int initialLawnWidthValue = initialLawnXValue;
	int initialLawnHeightValue = initialLawnYValue;
    // for(int i=0;i<1;i++){
    //         for(int j=0;j<numberLawnColums;j++){
    //             lawnRectangles[i][j].x = initialLawnXValue+initialLawnXValue*j;
    //             lawnRectangles[i][j].y = initialLawnYValue+40;
    //             lawnRectangles[i][j].width = initialLawnWidthValue;
    //             lawnRectangles[i][j].height = initialLawnHeightValue;
    //         }
    //     }
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColums;j++){
                lawnRectangles[i][j].x = 30+initialLawnXValue*j;
                lawnRectangles[i][j].y = 80+initialLawnYValue*i;
                lawnRectangles[i][j].width = initialLawnWidthValue;
                lawnRectangles[i][j].height = initialLawnHeightValue;
            }
        }
//---------
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!exitWindow)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();//tracking, effectively, the user's mouse
        
        //if user wants to end applications, redirect him to an exit confirmation
        if(WindowShouldClose()){
            currentScreen=  EXIT;
        }

        switch (currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                
                framesCounter++;    // Count frames

                // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
                if (framesCounter > 5)
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
                    previousScreen=currentScreen;
                    currentScreen = homePageOptions[i];
                }
            }
            else homePageOptionsRecHover[i] = 0;
        }

            } break;
            case PLAY:
            {
                // TODO: Update PLAY screen variables here!
                currentScreen = USER_DATA;//first thing to be done, is to ask for the User data
                
                
               
            } break;
              case USER_DATA:
            {
                // TODO: Update USERDATA screen variables here!-------------------
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
                    currentScreen = GAMEPLAY;  // Ou qualquer próxima tela desejada
                }


               
            } break;
            
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColums;j++){
               if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                    lawnRectanglesHover[i][j] = 1;
                }
                else lawnRectanglesHover[i][j] = 0;
           }
        }

               
            } break;
            case ABOUT:
            {
                // TODO: Update ENDING screen variables here!
                

                // Press enter to return to HOMEPAGE screen
               
            } break;
             case CONFIGURATIONS:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to HOMEPAGE screen
               
            } break;
             case EXIT:
            {
                //Confirming exit                
               
            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            if (IsKeyPressed(KEY_Y)) exitWindow = true;
            else if (IsKeyPressed(KEY_N)) {
                currentScreen=previousScreen;
            };
        
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
                    ClearBackground(RAYWHITE);
                    // TODO: Draw LOGO screen here!
                    DrawText("Pedrokas's gaming enterprise® \n\t\t\t\t\t\t\t\tpresents... ", 70, 150, 40, DARKGREEN);

                } break;
                case HOMEPAGE:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw HOMEPAGE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    for(int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(homePageOptionsRec[i], WHITE);
                         DrawRectangleLines(homePageOptionsRec[i].x, homePageOptionsRec[i].y, homePageOptionsRec[i].width, homePageOptionsRec[i].height, BLACK);
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
                case USER_DATA:
                {
                    // TODO: Draw USERDATA screen here!
                  
                    ClearBackground(RAYWHITE);
                    Rectangle textBox = { screenWidth/2 - 150, 180,  MeasureText("A",40)*8+30, 50 };
                    if(sizeOfName>8){
                        textBox.width = sizeOfName*MeasureText("A",40)+10;
                    }
                    
                    DrawText("Tell me your name!", 240, 140, 20, GRAY);

                    DrawRectangleRec(textBox, LIGHTGRAY);
                    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);

                    DrawText(playerName, textBox.x + 5, textBox.y + 8, 40, MAROON);

                    DrawText(TextFormat("Press enter to confirm: %i/%i", sizeOfName, MAX_SIZE_OF_NAME-1), 315, 250, 20, DARKGRAY);

                    if (sizeOfName < MAX_SIZE_OF_NAME) {
                        // Draw blinking underscore char
                     DrawText("_",( textBox.x + 8 + MeasureText(playerName, 40)),(textBox.y + 12), 40, MAROON);
                    } 
                  

                } break;
                 case DECK_SELECTION:
                {
                    // ClearBackground(RAYWHITE);
                    // // TODO: Draw ENDING screen here!
                    // DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
                    // DrawText(TextFormat("Nice to meet you, %s :)", playerName), 20, 20, 40, DARKBLUE);
                    // DrawText("Select you deck", 120, 220, 20, DARKBLUE);
                    currentScreen = GAMEPLAY;

                } break;
                case GAMEPLAY:
                {
                    for(int i=0;i<numberLawnRows;i++){
                        for(int j=0;j<numberLawnColums;j++){
                            //if "i" is odd, if "j" is odd, darkgreen, else, lightgreen
                            if(i&1){
                                if(j&1){
                                    DrawRectangleRec( lawnRectangles[i][j], DARKGREEN);
                                }else{
                                    DrawRectangleRec( lawnRectangles[i][j], GREEN);
                                }
                            }else{
                                 if(j&1){
                                     DrawRectangleRec( lawnRectangles[i][j], GREEN);
                                    }else{
                                    DrawRectangleRec( lawnRectangles[i][j], DARKGREEN);
                                }
                            }
                            //tracking hover over the options
                            if(lawnRectanglesHover[i][j]==true){
                                DrawRectangleLines( lawnRectangles[i][j].x,  lawnRectangles[i][j].y,  lawnRectangles[i][j].width,  lawnRectangles[i][j].height, BLACK);
                            }
                        }
                    }
                }break;
                case ABOUT:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                 case CONFIGURATIONS:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                 case EXIT:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw EXIT screen here!
                DrawRectangle(0, 100, screenWidth, 200, BLACK);
                DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);
            
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