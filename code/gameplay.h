#pragma once
#include "raylib.h"
#include "Plants.h"
#include "Player.h"
#include "stdbool.h"

extern bool gameHasEnded;  

bool verifyPeaShotColisionWithZombie(PeaShot pea, Zombie zombie);
bool verifyPlantColisionWithZombie(Plant plant, Zombie zombie);

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
void resetGameplay();