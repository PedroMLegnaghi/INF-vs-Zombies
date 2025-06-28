#include "Textures.h"
#include "raylib.h"

Vector2 origin = {0, 0};

Texture2D TEXTURE_BACKGROUND_IMG;
Rectangle TEXTURE_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_GAMING_BACKGROUND_IMG;
Rectangle TEXTURE_GAMING_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_WIN_BACKGROUND_IMG;
Rectangle TEXTURE_WIN_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG;
Rectangle TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_CONFIGURATIONS_BACKGROUND_IMG;
Rectangle TEXTURE_CONFIGURATIONS_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_LEADERBOARD_BACKGROUND_IMG;
Rectangle TEXTURE_LEADERBOARD_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_USERDATA_BACKGROUND_IMG;
Rectangle TEXTURE_USERDATA_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_SUN_IMG;
Rectangle TEXTURE_SUN_IMG_SOURCE_REC;

Texture2D TEXTURE_DIRT_IMG;
Rectangle TEXTURE_DIRT_IMG_SOURCE_REC;

Texture2D TEXTURE_GRASS_IMG;
Rectangle TEXTURE_GRASS_IMG_SOURCE_REC;

Texture2D TEXTURE_EXIT_BTN_IMG;
Rectangle TEXTURE_EXIT_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_LEADERBOARD_BTN_IMG;
Rectangle TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_PLAY_BTN_IMG;
Rectangle TEXTURE_PLAY_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_CONFIGURATIONS_BTN_IMG;
Rectangle TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_ABOUT_BTN_IMG;
Rectangle TEXTURE_ABOUT_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_RESUME_BTN_IMG;
Rectangle TEXTURE_RESUME_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_GOBACK_BTN_IMG;
Rectangle TEXTURE_GOBACK_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_ZOMBIES_BTN_IMG;
Rectangle TEXTURE_ZOMBIES_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_PLANTS_BTN_IMG;
Rectangle TEXTURE_PLANTS_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_PEASHOTS_BTN_IMG;
Rectangle TEXTURE_PEASHOTS_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_BACKGROUND_BTN_IMG;
Rectangle TEXTURE_BACKGROUND_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_SOUNDEFFECTS_BTN_IMG;
Rectangle TEXTURE_SOUNDEFFECTS_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_SUNS_BTN_IMG;
Rectangle TEXTURE_SUNS_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_UNMUTE_BTN_IMG;
Rectangle TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_MUTE_BTN_IMG;
Rectangle TEXTURE_MUTE_BTN_IMG_SOURCE_REC;

Texture2D TEXTURE_ENDGAME_BACKGROUND_IMG;
Rectangle TEXTURE_ENDGAME_BACKGROUND_IMG_SOURCE_REC;

Texture2D TEXTURE_NORMAL_ZOMBIE_IMG;
Texture2D TEXTURE_FOOTBALL_ZOMBIE_IMG;
Texture2D TEXTURE_CONEHEAD_ZOMBIE_IMG;
Texture2D TEXTURE_FLAG_ZOMBIE_IMG;
Texture2D TEXTURE_GIGA_GARGANTUAR_BOSS_ZOMBIE_IMG;

Texture2D TEXTURE_SUNFLOWER_IMG;
Texture2D TEXTURE_GREEN_PEASHOOTER_IMG;
Texture2D TEXTURE_GREEN_PEASHOT_IMG;
Texture2D TEXTURE_WALLNUT_IMG;
Texture2D TEXTURE_SHOVEL_IMG;

