#pragma once
#include "raylib.h"
#include "Enums.h"

// ZOMBIE
typedef struct Zombie
{
    Color color;
    // given in pixels/frame
    float velocity;
    Rectangle format;
    float health;
    // row where zombie was placed
    int rowOfZombie;
    // used to track wheter the zombie is attacking
    bool isAttacking;
    // damage is given in damage/frames
    float damage;
    int pointsPerKill;
    TYPE_OF_ZOMBIE type;
    Texture2D texture;
} Zombie;
//----------------------

// PEASHOT
typedef struct PeaShot
{
    unsigned int damage;
    Rectangle format;
    Color color;
    // velocity:in pixels/frame
    float velocity;
    // usedto optimize the verification of colision with zombies
    int rowOfShot;
    Texture2D texture;
} PeaShot;

//-----------------

// PLANT
typedef struct Plant
{
    Rectangle format;
    TYPE_OF_PLANT type;
    COST_OF_PLANT cost;
    ACTION_TIME_PLANT actionTime;
    float health;
    double creationTime;
    double existanceTime; // Time since the plant was created
    double referenceTime; // Time when the plant was last updated (used track when to activate the plant's action)
    Color color;
    PeaShot peashot;
    float damage;
    int rowOfPlant;

    Texture2D texture; // Kept for compatibility (not all plants have animations)
    Texture2D idleTexture;
    Texture2D actionTexture;

    int frames_per_row_idle_animation;    // Total frames per row in idle animation
    int frames_per_column_idle_animation; // Total frames per column in idle animation
    float idle_animation_duration;        // Duration of idle animation in seconds (frames_per_row_idle_animation*frames_per_column_idle_animation/TARGET_FPS)

    int frames_per_row_action_animation;    // Total frames per row in action animation
    int frames_per_column_action_animation; // Total frames per column in action animation
    float action_animation_duration;        // Duration of action animation in seconds (frames_per_row_action_animation*frames_per_column_action_animation/TARGET_FPS)

    bool isAction;         // Indicates if the plant is in action
    float actionStartTime; // Indicates when the action of the plant started (useful for the action animation)
    int currentFrame;      // Current frame of the animation (useful to track where in the spritesheet the plant is)
    float frameTime;       // Time per frame (usually 1/TARGET_FPS, but can be adjusted for different animations, so as to slow down or speed up the animations)

} Plant;

// GAMING_DECK
typedef struct
{
    Plant plant;
    COOLDOWN_OF_PLANT cooldown;
    float timeToTrackCooldown;
} Gaming_Deck;

// STRUCT CONSTANTS--------------------------------

extern Plant PLANT_SUNFLOWER;
extern Plant PLANT_GREEN_PEASHOOTER;
extern Plant PLANT_WALLNUT;
extern Plant SHOVEL_REMOVE_PLANTS;
extern Plant PLANT_POTATO_MINE;

extern PeaShot NORMAL_GREEN_PEASHOT;

extern Zombie NORMAL_ZOMBIE;
extern Zombie CONEHEAD_ZOMBIE;
extern Zombie FOOTBALL_ZOMBIE;
extern Zombie FLAG_ZOMBIE;
extern Zombie GIGA_GARGANTUAR_BOSS_ZOMBIE;
extern Zombie YETI_ZOMBIE;

extern const Plant NULL_PLANT;
extern const PeaShot NULL_PEA;
extern const Zombie NULL_ZOMBIE;

// FUNCTIONS-------------------------------------
void InitGameStructs(void);