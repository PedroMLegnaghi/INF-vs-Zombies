#include "raylib.h"
#pragma once

    //btn
    extern Sound SOUND_BTN_CLICK ;
    extern Sound SOUND_BTN_HOVER ;

    //gamestages--
    extern Sound SOUND_LOST_MUSIC ;
    extern Sound SOUND_PAUSE ;
    extern Sound SOUND_ZOMBIES_COMING ;
    extern Sound SOUND_WIN ;

    //plants
    extern Sound SOUND_PEASHOT_IMPACT ;
    extern Sound SOUND_PLANTING_PLANT ;
    extern Sound SOUND_COLLECTING_SUN ;
    extern Sound SOUND_SHOVEL  ;

    //soundtracks
    extern Sound SOUND_HOMEPAGE_MENU ;

    extern Sound SOUND_GAMEPLAY ;

    //zombies
    extern Sound SOUND_ZOMBIE_SPAWN ;
    extern Sound SOUND_ZOMBIE_EAT_PLANT ;
    void LoadAllSounds(void);
    void UnloadAllSounds(void);