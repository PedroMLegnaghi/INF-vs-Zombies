#pragma once

#include "raylib.h"
#include <stdbool.h>
#include "Enums.h"
#include "Plants.h"
#include "Structs.h"
#include "main.h"
#include "stdlib.h"
#include "string.h"
#include "lawn.h"
#include "sun.h"
#include "zombies.h"

//used to track which plants are deployed in the field(lawn)
extern Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];

//--PEASHOOTER AND PEASHOT
extern PeaShot peaShotsArr [SIZE_OF_PEASHOT_ARR];
extern int indexOfNextPea;
    
// PLANT FUNCTIONS
void UpdateHealthOfPlant(Plant *plant, float damage);
void RemovePlantFromArr(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], int rowOfPlantToBeRemoved, int columnOfPlantToBeRemoved);
void UpdateExistanceTime(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float timeCorrection);
void UpdateReferenceTime(Plant *plant);
void DrawPlants(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]);
void GenerateSunSunflower(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float groundOfTheSuns[SIZE_OF_SUN_ARR], Rectangle arr_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun);
void InitPlantArr(void);
// PEA FUNCTIONS
void addPeaToArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], PeaShot pea, int *indexOfNextPea);
void shootPea(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int *indexOfNextPea, Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]);
void DrawPeaShots(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfNextPea);
void UpdatePeaShotPosition(PeaShot *pea);
void RemovePeaFromArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfPeaToBeRemoved, int *indexOfNextPea);

// COLLISION FUNCTIONS
bool verifyPeaShotColisionWithZombie(PeaShot pea, Zombie zombie);
bool verifyPlantColisionWithZombie(Plant plant, Zombie zombie);

// GAMEPLAY/DECK FUNCTIONS
int updatePlantsAndZombiesGameplay(
    Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
    PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],
    Zombie zombieArr[SIZE_OF_ZOMBIES_ARR],
    bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
    int *indexOfNextPea,
    int *indexOfNextZombie,
    Sound peaImpactWithZombieSound,
    Sound zombieAtePlant
);

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

//ABOUT FUNCTIONS
void DrawAboutScreen(void);