#include "raylib.h"
#include "main.h"
#pragma once
extern const int marginFromTitle;
//Btn display
extern int BTN_WIDTH;
//margin from title from homepage and menu
extern int BTN_HEIGHT ;
extern int BTN_CENTERED_X_FOR_TWO_BUTTONS ;
extern int BTN_ALONE_CENTERED_X ;
extern int BTN_INITIAL_Y;
extern int BTN_DYNAMIC_Y;

//--about section
extern Rectangle BTN_GOBACK;
extern bool BTN_GOBACK_HOVER ;

//--exit confirmation
extern Rectangle GOBACK_BTN_CPY;
extern bool GOBACK_BTN_CPY_HOVER;
extern Rectangle EXIT_BTN_CPY;
extern bool EXIT_BTN_CPY_HOVER;

void InitButtons(void);