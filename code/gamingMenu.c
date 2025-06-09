#include "gamingMenu.h"
#include "raylib.h"
#include "Enums.h"
#include "main.h"
#include "Buttons.h"

    Rectangle gamingMenuOptionsRec[GAMING_MENU_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool gamingMenuOptionsRecHover[GAMING_MENU_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    GAME_SCREEN gamingMenuOptions[GAMING_MENU_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game

void InitGamingMenu(void){
        //Filling the homePageOptions
        gamingMenuOptions[0]= RESUME;
        gamingMenuOptions[1]= CONFIGURATIONS;
        gamingMenuOptions[2]= EXIT;
        
        //Filling the gamingMenuOptionsRec 
        for (int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
            gamingMenuOptionsRec[i].height = BTN_HEIGHT;
            gamingMenuOptionsRec[i].width = BTN_WIDTH;
            
gamingMenuOptionsRec[i].x=BTN_ALONE_CENTERED_X;

gamingMenuOptionsRec[i].y= marginFromTitle+((SCREEN_HEIGHT-marginFromTitle)/GAMING_MENU_OPTIONS_QUANTITY)*i;
}

}