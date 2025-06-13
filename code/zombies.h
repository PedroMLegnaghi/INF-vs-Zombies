
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
#define SIZE_OF_ZOMBIES_ARR 100 //maximum quantity of zombies in screen

//--ZOMBIE
extern Zombie zombieArr[SIZE_OF_ZOMBIES_ARR];
extern int indexOfNextZombie;
extern double spawnRateZombie;
extern bool firstZombieSpawn;
extern double timeOfLastZombie ;  //saves the actualTime
extern double timeForFirstSpawnZombie;
//used to spawn zombies appropriately
extern double timeSpawnZombieTracking;

void InitZombiesArr(Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]);

// ZOMBIE FUNCTIONS
void DrawZombie(Zombie zombie);
void DrawZombieArr(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int indexOfNextZombie);
void AddZombieToZombiesArr(Zombie zombie, Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie);
void AddZombieToZombiesArrRandomly(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], Zombie zombieToBeGenerated, int *indexOfNextZombie, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]);
void UpdateZombiePosition(Zombie *zombie);
void RemoveZombie(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie, int indexOfZombieToBeRemoved);
void UpdateZombieHealth(Zombie *zombie, int damage);