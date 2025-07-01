#pragma once
#include "Structs.h"
#include "main.h"
#include "Plants.h"
#include "Textures.h"
#include "lawn.h"
#include "string.h"
#define SIZE_OF_DECK 4 + 1 // maximum size of deck (4 plants + 1 shovel)

// Constants of deck display
extern int DECK_RECTANGLE_X_VALUE;
extern int DECK_RECTANGLE_Y_VALUE;
extern int DECK_ELEMENT_WIDTH_VALUE;
extern int DECK_ELEMENT_HEIGHT_VALUE;
extern Rectangle sunDisplayInGamingBarRectangle;

extern Gaming_Deck DeckOfPlants[SIZE_OF_DECK];

// used to track which card is selected. If card is all nulled, then there's no card selected
extern Gaming_Deck *cardSelected;

void InitDeckOfPlants(void);

void DrawMoldureOfSelectedCard();
void DrawGamingDeck();
void RemoveSelectedCard();
void PutPlantToField();
