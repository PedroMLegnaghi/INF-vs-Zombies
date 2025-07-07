#pragma once
#include "stdio.h"
#include "string.h"
#include "Structs.h"
#include "main.h"
#include "sun.h"
#include "zombies.h"

#define MAX_SIZE_OF_NAME 15
typedef struct
{
    unsigned int pointsOfPlayer;
    char playerName[MAX_SIZE_OF_NAME];
} PLAYER;

int handlePlayerNameInput(void);

extern PLAYER player;
extern char playerName[MAX_SIZE_OF_NAME];
extern unsigned int sizeOfName;