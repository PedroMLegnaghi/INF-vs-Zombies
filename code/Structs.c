#include "Structs.h"
#include "Enums.h"
#include "Textures.h"
#include "main.h"
#include "lawn.h"

Plant PLANT_SUNFLOWER;
Plant PLANT_GREEN_PEASHOOTER;
Plant PLANT_WALLNUT;
Plant SHOVEL_REMOVE_PLANTS;
PeaShot NORMAL_GREEN_PEASHOT;
Zombie NORMAL_ZOMBIE;

//Simple constants, used for initialization of structs or verification of emptiness
const Plant NULL_PLANT = {0};
const PeaShot NULL_PEA = {0};
const Zombie NULL_ZOMBIE = {0};

//InitGameStructs:
// Function that initializes game structures
void InitGameStructs(void) {
    SHOVEL_REMOVE_PLANTS = (Plant){
        .actionTime=0,
        .color=0,
        .cost=0,
        .creationTime=0,
        .existanceTime=0,
        .format.height= LAWN_HEIGHT_VALUE-20,
        .format.width= LAWN_WIDTH_VALUE-40,
        .health=0,
        .peashot=0,
        .referenceTime=0,
        .rowOfPlant=0,
        .texture=TEXTURE_SHOVEL_IMG,
        .type=TYPE_SHOVEL
    };

    PLANT_SUNFLOWER = (Plant){
        .format.height= LAWN_HEIGHT_VALUE-20,
        .format.width= LAWN_WIDTH_VALUE-40,
        .format.x= 0,
        .format.y= 0,
        .type = TYPE_SUNFLOWER,
        .cost = COST_SUNFLOWER,
        .color = BROWN, 
        .actionTime=ACTION_TIME_SUNFLOWER,
        .existanceTime=0,
        .referenceTime=0,
        .health=HEALTH_OF_SUNFLOWER,
        .texture = TEXTURE_SUNFLOWER_IMG,
        .rowOfPlant=-1
    };

    NORMAL_GREEN_PEASHOT = (PeaShot){
        .damage=20,
        .format={
            .height=20,
            .width=20,
            .x=0,
            .y=0
        },
        .color=WHITE,
        .velocity=1,
        .rowOfShot=0,
        .texture = TEXTURE_GREEN_PEASHOT_IMG
    };

    PLANT_GREEN_PEASHOOTER = (Plant){
        .format.height= LAWN_HEIGHT_VALUE-20,
        .format.width= LAWN_WIDTH_VALUE-40,
        .format.x= 0,
        .format.y= 0,
        .type = TYPE_GREEN_PEASHOOTER,
        .cost = COST_PEASHOOTER,
        .color = BLUE, 
        .actionTime=ACTION_TIME_PEASHOOTER,
        .existanceTime=0,
        .referenceTime=0,
        .health=HEALTH_OF_GREEN_PEASHOOTER,
        .peashot = NORMAL_GREEN_PEASHOT,
        .texture = TEXTURE_GREEN_PEASHOOTER_IMG,
        .rowOfPlant=-1
    };

    PLANT_WALLNUT = (Plant){
        .format.height= LAWN_HEIGHT_VALUE-20,
        .format.width= LAWN_WIDTH_VALUE-40,
        .format.x= 0,
        .format.y= 0,
        .type = TYPE_WALLNUT,
        .cost = COST_WALLNUT,
        .color = BLUE, 
        .existanceTime=0,
        .referenceTime=0,
        .health=HEALTH_OF_WALLNUT,
        .texture = TEXTURE_WALLNUT_IMG
    };

    NORMAL_ZOMBIE = (Zombie){
        .color=GRAY,
        .velocity = 0.5,
        .health =100,
        .rowOfZombie=-10,
        .format={
            .x=SCREEN_WIDTH+30,
            .y=0,
            .width=60,
            .height=LAWN_Y_VALUE
        },
        .isAttacking=0,
        .damage =0.33,
        .texture = TEXTURE_NORMAL_ZOMBIE_IMG
    };
}