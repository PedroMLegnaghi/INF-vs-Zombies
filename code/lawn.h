#include "raylib.h"
#include "main.h"
#pragma once

#define NUMBER_COLUMN_LAWN 9
#define NUMBER_ROWS_LAWN 5
//Constants of lawn display
extern const int DISTANCE_LAWN_RIGHT_EDGE;
extern const int DISTANCE_LAWN_LEFT_EDGE ;
extern const int DISTANCE_LAWN_INFERIOR_EDGE;
extern const int DISTANCE_LAWN_SUPERIOR_EDGE ;
extern const int LAWN_WIDTH_VALUE;
extern const int LAWN_HEIGHT_VALUE;
extern const int LAWN_X_VALUE;
extern const int LAWN_Y_VALUE;


//LAWN--------------
//lawns of the game
    extern bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
    extern bool lawnRectanglesHover[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
    extern Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
    
    void InitLawnRectangles(void);