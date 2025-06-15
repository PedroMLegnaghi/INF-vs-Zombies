#pragma once
#include "stdio.h"
#include "Player.h"
#include "main.h"
extern FILE *leaderBoardFile;
extern PLAYER leaderBoardTop5Players[5];

int checksIfInTop5(PLAYER player, PLAYER topFivePlayers[5]);
void reorderTop5(PLAYER player, PLAYER topFivePlayers[5]);
int recordPlayersToFile(FILE *file,PLAYER players[5]);
int importPlayersFromFile(FILE *file, PLAYER players [5]);

void drawTopFivePlayers(PLAYER players[5]);