void LoadAllTextures(void)
{
    // Backgrounds
    TEXTURE_BACKGROUND_IMG = LoadTexture("./resources/sprites/coloredBackGroundPVZ.png");
    TEXTURE_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_BACKGROUND_IMG.width, TEXTURE_BACKGROUND_IMG.height};

    TEXTURE_GAMING_BACKGROUND_IMG = LoadTexture("./resources/sprites/dayBackgroundGame.png");
    TEXTURE_GAMING_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, SCREEN_WIDTH * 1.2, TEXTURE_GAMING_BACKGROUND_IMG.height};

    TEXTURE_ENDGAME_BACKGROUND_IMG = LoadTexture("./resources/sprites/endGame.png");
    TEXTURE_ENDGAME_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_ENDGAME_BACKGROUND_IMG.width, TEXTURE_ENDGAME_BACKGROUND_IMG.height};

    TEXTURE_WIN_BACKGROUND_IMG = LoadTexture("./resources/sprites/win_background.png");
    TEXTURE_WIN_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_WIN_BACKGROUND_IMG.width, TEXTURE_WIN_BACKGROUND_IMG.height};

    TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG = LoadTexture("./resources/sprites/confirmingQuit.png");
    TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG.width, TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG.height};

    TEXTURE_CONFIGURATIONS_BACKGROUND_IMG = LoadTexture("./resources/sprites/configurationsBackground.png");
    TEXTURE_CONFIGURATIONS_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_CONFIGURATIONS_BACKGROUND_IMG.width, TEXTURE_CONFIGURATIONS_BACKGROUND_IMG.height};

    TEXTURE_LEADERBOARD_BACKGROUND_IMG = LoadTexture("./resources/sprites/leaderboard.png");
    TEXTURE_LEADERBOARD_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_LEADERBOARD_BACKGROUND_IMG.width, TEXTURE_LEADERBOARD_BACKGROUND_IMG.height};

    TEXTURE_USERDATA_BACKGROUND_IMG = LoadTexture("./resources/sprites/userData_background.png");
    TEXTURE_USERDATA_BACKGROUND_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_USERDATA_BACKGROUND_IMG.width, TEXTURE_USERDATA_BACKGROUND_IMG.height};

    // Sun
    TEXTURE_SUN_IMG = LoadTexture("./resources/sprites/sun.png");
    TEXTURE_SUN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_SUN_IMG.width, TEXTURE_SUN_IMG.height};

    // Lawn
    TEXTURE_DIRT_IMG = LoadTexture("./resources/sprites/dirt.png");
    TEXTURE_DIRT_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_DIRT_IMG.width, TEXTURE_DIRT_IMG.height};

    TEXTURE_GRASS_IMG = LoadTexture("./resources/sprites/grass.png");
    TEXTURE_GRASS_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_GRASS_IMG.width, TEXTURE_GRASS_IMG.height};

    // Buttons
    TEXTURE_EXIT_BTN_IMG = LoadTexture("./resources/sprites/exit-button.png");
    TEXTURE_EXIT_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_EXIT_BTN_IMG.width, TEXTURE_EXIT_BTN_IMG.height};

    TEXTURE_LEADERBOARD_BTN_IMG = LoadTexture("./resources/sprites/leaderboard-button.png");
    TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_LEADERBOARD_BTN_IMG.width, TEXTURE_LEADERBOARD_BTN_IMG.height};

    TEXTURE_PLAY_BTN_IMG = LoadTexture("./resources/sprites/play-button.png");
    TEXTURE_PLAY_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_PLAY_BTN_IMG.width, TEXTURE_PLAY_BTN_IMG.height};

    TEXTURE_CONFIGURATIONS_BTN_IMG = LoadTexture("./resources/sprites/configurations-button.png");
    TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_CONFIGURATIONS_BTN_IMG.width, TEXTURE_CONFIGURATIONS_BTN_IMG.height};

    TEXTURE_ABOUT_BTN_IMG = LoadTexture("./resources/sprites/about-button.png");
    TEXTURE_ABOUT_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_ABOUT_BTN_IMG.width, TEXTURE_ABOUT_BTN_IMG.height};

    TEXTURE_RESUME_BTN_IMG = LoadTexture("./resources/sprites/resume-button.png");
    TEXTURE_RESUME_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_RESUME_BTN_IMG.width, TEXTURE_RESUME_BTN_IMG.height};

    TEXTURE_GOBACK_BTN_IMG = LoadTexture("./resources/sprites/goBack-button.png");
    TEXTURE_GOBACK_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_GOBACK_BTN_IMG.width, TEXTURE_GOBACK_BTN_IMG.height};

    TEXTURE_ZOMBIES_BTN_IMG = LoadTexture("./resources/sprites/zombies-button-transparent.png");
    TEXTURE_ZOMBIES_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_ZOMBIES_BTN_IMG.width, TEXTURE_ZOMBIES_BTN_IMG.height};

    TEXTURE_PLANTS_BTN_IMG = LoadTexture("./resources/sprites/plants-button-transparent.png");
    TEXTURE_PLANTS_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_PLANTS_BTN_IMG.width, TEXTURE_PLANTS_BTN_IMG.height};

    TEXTURE_PEASHOTS_BTN_IMG = LoadTexture("./resources/sprites/peashots-button-transparent.png");
    TEXTURE_PEASHOTS_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_PEASHOTS_BTN_IMG.width, TEXTURE_PEASHOTS_BTN_IMG.height};

    TEXTURE_BACKGROUND_BTN_IMG = LoadTexture("./resources/sprites/background-button-transparent.png");
    TEXTURE_BACKGROUND_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_BACKGROUND_BTN_IMG.width, TEXTURE_BACKGROUND_BTN_IMG.height};

    TEXTURE_SOUNDEFFECTS_BTN_IMG = LoadTexture("./resources/sprites/soundeffects-button-transparent.png");
    TEXTURE_SOUNDEFFECTS_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_SOUNDEFFECTS_BTN_IMG.width, TEXTURE_SOUNDEFFECTS_BTN_IMG.height};

    TEXTURE_SUNS_BTN_IMG = LoadTexture("./resources/sprites/suns-button-transparent.png");
    TEXTURE_SUNS_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_SUNS_BTN_IMG.width, TEXTURE_SUNS_BTN_IMG.height};

    TEXTURE_UNMUTE_BTN_IMG = LoadTexture("./resources/sprites/unmute-button-transparent.png");
    TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_UNMUTE_BTN_IMG.width, TEXTURE_UNMUTE_BTN_IMG.height};

    TEXTURE_MUTE_BTN_IMG = LoadTexture("./resources/sprites/mute-button-transparent.png");
    TEXTURE_MUTE_BTN_IMG_SOURCE_REC = (Rectangle){0, 0, TEXTURE_MUTE_BTN_IMG.width, TEXTURE_MUTE_BTN_IMG.height};

    // Zombies
    TEXTURE_NORMAL_ZOMBIE_IMG = LoadTexture("./resources/sprites/zombie.png");
    TEXTURE_GIGA_GARGANTUAR_BOSS_ZOMBIE_IMG = LoadTexture("./resources/sprites/giga_gargantuar_boss.png");
    TEXTURE_FOOTBALL_ZOMBIE_IMG = LoadTexture("./resources/sprites/footballZombie.png");
    TEXTURE_CONEHEAD_ZOMBIE_IMG = LoadTexture("./resources/sprites/coneHeadZombie.png");
    TEXTURE_FLAG_ZOMBIE_IMG = LoadTexture("./resources/sprites/flagZombie.png");

    // Plants
    TEXTURE_SUNFLOWER_IMG = LoadTexture("./resources/sprites/sunflower.png");
    TEXTURE_GREEN_PEASHOOTER_IMG = LoadTexture("./resources/sprites/peashooter.png");
    TEXTURE_GREEN_PEASHOT_IMG = LoadTexture("./resources/sprites/peashooter-proj.png");
    TEXTURE_WALLNUT_IMG = LoadTexture("./resources/sprites/wallnut.png");
    TEXTURE_SHOVEL_IMG = LoadTexture("./resources/sprites/shovel.png");
}

