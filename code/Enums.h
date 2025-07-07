#pragma once
// enumaration to reference to the gameScreens of the game
typedef enum GAME_SCREEN
{
    LOGO = 0,
    HOMEPAGE,
    PLAY,
    LEADERBOARD,
    ABOUT,
    CONFIGURATIONS,
    EXIT,
    USER_DATA,
    DECK_SELECTION,
    GAMEPLAY,
    END_GAME,
    MENU,
    RESUME,
    WIN
} GAME_SCREEN;

// enumeration to reference the options of the configurations menu
typedef enum CONFIGURATIONS_OPTIONS
{
    CONFIGURATIONS_BTN_ZOMBIES = 0,
    CONFIGURATIONS_SOUND_ZOMBIES,
    CONFIGURATIONS_BTN_PLANTS,
    CONFIGURATIONS_SOUND_PLANTS,
    CONFIGURATIONS_BTN_BACKGROUND,
    CONFIGURATIONS_SOUND_BACKGROUND,
    CONFIGURATIONS_BTN_PEASHOTS,
    CONFIGURATIONS_SOUND_PEASHOTS,
    CONFIGURATIONS_BTN_SUNS,
    CONFIGURATIONS_SOUND_SUNS,
    CONFIGURATIONS_BTN_SOUNDEFFECTS,
    CONFIGURATIONS_SOUND_SOUNDEFFECTS,
    CONFIGURATIONS_BTN_GOBACK
} CONFIGURATIONS_OPTIONS;

// enumeration to reference the cost of each plant.
// model: COST_(type_of_plant)
typedef enum COST_OF_PLANT
{
    COST_SUNFLOWER = 50,
    COST_PEASHOOTER = 100,
    COST_POTATO_MINE = 25,
    COST_WALLNUT = 75

} COST_OF_PLANT;

// enumeration to reference the time(in seconds) of the action of each plant.
// model: ACTION_TIME_(type_of_plant)
typedef enum action_time
{
    ACTION_TIME_SUNFLOWER = 6,
    ACTION_TIME_PEASHOOTER = 2,

} ACTION_TIME_PLANT;

// enumeration to reference the TYPE_OF_PLANTs available
// model: TYPE_(Plant)
typedef enum TYPE_OF_PLANT
{
    TYPE_NULL_PLANT = 0,
    TYPE_SUNFLOWER,
    TYPE_GREEN_PEASHOOTER,
    TYPE_POTATO_MINE,
    TYPE_WALLNUT,
    TYPE_SHOVEL,
} TYPE_OF_PLANT;

// enumaration to reference the HEALTH_OF_PLANT
// model: HEALTH_OF_(type_of_plant)
typedef enum HEALTH_OF_PLANT
{
    HEALTH_OF_SUNFLOWER = 100,
    HEALTH_OF_GREEN_PEASHOOTER = 200,
    HEALTH_OF_POTATO_MINE = 1,
    HEALTH_OF_WALLNUT = 500
} HEALTH_OF_PLANT;

// enumeration to referente the COOLDOWN_OF_PLANTS
// model: COOLDOWN_OF_(ype_of_plant)
typedef enum
{
    COOLDOWN_OF_SUNFLOWER = 3,
    COOLDOWN_OF_GREEN_PEASHOOTER = 5,
    COOLDOWN_OF_POTATO_MINE = 15,
    COOLDOWN_OF_WALLNUT = 10
} COOLDOWN_OF_PLANT;

// enumeration to reference the TYPE_OF_ZOMBIEs available
// model: TYPE_(Zombie)
typedef enum TYPE_OF_ZOMBIE
{
    TYPE_NULL_ZOMBIE = 0,
    TYPE_NORMAL_ZOMBIE,
    TYPE_CONEHEAD_ZOMBIE,
    TYPE_FLAG_ZOMBIE,
    TYPE_FOOTBALL_ZOMBIE,
    TYPE_GIGA_GARGANTUAR_BOSS_ZOMBIE,
    TYPE_YETI_ZOMBIE
} TYPE_OF_ZOMBIE;

// enumeration to reference the HEALTH_OF_ZOMBIEs available
// model: HEALTH_OF_(type_of_zombie)
typedef enum HEALTH_OF_ZOMBIE
{
    HEALTH_OF_NORMAL_ZOMBIE = 100,
    HEALTH_OF_CONEHEAD_ZOMBIE = 150,
    HEALTH_OF_FOOTBALL_ZOMBIE = 80,
    HEALTH_OF_FLAG_ZOMBIE = 200,
    HEALTH_OF_GIGA_GARGANTUAR_BOSS_ZOMBIE = 3000,
    HEALTH_OF_YETI_ZOMBIE = 600
} HEALTH_OF_ZOMBIE;
