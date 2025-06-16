#pragma once
#include "stdio.h"
#include "Player.h"
#include "main.h"
#include "zombies.h"
extern FILE *leaderBoardFile;
extern PLAYER leaderBoardTop5Players[5];
extern FILE *zombiesHordesText;
 
int checksIfInTop5(PLAYER player, PLAYER topFivePlayers[5]);
void reorderTop5(PLAYER player, PLAYER topFivePlayers[5]);
int recordPlayersToFile(FILE *file,PLAYER players[5]);
int importPlayersFromFile(FILE *file, PLAYER players [5]);
void importZombiesFromFile(FILE *file, int zombiesHordes[QUANTITY_MAX_HORDES], int *quantityOfHordes);

void drawTopFivePlayers(PLAYER players[5]);