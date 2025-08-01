#pragma once
#include "Buttons.h"
#include "main.h"
#include "Enums.h"

#define CONFIGURATIONS_OPTIONS_QUANTITY 13 // quantity of options in the configurations menu, 6 options + 6 corresponding rectangles mute/unmute +1 Back to menu option

extern Rectangle configurationsOptionsRec[CONFIGURATIONS_OPTIONS_QUANTITY]; // initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
extern bool configurationsOptionsRecHover[CONFIGURATIONS_OPTIONS_QUANTITY]; // array that tells if an option is hovered
extern bool mutedSounds[CONFIGURATIONS_OPTIONS_QUANTITY];

extern CONFIGURATIONS_OPTIONS configurationsOptions[CONFIGURATIONS_OPTIONS_QUANTITY]; // array to navigate over the options of the game

void InitConfigurationsMenu(void);