#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef enum GameScreen {LOGO = 0, HOMEPAGE, PLAY, LEADERBOARD, ABOUT, CONFIGURATIONS, EXIT //HOMEPAGE OPTIONS
                         ,USER_DATA, DECK_SELECTION, GAMEPLAY,MENU, RESUME} GameScreen;//GAMEPLAY OPTIONS
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
#define GAMING_MENU_OPTIONS_QUANTITY 3 //resume, configurations and exit
#define MAX_SUN_IN_SCREEN 100 //maximum quantity of suns in screen
#define screenWidth 800
#define screenHeight 450
#define targetFPS 60
#define numberLawnColumns 9
#define numberLawnRows 5
#define VALUE_OF_EACH_SUN 25
   

//Functions-------------------------------------
//SUN FUNCTIONS---
    //AddSunToArray:Rectangle[], int, Rectangle [] [], float->img(void)
    //Given an array of suns, the index of the next sun , the array of lawns of the game and the groundOfTheSuns array, add that sun to the array with x and y position of a random lawn of the game 
    void AddSunToArray(Rectangle array_of_suns[MAX_SUN_IN_SCREEN],  int indexOfNextSun, Rectangle lawn_array[numberLawnRows][numberLawnColumns],float groundOfTheSuns[MAX_SUN_IN_SCREEN]) {
        if (indexOfNextSun >= MAX_SUN_IN_SCREEN) return;
        //chooses a number beetwen 0 and numberLawnRows
        int row = rand() % (numberLawnRows);
        //chooses a number beetwen 0 and numberLawnColumns
        int colum = rand() % (numberLawnColumns);
        //we set this "y" position as the final "y" position of the sun, so it starts at 0 and goes until it hit it's "y" position (it's ground)
        groundOfTheSuns[indexOfNextSun] =  lawn_array[row][colum].y;
        array_of_suns[indexOfNextSun].x = lawn_array[row][colum].x;
        //starts at "y"=0 and goes until it hits its ground (groundArray)
        array_of_suns[indexOfNextSun].y =  0;
        array_of_suns[indexOfNextSun].width = 20;
        array_of_suns[indexOfNextSun].height = 20;
        
    }

    //updateSunsPosition:Rectangle[] int Rectangle []->void
    //given the array of the suns of the game ,groundOfTheSuns and the index of the next sun, update their position, until they hit their ground
    void updateSunsPosition(Rectangle array_of_suns[MAX_SUN_IN_SCREEN], int indexOfSun,float groundOfTheSuns[MAX_SUN_IN_SCREEN]){
        for(int i=0;i<indexOfSun;i++){
            //if the sun hasn't hit it's ground, (we use "lesser than" here to avoid rounding problems)
            if((array_of_suns[i].y<groundOfTheSuns[i]))
            //update its position
            array_of_suns[i].y+=0.5;
            if (array_of_suns[i].y > groundOfTheSuns[i]) {
                array_of_suns[i].y = groundOfTheSuns[i]; // evita ultrapassar o chão
            }
        }
    };

    //RemoveSunToArray:Rectangle[], int, float []->img(void)
    //Given an array of suns ,groundOfTheSuns and the index of the sun, remove the last sun of the arrayofsuns and remove its ground and update the array
    void RemoveSunOfArray(Rectangle array_of_suns[MAX_SUN_IN_SCREEN], int *indexOfNextSun, int index, float groundOfTheSuns[MAX_SUN_IN_SCREEN]) {
        if (index < 0 || index >= *indexOfNextSun) return;

        for (int i = index; i < (*indexOfNextSun) - 1; i++) {
            array_of_suns[i] = array_of_suns[i + 1];
            groundOfTheSuns[i] = groundOfTheSuns[i + 1];
        }
        (*indexOfNextSun)--;

        // Invalida o último (agora vazio)
        array_of_suns[*indexOfNextSun].x = -1;
        array_of_suns[*indexOfNextSun].y = -1;
        groundOfTheSuns[*indexOfNextSun] = 0;
    }

    //collectSun:Rectangle [], Vector2, int, float []->int
    //Given an array of suns, the mousePointer,the quantityofSun of the player, the index of the next sun and the array of the grounds of the suns, returns wheter one sun was collected(1) or not(0) and collects it
    int collectSun(Rectangle array_of_suns[MAX_SUN_IN_SCREEN],int *indexOfNextSun, Vector2 mousePoint, float groundOfTheSuns[MAX_SUN_IN_SCREEN])
    {
        for (int i = 0; i < *indexOfNextSun; i++) {
            if (CheckCollisionPointRec(mousePoint, array_of_suns[i])) {
                if (IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    RemoveSunOfArray(array_of_suns, indexOfNextSun, i, groundOfTheSuns);
                    return 1;//Sun was collected
                    break; // removes only one sun per click
                }
            }
        }
        return 0;//sun was not collected
    }
    //addSunToStorage:unsigned int -> void
    //given the gamingSunStorage, add the value of one sun to the storage
    void addSunToStorage(unsigned int *gamingSunStorage){
        *gamingSunStorage+=VALUE_OF_EACH_SUN;
    }

    //DrawSuns:Rectangle[], int->img(void)
    //Given the array of suns of the game and the index of the sun, draw the suns
    void DrawSuns(Rectangle array_of_suns[MAX_SUN_IN_SCREEN], int indexOfNextSun){
        for(int i = 0; i < indexOfNextSun; i++){
            if (array_of_suns[i].x != -1 && array_of_suns[i].y != -1) {
                DrawRectangle(array_of_suns[i].x, array_of_suns[i].y,
                            array_of_suns[i].width, array_of_suns[i].height, YELLOW);
            }
        }
    }
