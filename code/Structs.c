#include "Structs.h"
#include "Enums.h"
#include "Textures.h"
#include "main.h"
#include "lawn.h"

Plant PLANT_SUNFLOWER;
Plant PLANT_GREEN_PEASHOOTER;
Plant PLANT_WALLNUT;
Plant PLANT_POTATO_MINE;
Plant SHOVEL_REMOVE_PLANTS;

PeaShot NORMAL_GREEN_PEASHOT;

Zombie NORMAL_ZOMBIE;
Zombie CONEHEAD_ZOMBIE;
Zombie FOOTBALL_ZOMBIE;
Zombie FLAG_ZOMBIE;
Zombie GIGA_GARGANTUAR_BOSS_ZOMBIE;
Zombie YETI_ZOMBIE;

// Simple constants, used for initialization of structs or verification of emptiness
const Plant NULL_PLANT = {0};
const PeaShot NULL_PEA = {0};
const Zombie NULL_ZOMBIE = {0};

// InitGameStructs:
//  Function that initializes game structures
void InitGameStructs(void)
{
    float plant_width = LAWN_WIDTH_VALUE - 30;
    float plant_height = LAWN_HEIGHT_VALUE - 10;
    SHOVEL_REMOVE_PLANTS = (Plant){
        .actionTime = 0,
        .color = 0,
        .cost = 0,
        .creationTime = 0,
        .existanceTime = 0,
        .format.height = plant_height,
        .format.width = plant_width,
        .health = 0,
        .peashot = 0,
        .referenceTime = 0,
        .rowOfPlant = 0,
        .texture = TEXTURE_SHOVEL_IMG,
        .type = TYPE_SHOVEL};

    PLANT_SUNFLOWER = (Plant){
        .format.height = plant_height,
        .format.width = plant_width,
        .format.x = 0,
        .format.y = 0,
        .type = TYPE_SUNFLOWER,
        .cost = COST_SUNFLOWER,
        .color = BROWN,
        .actionTime = ACTION_TIME_SUNFLOWER,
        .existanceTime = 0,
        .referenceTime = 0,
        .health = HEALTH_OF_SUNFLOWER,
        .texture = TEXTURE_SUNFLOWER_IMG,
        .rowOfPlant = -1,

        .idleTexture = TEXTURE_SUNFLOWER_IDLE_IMG,
        .frames_per_row_idle_animation = 36,
        .frames_per_column_idle_animation = 1,
        .idle_animation_duration = 36 * 1 / (float)TARGET_FPS,

        .actionTexture = 0,
        .frames_per_row_action_animation = 0,
        .frames_per_column_action_animation = 0,
        .action_animation_duration = 0,

        .rowOfPlant = -1,

        .isAction = false,
        .actionStartTime = 0,
        .currentFrame = 0,
        .frameTime = 3.0f / TARGET_FPS};

    PLANT_POTATO_MINE = (Plant){
        .format.height = plant_height,
        .format.width = plant_width,
        .format.x = 0,
        .format.y = 0,
        .type = TYPE_POTATO_MINE,
        .cost = COST_POTATO_MINE,
        .color = BROWN,
        .existanceTime = 0,
        .referenceTime = 0,
        .health = HEALTH_OF_POTATO_MINE,
        .texture = TEXTURE_POTATO_MINE_IMG,
        .rowOfPlant = -1,
        .damage = 600,

        .idleTexture = TEXTURE_POTATO_MINE_IDLE_IMG,
        .frames_per_row_idle_animation = 29,
        .frames_per_column_idle_animation = 1,
        .idle_animation_duration = 29 * 1 / (float)TARGET_FPS,

        .actionTexture = 0,
        .frames_per_row_action_animation = 0,
        .frames_per_column_action_animation = 0,
        .action_animation_duration = 0,

        .rowOfPlant = -1,

        .isAction = false,
        .actionStartTime = 0,
        .currentFrame = 0,
        .frameTime = 3.0f / TARGET_FPS};

    NORMAL_GREEN_PEASHOT = (PeaShot){
        .damage = 20,
        .format = {
            .height = 45,
            .width = 45,
            .x = 0,
            .y = 0},
        .color = WHITE,
        .velocity = 3,
        .rowOfShot = 0,
        .texture = TEXTURE_GREEN_PEASHOT_IMG};

    PLANT_GREEN_PEASHOOTER = (Plant){
        .format.height = plant_height,
        .format.width = plant_width,
        .format.x = 0,
        .format.y = 0,
        .type = TYPE_GREEN_PEASHOOTER,
        .cost = COST_PEASHOOTER,
        .color = BLUE,
        .actionTime = ACTION_TIME_PEASHOOTER,
        .existanceTime = 0,
        .referenceTime = 0,
        .health = HEALTH_OF_GREEN_PEASHOOTER,
        .peashot = NORMAL_GREEN_PEASHOT,
        .texture = TEXTURE_GREEN_PEASHOOTER_IMG,

        .idleTexture = TEXTURE_GREEN_PEASHOOTER_IDLE_IMG,
        .frames_per_row_idle_animation = 60,
        .frames_per_column_idle_animation = 1,
        .idle_animation_duration = 60 * 1 / TARGET_FPS,

        .actionTexture = TEXTURE_GREEN_PEASHOOTER_SHOOTING_IMG,
        .frames_per_row_action_animation = 60,
        .frames_per_column_action_animation = 1,
        .action_animation_duration = 60 * 1 / TARGET_FPS,

        .rowOfPlant = -1,

        .isAction = false,
        .actionStartTime = 0,
        .currentFrame = 0,
        .frameTime = 1.0f / TARGET_FPS};

    PLANT_WALLNUT = (Plant){
        .format.height = plant_height,
        .format.width = plant_width,
        .format.x = 0,
        .format.y = 0,
        .type = TYPE_WALLNUT,
        .cost = COST_WALLNUT,
        .color = BLUE,
        .existanceTime = 0,
        .referenceTime = 0,
        .health = HEALTH_OF_WALLNUT,
        .texture = TEXTURE_WALLNUT_IMG};

    NORMAL_ZOMBIE = (Zombie){
        .color = GRAY,
        .velocity = 1,
        .health = HEALTH_OF_NORMAL_ZOMBIE,
        .rowOfZombie = -10,
        .format = {
            .x = SCREEN_WIDTH + 30,
            .y = 0,
            .width = LAWN_WIDTH_VALUE - 30,
            .height = LAWN_HEIGHT_VALUE},
        .isAttacking = 0,
        .damage = 0.33,
        .texture = TEXTURE_NORMAL_ZOMBIE_IMG,
        .pointsPerKill = 100,
        .type = TYPE_NORMAL_ZOMBIE};

    FOOTBALL_ZOMBIE = (Zombie){
        .color = GRAY,
        .velocity = 2,
        .health = HEALTH_OF_FOOTBALL_ZOMBIE,
        .rowOfZombie = -10,
        .format = {
            .x = SCREEN_WIDTH + 30,
            .y = 0,
            .width = LAWN_WIDTH_VALUE - 30,
            .height = LAWN_HEIGHT_VALUE},
        .isAttacking = 0,
        .damage = 1.5,
        .texture = TEXTURE_FOOTBALL_ZOMBIE_IMG,
        .pointsPerKill = 300,
        .type = TYPE_FOOTBALL_ZOMBIE};

    CONEHEAD_ZOMBIE = (Zombie){
        .color = GRAY,
        .velocity = 0.7,
        .health = HEALTH_OF_CONEHEAD_ZOMBIE,
        .rowOfZombie = -10,
        .format = {
            .x = SCREEN_WIDTH + 30,
            .y = 0,
            .width = LAWN_WIDTH_VALUE - 30,
            .height = LAWN_HEIGHT_VALUE},
        .isAttacking = 0,
        .damage = 0.7,
        .texture = TEXTURE_CONEHEAD_ZOMBIE_IMG,
        .pointsPerKill = 200,
        .type = TYPE_CONEHEAD_ZOMBIE};

    FLAG_ZOMBIE = (Zombie){
        .color = GRAY,
        .velocity = 1,
        .health = HEALTH_OF_FLAG_ZOMBIE,
        .rowOfZombie = -10,
        .format = {
            .x = SCREEN_WIDTH + 30,
            .y = 0,
            .width = LAWN_WIDTH_VALUE - 30,
            .height = LAWN_HEIGHT_VALUE},
        .isAttacking = 0,
        .damage = 0.33,
        .texture = TEXTURE_FLAG_ZOMBIE_IMG,
        .pointsPerKill = 100,
        .type = TYPE_FLAG_ZOMBIE};

    GIGA_GARGANTUAR_BOSS_ZOMBIE = (Zombie){
        .color = GRAY,
        .velocity = 0.4,
        .health = HEALTH_OF_GIGA_GARGANTUAR_BOSS_ZOMBIE,
        .rowOfZombie = -10,
        .format = {
            .x = SCREEN_WIDTH + 30,
            .y = 0,
            .width = LAWN_WIDTH_VALUE * 4,
            .height = LAWN_HEIGHT_VALUE * 3},
        .isAttacking = 0,
        .damage = 2,
        .texture = TEXTURE_GIGA_GARGANTUAR_BOSS_ZOMBIE_IMG,
        .pointsPerKill = 500,
        .type = TYPE_GIGA_GARGANTUAR_BOSS_ZOMBIE};

    YETI_ZOMBIE = (Zombie){
        .color = GRAY,
        .velocity = 0.4,
        .health = HEALTH_OF_YETI_ZOMBIE,
        .rowOfZombie = -10,
        .format = {
            .x = SCREEN_WIDTH + 30,
            .y = 0,
            .width = LAWN_WIDTH_VALUE,
            .height = LAWN_HEIGHT_VALUE},
        .isAttacking = 0,
        .damage = 2,
        .texture = TEXTURE_YETI_ZOMBIE_IMG,
        .pointsPerKill = 700,
        .type = TYPE_YETI_ZOMBIE};
}