#pragma once
#include "raylib.h"
#include "main.h"
#include "Structs.h"
#include "Buttons.h"
#include "Sound.h"

#define HOME_PAGE_OPTIONS_QUANTITY 5 // quantity of options in the Homepage

extern Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY];
extern bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY];
extern GAME_SCREEN homePageOptions[HOME_PAGE_OPTIONS_QUANTITY];

void InitHomePage(void);
void HandleHomePageButtons(void);