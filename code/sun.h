#pragma once
#include "raylib.h"
#include "stdlib.h"
#include "main.h"
#include "lawn.h"
#include "Sound.h"
#define SIZE_OF_SUN_ARR 100 // maximum quantity of suns in screen
#define VALUE_OF_EACH_SUN 25
#define SUN_HEIGHT 80
#define SUN_WIDTH 80
#define STANDART_SPAWN_RATE_SUN 6.0 // seconds

extern double timeOfLastSun;
extern Rectangle sunArray[SIZE_OF_SUN_ARR];
extern unsigned int sunGamingStorage;
extern float groundOfTheSuns[SIZE_OF_SUN_ARR];
extern int indexOfNextSun;
extern double spawnRateSun;
extern double timeSpawnSunTracking;
extern float timeToIncreaseSpawnRateSunEachHorde;

void AddSunToArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], int rowOfGround, int columnOfGround, float groundOfTheSuns[SIZE_OF_SUN_ARR], int x, int y);
void AddRandomlySunToArr(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void updateSunsPosition(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfSun, float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void RemoveSunOfArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, int indexOfSunRemoved, float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void collectSun(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, float groundOfTheSuns[SIZE_OF_SUN_ARR]);
void addSunToStorage(unsigned int *gamingSunStorage);
void DrawSuns(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfNextSun, Texture2D sunTexture);