void UnloadAllTextures(void)
{
    // Backgrounds
    UnloadTexture(TEXTURE_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_LEADERBOARD_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_GAMING_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_CONFIGURATIONS_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_WIN_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_USERDATA_BACKGROUND_IMG);

    // Sun
    UnloadTexture(TEXTURE_SUN_IMG);

    // Lawn
    UnloadTexture(TEXTURE_DIRT_IMG);
    UnloadTexture(TEXTURE_GRASS_IMG);

    // Buttons
    UnloadTexture(TEXTURE_EXIT_BTN_IMG);
    UnloadTexture(TEXTURE_LEADERBOARD_BTN_IMG);
    UnloadTexture(TEXTURE_PLAY_BTN_IMG);
    UnloadTexture(TEXTURE_CONFIGURATIONS_BTN_IMG);
    UnloadTexture(TEXTURE_ABOUT_BTN_IMG);
    UnloadTexture(TEXTURE_RESUME_BTN_IMG);
    UnloadTexture(TEXTURE_GOBACK_BTN_IMG);
    UnloadTexture(TEXTURE_ZOMBIES_BTN_IMG);
    UnloadTexture(TEXTURE_PLANTS_BTN_IMG);
    UnloadTexture(TEXTURE_PEASHOTS_BTN_IMG);
    UnloadTexture(TEXTURE_BACKGROUND_BTN_IMG);
    UnloadTexture(TEXTURE_SOUNDEFFECTS_BTN_IMG);
    UnloadTexture(TEXTURE_SUNS_BTN_IMG);
    UnloadTexture(TEXTURE_UNMUTE_BTN_IMG);
    UnloadTexture(TEXTURE_MUTE_BTN_IMG);

    // Zombies
    UnloadTexture(TEXTURE_NORMAL_ZOMBIE_IMG);
    UnloadTexture(TEXTURE_FOOTBALL_ZOMBIE_IMG);
    UnloadTexture(TEXTURE_CONEHEAD_ZOMBIE_IMG);

    // Plants
    UnloadTexture(TEXTURE_SUNFLOWER_IMG);
    UnloadTexture(TEXTURE_GREEN_PEASHOOTER_IMG);
    UnloadTexture(TEXTURE_GREEN_PEASHOT_IMG);
    UnloadTexture(TEXTURE_WALLNUT_IMG);
    UnloadTexture(TEXTURE_SHOVEL_IMG);
}