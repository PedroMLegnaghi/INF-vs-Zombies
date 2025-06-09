#include "configMenu.h"
#include "Buttons.h"
#include "main.h"
#include "Enums.h"

    Rectangle configurationsOptionsRec[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool configurationsOptionsRecHover[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered
    bool mutedSounds[CONFIGURATIONS_OPTIONS_QUANTITY]={0}; 
    // bool configurationsOptionsSoundHover[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//array that tells if an option outputed its sound
    
    CONFIGURATIONS_OPTIONS configurationsOptions[CONFIGURATIONS_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
    
void InitConfigurationsMenu(void){

    //Filling the configurationsOptions
    //backgroundMusic ->mute/unmute
    //zombies ->mute/unmute
    //plants ->mute/unmute
    //peashots ->mute/unmute
    //sun ->mute/unmute
    //soundEffects ->mute/unmute
    
    configurationsOptions[CONFIGURATIONS_SOUND_BACKGROUND]= CONFIGURATIONS_SOUND_BACKGROUND;
    configurationsOptions[CONFIGURATIONS_BTN_BACKGROUND]= CONFIGURATIONS_BTN_BACKGROUND;
    configurationsOptions[CONFIGURATIONS_SOUND_ZOMBIES]= CONFIGURATIONS_SOUND_ZOMBIES;
    configurationsOptions[CONFIGURATIONS_BTN_ZOMBIES]= CONFIGURATIONS_BTN_ZOMBIES;
    configurationsOptions[CONFIGURATIONS_SOUND_PLANTS]= CONFIGURATIONS_SOUND_PLANTS;
    configurationsOptions[CONFIGURATIONS_BTN_PLANTS]= CONFIGURATIONS_BTN_PLANTS;
    configurationsOptions[CONFIGURATIONS_SOUND_PEASHOTS]= CONFIGURATIONS_SOUND_PEASHOTS;
    configurationsOptions[CONFIGURATIONS_BTN_PEASHOTS]= CONFIGURATIONS_BTN_PEASHOTS;
    configurationsOptions[CONFIGURATIONS_SOUND_SUNS]= CONFIGURATIONS_SOUND_SUNS;
    configurationsOptions[CONFIGURATIONS_BTN_SUNS]= CONFIGURATIONS_BTN_SUNS;
    configurationsOptions[CONFIGURATIONS_SOUND_SOUNDEFFECTS]= CONFIGURATIONS_SOUND_SOUNDEFFECTS;
    configurationsOptions[CONFIGURATIONS_BTN_SOUNDEFFECTS]= CONFIGURATIONS_BTN_SOUNDEFFECTS;
    configurationsOptions[CONFIGURATIONS_BTN_GOBACK]= CONFIGURATIONS_BTN_GOBACK;

    //Filling the configurationsOptionsRec
    BTN_DYNAMIC_Y=BTN_INITIAL_Y;
        //If index is odd, option goes to the right, if index is even, option to the left
        for (int i=0;i<CONFIGURATIONS_OPTIONS_QUANTITY;i++){
            //if its even
            if(!(i&1)){
                configurationsOptionsRec[i].height = BTN_HEIGHT;
                configurationsOptionsRec[i].width = BTN_WIDTH;
                configurationsOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS;
                configurationsOptionsRec[i].y = BTN_DYNAMIC_Y;
            }else{
                //if its odd
                configurationsOptionsRec[i].height = BTN_HEIGHT;
                configurationsOptionsRec[i].width = BTN_WIDTH;
                configurationsOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
                configurationsOptionsRec[i].y =BTN_DYNAMIC_Y;
                BTN_DYNAMIC_Y+=BTN_HEIGHT;
                // (((SCREEN_HEIGHT-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY))*i
            }
        }
        configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK]=BTN_GOBACK;
        configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK].x=0;
    }