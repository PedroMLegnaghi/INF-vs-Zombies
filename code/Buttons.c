
#include "Buttons.h"
#include "Homepage.h"
const int marginFromTitle = 0.3 * SCREEN_HEIGHT;
// Btn display
int BTN_WIDTH;
// margin from title from homepage and menu
int BTN_HEIGHT;
int BTN_CENTERED_X_FOR_TWO_BUTTONS;
int BTN_ALONE_CENTERED_X;
int BTN_INITIAL_Y;
int BTN_DYNAMIC_Y;

//--about section
Rectangle BTN_GOBACK;
bool BTN_GOBACK_HOVER;

//--exit confirmation
Rectangle GOBACK_BTN_CPY;
Rectangle EXIT_BTN_CPY;
bool GOBACK_BTN_CPY_HOVER;
bool EXIT_BTN_CPY_HOVER;

//--endgame
Rectangle BTN_ENDGAME_GOBACK;
bool BTN_ENDGAME_GOBACK_HOVER;

// InitButtons:
// Initializes the buttons of the game (sets the width and height of the buttons based on the screen size and margin from title)
void InitButtons(void)
{
    // Btn display
    BTN_WIDTH = SCREEN_WIDTH / 4;

    // margin from title from homepage and menu
    BTN_HEIGHT = (SCREEN_WIDTH - marginFromTitle) / 8;
    BTN_CENTERED_X_FOR_TWO_BUTTONS = (SCREEN_WIDTH - BTN_WIDTH * 2) / 2;
    BTN_ALONE_CENTERED_X = (SCREEN_WIDTH - BTN_WIDTH) / 2;
    BTN_INITIAL_Y = marginFromTitle;
    BTN_DYNAMIC_Y = BTN_INITIAL_Y;

    //--about section
    BTN_GOBACK = (Rectangle){.x = SCREEN_WIDTH / 60, .y = SCREEN_HEIGHT / 45, .width = BTN_WIDTH, .height = BTN_HEIGHT};
    BTN_GOBACK_HOVER = 0;

    //--exit confirmation
    GOBACK_BTN_CPY = BTN_GOBACK;
    GOBACK_BTN_CPY.x = SCREEN_WIDTH / 4;
    GOBACK_BTN_CPY.y = (SCREEN_HEIGHT - BTN_HEIGHT * 2) / 1.2;
    GOBACK_BTN_CPY_HOVER = 0;

    EXIT_BTN_CPY = BTN_GOBACK;
    EXIT_BTN_CPY.x = GOBACK_BTN_CPY.x + BTN_WIDTH;
    EXIT_BTN_CPY.y = (SCREEN_HEIGHT - BTN_HEIGHT * 2) / 1.2;
    EXIT_BTN_CPY_HOVER = 0;

    //--endgame
    // return to the homepage button
    BTN_ENDGAME_GOBACK = GOBACK_BTN_CPY;
    BTN_ENDGAME_GOBACK.x = SCREEN_WIDTH - BTN_ENDGAME_GOBACK.width - SCREEN_WIDTH / 30;
    BTN_ENDGAME_GOBACK_HOVER = 0;
}
