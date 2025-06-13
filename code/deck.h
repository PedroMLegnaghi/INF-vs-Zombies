#pragma once
#include "Structs.h"
#include "main.h"
#include "Plants.h"

#define SIZE_OF_DECK 3+1    //maximum size of deck (3 plants + 1 shovel) 

//Constants of deck display
extern const int DECK_RECTANGLE_X_VALUE;
extern const int DECK_RECTANGLE_Y_VALUE;
extern const int DECK_ELEMENT_WIDTH_VALUE;
extern const int DECK_ELEMENT_HEIGHT_VALUE;
extern Plant DeckOfPlants [SIZE_OF_DECK];

//used to track which card is selected. If card is all nulled, then there's no card selected
extern Plant cardSelected;

void InitDeckOfPlants(void);

void DrawMoldureOfSelectedCard(Plant cardSelected);
void DrawGamingDeck(
    Plant DeckOfPlants[SIZE_OF_DECK],
    unsigned int quantityOfSun,
    Plant *cardSelected
);
void RemoveSelectedCard(Plant *cardSelected);
void PutPlantToField(
    Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
    Plant *cardSelected,
    unsigned int *sunStorage,
    bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
    Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
    Sound soundOfPlantingPlant,
    Sound shovelSound
);


