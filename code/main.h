#include "raylib.h"
#pragma once
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
#define GAMING_MENU_OPTIONS_QUANTITY 3 //resume, configurations and exit
#define CONFIGURATIONS_OPTIONS_QUANTITY 13//quantity of options in the configurations menu, 6 options + 6 corresponding rectangles mute/unmute +1 Back to menu option
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define TARGET_FPS 60
#define NUMBER_COLUMN_LAWN 9
#define NUMBER_ROWS_LAWN 5


//Sizes of arrays
#define SIZE_OF_DECK 3+1    //maximum size of deck (3 plants + 1 shovel) 
#define SIZE_OF_PEASHOT_ARR 1000 //maximum quantity of peashots in screen


//Constants of deck display
extern const int DECK_RECTANGLE_X_VALUE;
extern const int DECK_RECTANGLE_Y_VALUE;
extern const int DECK_ELEMENT_WIDTH_VALUE;
extern const int DECK_ELEMENT_HEIGHT_VALUE;

//User's Mouse
extern Vector2 mousePoint;

//ScreenRectangle(used to display textures)
extern const Rectangle SCREEN_RECTANGLE;

