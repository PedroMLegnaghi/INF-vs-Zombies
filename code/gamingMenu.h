#pragma once
#include "raylib.h"
#include "Enums.h"
#include "main.h"
#include "Buttons.h"

#define GAMING_MENU_OPTIONS_QUANTITY 3 //resume, configurations and exit

extern Rectangle gamingMenuOptionsRec[GAMING_MENU_OPTIONS_QUANTITY];//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    extern bool gamingMenuOptionsRecHover[GAMING_MENU_OPTIONS_QUANTITY];//array that tells if an option is hovered 
    extern GAME_SCREEN gamingMenuOptions[GAMING_MENU_OPTIONS_QUANTITY] ;//array to navigate over the options of the game

void InitGamingMenu(void);