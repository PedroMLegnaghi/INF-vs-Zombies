#include "raylib.h"
#include "main.h"
#include "Structs.h"
#include "Buttons.h"
Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    // bool homePageOptionsSoundHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option outputed its sound
    GAME_SCREEN homePageOptions[HOME_PAGE_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
       
    

void InitHomePage(void){
        //Filling the homePageOptions
        homePageOptions[0]= PLAY;
        homePageOptions[1]= LEADERBOARD;
        homePageOptions[2]= ABOUT;
        homePageOptions[3]= CONFIGURATIONS;
        homePageOptions[4]= EXIT;

        
        //Filling the homePageOptionsRec 

        //If index is odd, option goes to the right, if index is even, option to the left
        for (int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
            //if its even
            if(!(i&1)){
                homePageOptionsRec[i].height = BTN_HEIGHT;
                homePageOptionsRec[i].width = BTN_WIDTH;
                homePageOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS;
                homePageOptionsRec[i].y = BTN_DYNAMIC_Y;
            }else{
                //if its odd
                homePageOptionsRec[i].height = BTN_HEIGHT;
                homePageOptionsRec[i].width = BTN_WIDTH;
                homePageOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
                homePageOptionsRec[i].y =BTN_DYNAMIC_Y;
                BTN_DYNAMIC_Y+=BTN_HEIGHT;
                // (((SCREEN_HEIGHT-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY))*i
            }
        }
        //if the home_page_option_quantity is an odd number, then the last option will be displayed alone, so,
        //we need to centralize it
        if(HOME_PAGE_OPTIONS_QUANTITY&1){
            homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY-1].x=BTN_ALONE_CENTERED_X;
        }
    }

        
