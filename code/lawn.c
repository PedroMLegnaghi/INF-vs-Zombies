#include "raylib.h"
#include "main.h"
#include "lawn.h"
// Constants of lawn display
const int DISTANCE_LAWN_RIGHT_EDGE = (0.12) * SCREEN_WIDTH;
const int DISTANCE_LAWN_LEFT_EDGE = (0.22) * SCREEN_WIDTH;
const int DISTANCE_LAWN_INFERIOR_EDGE = (0.063) * SCREEN_HEIGHT;
const int DISTANCE_LAWN_SUPERIOR_EDGE = (0.1) * SCREEN_HEIGHT;
const int LAWN_WIDTH_VALUE = (SCREEN_WIDTH - DISTANCE_LAWN_RIGHT_EDGE - DISTANCE_LAWN_LEFT_EDGE) / NUMBER_COLUMN_LAWN;
const int LAWN_HEIGHT_VALUE = (SCREEN_HEIGHT - DISTANCE_LAWN_INFERIOR_EDGE - DISTANCE_LAWN_SUPERIOR_EDGE) / NUMBER_ROWS_LAWN;
const int LAWN_X_VALUE = LAWN_WIDTH_VALUE;
const int LAWN_Y_VALUE = LAWN_HEIGHT_VALUE;

// LAWN--------------
// lawns of the game
// used to verify if a plant already exists in a spot
bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN] = {0};
bool lawnRectanglesHover[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];

void InitLawnRectangles()
{
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
    {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
        {
            lawnRectangles[i][j].x = DISTANCE_LAWN_LEFT_EDGE + LAWN_X_VALUE * j;
            lawnRectangles[i][j].y = DISTANCE_LAWN_SUPERIOR_EDGE + LAWN_Y_VALUE * i;
            lawnRectangles[i][j].width = LAWN_WIDTH_VALUE;
            lawnRectangles[i][j].height = LAWN_HEIGHT_VALUE;
        }
    }
}