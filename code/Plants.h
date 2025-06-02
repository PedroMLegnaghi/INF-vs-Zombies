#pragma once

#include "raylib.h"
#include <stdbool.h>
#include "Enums.h"
#include "main.h"
#include "Structs.h"

// SUN FUNCTIONS
void AddSunToArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], int rowOfGround, int columnOfGround, float groundOfTheSuns[SIZE_OF_SUN_ARR], int x, int y);
void AddRandomlySunToArr(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void updateSunsPosition(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfSun, float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void RemoveSunOfArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, int indexOfSunRemoved, float groundOfTheSuns[SIZE_OF_SUN_ARR]);
int collectSun(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void addSunToStorage(unsigned int *gamingSunStorage);
void DrawSuns(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfNextSun, Texture2D sunTexture);

// PLANT FUNCTIONS
void UpdateHealthOfPlant(Plant *plant, float damage);
void RemovePlantFromArr(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], int rowOfPlantToBeRemoved, int columnOfPlantToBeRemoved);
void UpdateExistanceTime(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float timeCorrection);
void UpdateReferenceTime(Plant *plant);
void DrawPlants(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]);
void GenerateSunSunflower(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float groundOfTheSuns[SIZE_OF_SUN_ARR], Rectangle arr_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun);

// PEA FUNCTIONS
void addPeaToArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], PeaShot pea, int *indexOfNextPea);
void shootPea(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int *indexOfNextPea, Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]);
void DrawPeaShots(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfNextPea);
void UpdatePeaShotPosition(PeaShot *pea);
void RemovePeaFromArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfPeaToBeRemoved, int *indexOfNextPea);

// ZOMBIE FUNCTIONS
void DrawZombie(Zombie zombie);
void DrawZombieArr(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int indexOfNextZombie);
void AddZombieToZombiesArr(Zombie zombie, Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie);
void AddZombieToZombiesArrRandomly(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], Zombie zombieToBeGenerated, int *indexOfNextZombie, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]);
void UpdateZombiePosition(Zombie *zombie);
void RemoveZombie(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie, int indexOfZombieToBeRemoved);
void UpdateZombieHealth(Zombie *zombie, int damage);

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