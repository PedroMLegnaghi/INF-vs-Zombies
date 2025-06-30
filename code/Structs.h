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
    double existanceTime;
    double referenceTime;
    Color color;
    int rowOfPlant;
    Texture2D texture;                      // Mantenha para compatibilidade
    Texture2D idleTexture;                  // Textura para estado idle
    Texture2D actionTexture;                // Textura para estado action
    int frames_per_row_idle_animation;      // Total de quadros na animação idle
    int frames_per_column_idle_animation;   // Total de colunas na animação idle
    float idle_animation_duration;          // Duração da animação em segundos (frames_per_row_idle_animation/TARGET_FPS)
    int frames_per_row_action_animation;    // Total de quadros na animação de ação
    int frames_per_column_action_animation; // Total de colunas na animação de ação
    float action_animation_duration;        // Duração da animação de ação em segundos (frames_per_row_action_animation/TARGET_FPS)
    PeaShot peashot;
    float damage;

    // Novos campos para animação
    bool isAction;         // Indica se a planta está em ação
    float actionStartTime; // Tempo em que começou a ação
    int currentFrame;      // Quadro atual da animação
    float frameTime;       // Tempo por quadro (será calculado automaticamente)
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