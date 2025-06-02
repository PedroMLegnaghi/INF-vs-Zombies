#include "raylib.h"
#pragma once
#define MAX_SIZE_OF_NAME 15
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
#define GAMING_MENU_OPTIONS_QUANTITY 3 //resume, configurations and exit
#define CONFIGURATIONS_OPTIONS_QUANTITY 13//quantity of options in the configurations menu, 6 options + 6 corresponding rectangles mute/unmute +1 Back to menu option
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define TARGET_FPS 60
#define NUMBER_COLUMN_LAWN 9
#define NUMBER_ROWS_LAWN 5
#define VALUE_OF_EACH_SUN 25
#define SUN_HEIGHT 30
#define SUN_WIDTH 30

//Sizes of arrays
#define SIZE_OF_SUN_ARR 100 //maximum quantity of suns in screen
#define SIZE_OF_DECK 3+1    //maximum size of deck (3 plants + 1 shovel) 
#define SIZE_OF_ZOMBIES_ARR 100 //maximum quantity of zombies in screen
#define SIZE_OF_PEASHOT_ARR 1000 //maximum quantity of peashots in screen

//Constants of lawn display
extern const int DISTANCE_LAWN_RIGHT_EDGE;
extern const int DISTANCE_LAWN_LEFT_EDGE;
extern const int DISTANCE_LAWN_INFERIOR_EDGE;
extern const int DISTANCE_LAWN_SUPERIOR_EDGE;
extern const int LAWN_WIDTH_VALUE;
extern const int LAWN_HEIGHT_VALUE;
extern const int LAWN_X_VALUE;
extern const int LAWN_Y_VALUE;

//Constants of deck display
extern const int DECK_RECTANGLE_X_VALUE;
extern const int DECK_RECTANGLE_Y_VALUE;
extern const int DECK_ELEMENT_WIDTH_VALUE;
extern const int DECK_ELEMENT_HEIGHT_VALUE;

//User's Mouse
extern Vector2 mousePoint;

//ScreenRectangle(used to display textures)
extern const Rectangle SCREEN_RECTANGLE;

