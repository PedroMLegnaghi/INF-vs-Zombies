
#include "Buttons.h"
#include "Homepage.h"
const int marginFromTitle=0.3*SCREEN_HEIGHT;
//Btn display
int BTN_WIDTH;
//margin from title from homepage and menu
int BTN_HEIGHT ;
int BTN_CENTERED_X_FOR_TWO_BUTTONS ;
int BTN_ALONE_CENTERED_X ;
int BTN_INITIAL_Y;
int BTN_DYNAMIC_Y;

//--about section
Rectangle BTN_GOBACK;
bool BTN_GOBACK_HOVER ;

//--exit confirmation
Rectangle GOBACK_BTN_CPY;
Rectangle   EXIT_BTN_CPY;
bool GOBACK_BTN_CPY_HOVER;
bool EXIT_BTN_CPY_HOVER;
void InitButtons(void){

//Btn display
 BTN_WIDTH =SCREEN_WIDTH/3.5;
 
//margin from title from homepage and menu
 BTN_HEIGHT =(SCREEN_WIDTH-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY;
 BTN_CENTERED_X_FOR_TWO_BUTTONS = (SCREEN_WIDTH-BTN_WIDTH*2)/2;
 BTN_ALONE_CENTERED_X = (SCREEN_WIDTH-BTN_WIDTH)/2;
 BTN_INITIAL_Y=marginFromTitle;
 BTN_DYNAMIC_Y=BTN_INITIAL_Y;

//--about section
 BTN_GOBACK= (Rectangle) {.x=SCREEN_WIDTH/30,.y=SCREEN_HEIGHT/30,.width=BTN_WIDTH,.height=BTN_HEIGHT};
 BTN_GOBACK_HOVER =0;
 
 //--exit confirmation
         GOBACK_BTN_CPY=BTN_GOBACK;
             GOBACK_BTN_CPY.x = BTN_CENTERED_X_FOR_TWO_BUTTONS;
             GOBACK_BTN_CPY.y =(SCREEN_HEIGHT-BTN_HEIGHT*2)/2;
             GOBACK_BTN_CPY_HOVER =0;

         EXIT_BTN_CPY=BTN_GOBACK;
             EXIT_BTN_CPY.x = BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
             EXIT_BTN_CPY.y =(SCREEN_HEIGHT-BTN_HEIGHT*2)/2;
             EXIT_BTN_CPY_HOVER =0;

}
