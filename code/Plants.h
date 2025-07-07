#pragma once
#define SIZE_OF_PEASHOT_ARR 1000 // maximum quantity of peashots in screen

#include "raylib.h"
#include <stdbool.h>
#include "sun.h"
#include "main.h"
#include "Enums.h"
#include "Structs.h"
#include "stdlib.h"
#include "string.h"
#include "lawn.h"
#include "zombies.h"
#include "Textures.h"

// used to track which plants are deployed in the field(lawn)
extern Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];

//--PEASHOOTER AND PEASHOT
extern PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR];
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

// ABOUT FUNCTIONS
void DrawAboutScreen(void);