//----------------------------------
//GAMING DECK----------------
    //DrawGamingDeck:Plant [], int ->void
    //given the deck of plants and the quantity of suns, draw the interface
    void DrawGamingDeck(unsigned int quantityOfSun){
        DrawRectangle(10,10,40,40,MAROON);
        DrawRectangleLines(10,10,40,40,BLACK);
        DrawText(TextFormat("%d", quantityOfSun),20,20,20,YELLOW);
    }
//--------------------------------
//-------------/------------------------------------


int main (void){
    //initialization
    // TODO: Initialize all required variables and load all required data here!
//--screen
   srand(time(NULL));
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
//--Gaming Menu
Rectangle gamingMenuOptionsRec[GAMING_MENU_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
bool gamingMenuOptionsRecHover[GAMING_MENU_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
GameScreen gamingMenuOptions[GAMING_MENU_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
    //Filling the homePageOptions
    gamingMenuOptions[0]= RESUME;
    gamingMenuOptions[1]= CONFIGURATIONS;
    gamingMenuOptions[2]= EXIT;

    //Filling the gamingMenuOptionsRec 
    for (int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
        gamingMenuOptionsRec[i].height = 40;
        gamingMenuOptionsRec[i].width = 240;
        gamingMenuOptionsRec[i].x=(screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30;
        gamingMenuOptionsRec[i].y = 110+60*i;
    }
    //--exiting application
    SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works
    bool exitWindow = false;    // Flag to set window to exit
    //---------
    double startTime = GetTime();  //saves the actualTime
    int framesCounter = 0;          // Useful to count frames
    Vector2 mousePoint = { 0.0f, 0.0f }; //useful to track the user's mouse
    SetTargetFPS(targetFPS);               // Set desired framerate (frames-per-second)

//--PLAY variables
const unsigned int MAX_SIZE_OF_NAME = 14+1;
char playerName[MAX_SIZE_OF_NAME];
    for (int i=0;i<MAX_SIZE_OF_NAME;i++)
        playerName[i]='\0';
    unsigned int sizeOfName = 0;//variable used to track the size of the name of the user
    //LAWN--------------
    //lawns of the game
 
    bool lawnRectanglesHover[numberLawnRows][numberLawnColumns];
    Rectangle lawnRectangles[numberLawnRows][numberLawnColumns];
    int initialLawnXValue = (screenWidth-35*2)/numberLawnColumns;
	int initialLawnYValue = (screenHeight-(60+40))/numberLawnRows;
	int initialLawnWidthValue = initialLawnXValue;
	int initialLawnHeightValue = initialLawnYValue;
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColumns;j++){
                lawnRectangles[i][j].x = 30+initialLawnXValue*j;
                lawnRectangles[i][j].y = 80+initialLawnYValue*i;
                lawnRectangles[i][j].width = initialLawnWidthValue;
                lawnRectangles[i][j].height = initialLawnHeightValue;
            }
        }

    //SUN
    //array to track the suns, if the x and y coordinates are "-1", then we consider it an empty sun
    Rectangle sunArray[MAX_SUN_IN_SCREEN]={0};
    //array to track the quantity of sun that the player has
    unsigned int sunGamingStorage=0;
    //array to track when a sun hits the ground
    float groundOfTheSuns[MAX_SUN_IN_SCREEN]={0};
    //indexToTrack the end of the array
    int indexOfNextSun = 0;
    //time of spawn of suns = 15s
    double spawnRateSun = 5.0;   
    //initializing the size of all suns
    for (int i=0;i<MAX_SUN_IN_SCREEN;i++){
        sunArray[i].height = 20;
        sunArray[i].width = 20;
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
                previousScreen=currentScreen;
                updateSunsPosition(sunArray,indexOfNextSun,groundOfTheSuns);
                for(int i=0;i<numberLawnRows;i++){
                    for(int j=0;j<numberLawnColumns;j++){
                        if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                            lawnRectanglesHover[i][j] = 1;
                        }   
                        else lawnRectanglesHover[i][j] = 0;
                    }
                }
                 double timeSpawnSunTracking =GetTime();
                 //spawn of the suns
                 if(timeSpawnSunTracking-startTime>spawnRateSun){
                    AddSunToArray(sunArray, indexOfNextSun,lawnRectangles,groundOfTheSuns);
                    indexOfNextSun++;
                    startTime=GetTime();
                 }
                 if(collectSun(sunArray,&indexOfNextSun,mousePoint,groundOfTheSuns)){
                    addSunToStorage(&sunGamingStorage);
                 }

                if(IsKeyPressed(KEY_ESCAPE)){
                    currentScreen = MENU;
                }

               
            } break;
            case MENU:{
                    // TODO: Draw HOMEPAGE screen here!
                for (int i = 0; i < GAMING_MENU_OPTIONS_QUANTITY; i++)
                {
                    if (CheckCollisionPointRec(mousePoint, gamingMenuOptionsRec[i])) {
                        gamingMenuOptionsRecHover[i] = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            previousScreen=currentScreen;
                            currentScreen = gamingMenuOptions[i];
                        }
                    }
                    else gamingMenuOptionsRecHover[i] = 0;
                }
            }break;
            case RESUME:{
                currentScreen = GAMEPLAY;
            }
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
                        for(int j=0;j<numberLawnColumns;j++){
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
                    DrawSuns(sunArray,indexOfNextSun);
                    DrawGamingDeck(sunGamingStorage);
                }break;
                case MENU:{
                    for(int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(gamingMenuOptionsRec[i], WHITE);
                         DrawRectangleLines(gamingMenuOptionsRec[i].x, gamingMenuOptionsRec[i].y, gamingMenuOptionsRec[i].width, gamingMenuOptionsRec[i].height, BLACK);
                         //tracking hover over the options
                         if(gamingMenuOptionsRecHover[i]==true){
                             DrawRectangleRec(gamingMenuOptionsRec[i], RED);
                            }
                        }
                        DrawText("INF vs ZOMBIES", (screenWidth-MeasureText("INF vs ZOMBIES", 40))/2, 20, 40, DARKGREEN);
                        DrawText("RESUME", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 120, 20, DARKGREEN);
                        DrawText("CONFIGURATIONS", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 180, 20, DARKGREEN);
                        DrawText("EXIT", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 240, 20, DARKGREEN);
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