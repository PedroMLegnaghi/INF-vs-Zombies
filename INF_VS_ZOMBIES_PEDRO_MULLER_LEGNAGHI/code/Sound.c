#include "raylib.h"
#include "sound.h"
// btn
Sound SOUND_BTN_CLICK;
Sound SOUND_BTN_HOVER;

// gamestages--
Sound SOUND_LOST_MUSIC;
Sound SOUND_PAUSE;
Sound SOUND_ZOMBIES_COMING;
Sound SOUND_WIN;

// plants
Sound SOUND_PEASHOT_IMPACT;
Sound SOUND_PLANTING_PLANT;
Sound SOUND_COLLECTING_SUN;
Sound SOUND_SHOVEL;
Sound SOUND_POTATO_MINE_EXPLOSION;

// soundtracks
Sound SOUND_HOMEPAGE_MENU;

Sound SOUND_GAMEPLAY;

// zombies
Sound SOUND_ZOMBIE_SPAWN;
Sound SOUND_ZOMBIE_EAT_PLANT;

// Load all sounds
void LoadAllSounds(void)
{
    // btn
    SOUND_BTN_CLICK = LoadSound("./resources/sound/buttons/buttonclick.ogg");
    SOUND_BTN_HOVER = LoadSound("./resources/sound/buttons/ceramic.ogg");

    // gamestages--
    SOUND_LOST_MUSIC = LoadSound("./resources/sound/gameStages/losemusic.ogg");
    SOUND_PAUSE = LoadSound("./resources/sound/gameStages/pause.ogg");
    SOUND_ZOMBIES_COMING = LoadSound("./resources/sound/gameStages/thezombiesarecomming.ogg");
    SetSoundVolume(SOUND_ZOMBIES_COMING, 1.0f);
    SOUND_WIN = LoadSound("./resources/sound/gameStages/win.ogg");

    // plants
    SOUND_PEASHOT_IMPACT = LoadSound("./resources/sound/plant/juicy.ogg");
    SOUND_PLANTING_PLANT = LoadSound("./resources/sound/plant/plant.ogg");
    SOUND_COLLECTING_SUN = LoadSound("./resources/sound/plant/points.ogg");
    SOUND_SHOVEL = LoadSound("./resources/sound/plant/shovel.ogg");
    SOUND_POTATO_MINE_EXPLOSION = LoadSound("./resources/sound/plant/potatomine.ogg");

    // soundtracks
    SOUND_HOMEPAGE_MENU = LoadSound("./resources/sound/soundtracks/homepage.mp3");
    SetSoundVolume(SOUND_HOMEPAGE_MENU, 0.4f);

    SOUND_GAMEPLAY = LoadSound("./resources/sound/soundtracks/gameplay.mp3");
    SetSoundVolume(SOUND_GAMEPLAY, 0.2f);

    // zombies
    SOUND_ZOMBIE_SPAWN = LoadSound("./resources/sound/zombies/groan3.ogg");
    SOUND_ZOMBIE_EAT_PLANT = LoadSound("./resources/sound/zombies/bigchomp.ogg");
}

void UnloadAllSounds(void)
{
    // Unload all sounds
    UnloadSound(SOUND_BTN_CLICK);
    UnloadSound(SOUND_BTN_HOVER);
    UnloadSound(SOUND_COLLECTING_SUN);
    UnloadSound(SOUND_GAMEPLAY);
    UnloadSound(SOUND_HOMEPAGE_MENU);
    UnloadSound(SOUND_LOST_MUSIC);
    UnloadSound(SOUND_PAUSE);
    UnloadSound(SOUND_PEASHOT_IMPACT);
    UnloadSound(SOUND_PLANTING_PLANT);
    UnloadSound(SOUND_SHOVEL);
    UnloadSound(SOUND_WIN);
    UnloadSound(SOUND_ZOMBIE_EAT_PLANT);
    UnloadSound(SOUND_ZOMBIE_SPAWN);
    UnloadSound(SOUND_ZOMBIES_COMING);
    UnloadSound(SOUND_POTATO_MINE_EXPLOSION);
}

void PlaySoundInLoop(Sound sound)
{
    if (!IsSoundPlaying(sound))
    {
        PlaySound(sound);
    }
}