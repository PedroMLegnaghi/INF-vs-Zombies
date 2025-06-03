#pragma once
#include "raylib.h"
#include "Enums.h"  

//ZOMBIE
    typedef struct Zombie{
    Color color;
    //given in pixels/frame
    float velocity;
    Rectangle format;
    float health;
    //row where zombie was placed
    int rowOfZombie;
    //used to track wheter the zombie is attacking
    bool isAttacking;
    //damage is given in damage/frames
    float damage;
    Texture2D texture;
}Zombie;
//----------------------

//PEASHOT
typedef struct PeaShot
{
    unsigned int damage;
    Rectangle format;
    Color color;
    //velocity:in pixels/frame
    float velocity;
    //usedto optimize the verification of colision with zombies
    int rowOfShot;
    Texture2D texture;
}PeaShot;

//-----------------

//PLANT
typedef struct Plant    
{
    Rectangle format;
    TYPE_OF_PLANT type;
    COST_OF_PLANT cost;
    ACTION_TIME_PLANT actionTime;
    //uses enum of health
    float health;
    double creationTime;
    double existanceTime;
    //referenceTime:Auxilar variable to enable time tracking and habilities triggered of each plant
    //      :-> referenceTime goes from 0 to actionTime. When referenceTime == actionTime, Hability is triggered and referenceTime =0;
    double referenceTime;
    Color color;
    int rowOfPlant;
    Texture2D texture;
    PeaShot peashot;
    
}Plant;

//STRUCT CONSTANTS--------------------------------

extern Plant PLANT_SUNFLOWER;
extern Plant PLANT_GREEN_PEASHOOTER;
extern Plant PLANT_WALLNUT;
extern Plant SHOVEL_REMOVE_PLANTS;
extern PeaShot NORMAL_GREEN_PEASHOT;
extern Zombie NORMAL_ZOMBIE;

extern const Plant NULL_PLANT;
extern const PeaShot NULL_PEA;
extern const Zombie NULL_ZOMBIE;

//FUNCTIONS-------------------------------------
void InitGameStructs(void);