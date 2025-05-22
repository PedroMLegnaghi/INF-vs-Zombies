#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // necessary for mmemov


//================================================================================================================================================
//CONSTANTS=======================================================================================================================================
//================================================================================================================================================

#define MAX_SIZE_OF_NAME 15
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
#define GAMING_MENU_OPTIONS_QUANTITY 3 //resume, configurations and exit
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 580
#define TARGET_FPS 60
#define NUMBER_COLUMN_LAWN 9
#define NUMBER_ROWS_LAWN 5
#define VALUE_OF_EACH_SUN 25

//Btn display
#define BTN_WIDTH 280
#define BTN_HEIGHT 120
int BTN_CENTERED_X = (SCREEN_WIDTH-BTN_WIDTH)/2;

//Sizes of arrays
#define SIZE_OF_SUN_ARR 100 //maximum quantity of suns in screen
#define SIZE_OF_DECK 2 
#define SIZE_OF_ZOMBIES_ARR 100 //maximum quantity of zombies in screen
#define SIZE_OF_PEASHOT_ARR 300 //maximum quantity of peashots in screen

//Constants of lawn display
const int DISTANCE_LAWN_RIGHT_EDGE = (0.025)*SCREEN_WIDTH;
const int DISTANCE_LAWN_LEFT_EDGE = DISTANCE_LAWN_RIGHT_EDGE;
const int DISTANCE_LAWN_INFERIOR_EDGE = DISTANCE_LAWN_RIGHT_EDGE;
const int DISTANCE_LAWN_SUPERIOR_EDGE = DISTANCE_LAWN_RIGHT_EDGE*2;
const int LAWN_WIDTH_VALUE = (SCREEN_WIDTH-DISTANCE_LAWN_RIGHT_EDGE-DISTANCE_LAWN_LEFT_EDGE)/NUMBER_COLUMN_LAWN-12;
const int LAWN_HEIGHT_VALUE = (SCREEN_HEIGHT-DISTANCE_LAWN_INFERIOR_EDGE-DISTANCE_LAWN_SUPERIOR_EDGE)/NUMBER_ROWS_LAWN-8;
const int LAWN_X_VALUE = LAWN_WIDTH_VALUE;
const int LAWN_Y_VALUE = LAWN_HEIGHT_VALUE;

//Constants of deck display
const int DECK_RECTANGLE_X_VALUE = 10;
const int DECK_RECTANGLE_Y_VALUE = 10;
const int DECK_ELEMENT_WIDTH_VALUE =LAWN_WIDTH_VALUE;
const int DECK_ELEMENT_HEIGHT_VALUE = 70;

//User's Mouse
Vector2 mousePoint = { 0.0f, 0.0f }; 



// ================================================================================================================================================
// ================================================================================================================================================
// ================================================================================================================================================

//================================================================================================================================================
//ENUM=====================================================================================================================================
//================================================================================================================================================


//enumaration to reference to the gameScreens of the game
typedef enum GAME_SCREEN {LOGO = 0, HOMEPAGE, PLAY, LEADERBOARD, ABOUT, CONFIGURATIONS, EXIT 
    ,USER_DATA, DECK_SELECTION, GAMEPLAY,END_GAME,MENU, RESUME} GAME_SCREEN;
    
//enumeration to reference the cost of each plant.
//model: COST_(type_of_plant)
typedef enum COST_OF_PLANT{
    COST_SUNFLOWER = 50,
    COST_PEASHOOTER = 100,
    
}COST_OF_PLANT;

//enumeration to reference the time(in seconds) of the action of each plant.
//model: ACTION_TIME_(type_of_plant)
typedef enum action_time{
    //time to generate sun = 5seg
    ACTION_TIME_SUNFLOWER = 5,
    ACTION_TIME_PEASHOOTER =5,
    
}ACTION_TIME_PLANT;

//enumeration to reference the TYPE_OF_PLANTs available
//model: TYPE_(Plant)
typedef enum TYPE_OF_PLANT{
    TYPE_SUNFLOWER =0,
    TYPE_GREEN_PEASHOOTER,
}TYPE_OF_PLANT;

//enumaration to reference the HEALTH_OF_PLANT
//model: HEALTH_OF_(type_of_plant)
typedef enum {
    HEALTH_OF_SUNFLOWER = 100,
    HEALTH_OF_GREEN_PEASHOOTER = 100,
}HEALTH_OF_PLANT;


//================================================================================================================================================
//================================================================================================================================================
//================================================================================================================================================

//================================================================================================================================================
//STRUCTS=========================================================================================================================================
//================================================================================================================================================

//ZOMBIE
    typedef struct{
        //png/sprite?
    //sound?
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
}Zombie;
//----------------------

//PEASHOT
typedef struct
{
    unsigned int damage;
    Rectangle format;
    // img?
    // sound?
    Color color;
    //velocity:in pixels/frame
    float velocity;
    //usedto optimize the verification of colision with zombies
    int rowOfShot;

}PeaShot;

//-----------------

//PLANT
typedef struct 
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
    //sound?
    //png?
    
}Plant;

//----------------------------
//================================================================================================================================================
//================================================================================================================================================
//================================================================================================================================================


//================================================================================================================================================
//STRUCT CONSTANTS=====================================================================================================================================
//================================================================================================================================================

//PLANT
const Plant NULL_PLANT ={0};

const Plant PLANT_SUNFLOWER={
    .format.height= LAWN_HEIGHT_VALUE-20,
    .format.width= LAWN_WIDTH_VALUE-20,
    .format.x= 0,
    .format.y= 0,
    .type = TYPE_SUNFLOWER,
    .cost = COST_SUNFLOWER,
    .color = BROWN, 
    .actionTime=ACTION_TIME_SUNFLOWER,
    .existanceTime=0,
    .referenceTime=0,
    .existanceTime=0,
    .health=HEALTH_OF_SUNFLOWER
};

const Plant PLANT_GREEN_PEASHOOTER={
    .format.height= LAWN_HEIGHT_VALUE-20,
    .format.width= LAWN_WIDTH_VALUE-20,
    .format.x= 0,
    .format.y= 0,
    .type = TYPE_GREEN_PEASHOOTER,
    .cost = COST_PEASHOOTER,
    .color = BLUE, 
    .actionTime=ACTION_TIME_PEASHOOTER,
    .existanceTime=0,
    .referenceTime=0,
    .existanceTime=0,
    .health=HEALTH_OF_GREEN_PEASHOOTER
};
//------------------

//PEASHOT
const PeaShot NULL_PEA={0};
const PeaShot NORMAL_GREEN_PEASHOT={
    .damage=20,
    .format={
        .height=5,
        .width=5,
        .x=0,
        .y=0
    },
    .color=WHITE,
    .velocity=1,
    .rowOfShot=0
};
//----------------

//ZOMBIE
const Zombie NORMAL_ZOMBIE={
    .color=GRAY,
    .velocity = 0.5,
    .health =100,
    .rowOfZombie=-1,
    .format={
        //make zombie appear from outside of the window
        .x=SCREEN_WIDTH+30,
        .y=0,
        .width=20,
        .height=LAWN_Y_VALUE
    },
    .isAttacking=0,
    //damage=20/second
    .damage =0.33
};
const Zombie NULL_ZOMBIE={
    .color=0,
    .format={0},
    .health=0,
    .rowOfZombie=0,
    .velocity=0,
    .isAttacking=0,
    .damage=0
};
//--------------------------------

//================================================================================================================================================
//================================================================================================================================================
//================================================================================================================================================


//================================================================================================================================================
//FUNCTIONS=======================================================================================================================================
//================================================================================================================================================


//SUN FUNCTIONS---

//AddSunToArray:
//Given an array of suns, the index of the next sun, the array of lawns, the array of grounds of suns and the proprieties of Rectangle, add that sun to the array of suns 
void AddSunToArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR],  int *indexOfNextSun,Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],int rowOfGround,int columnOfGround,float groundOfTheSuns[SIZE_OF_SUN_ARR], int x, int y, int width, int height) {
    array_of_suns[*indexOfNextSun].x = x;
        array_of_suns[*indexOfNextSun].y =  y;
        array_of_suns[*indexOfNextSun].width = width;
        array_of_suns[*indexOfNextSun].height = height;
        groundOfTheSuns[*indexOfNextSun] =  lawn_array[rowOfGround][columnOfGround].y;

        *indexOfNextSun+=1;
}

//AddRandomlySunToArr:
//Given an array of suns, the index of the next sun , the array of lawns of the game and the groundOfTheSuns array, add that sun to the array with x and y position of a random lawn of the game 
void AddRandomlySunToArr(Rectangle array_of_suns[SIZE_OF_SUN_ARR],  int *indexOfNextSun, Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],float groundOfTheSuns[SIZE_OF_SUN_ARR]) {
    if (*indexOfNextSun >= SIZE_OF_SUN_ARR) return;
    //chooses a number beetwen 0 and NUMBER_ROWS_LAWN
    int row = rand() % (NUMBER_ROWS_LAWN);
    //chooses a number beetwen 0 and NUMBER_COLUMN_LAWN
    int column = rand() % (NUMBER_COLUMN_LAWN);

    AddSunToArray(array_of_suns,indexOfNextSun,lawn_array,row,column,groundOfTheSuns,lawn_array[row][column].x,-20,20,20);
}

//updateSunsPosition:
//given the array of the suns of the game ,groundOfTheSuns and the index of the next sun, update their position, until they hit their ground
void updateSunsPosition(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfSun,float groundOfTheSuns[SIZE_OF_SUN_ARR]){
    for(int i=0;i<indexOfSun;i++){

        //if the sun hasn't hit it's ground, (we use "lesser than" here to avoid rounding problems)
        if((array_of_suns[i].y<groundOfTheSuns[i]))
            //update its position
            array_of_suns[i].y+=0.5;

        if (array_of_suns[i].y > groundOfTheSuns[i]) {
            array_of_suns[i].y = groundOfTheSuns[i]; // prevent it from falling under the ground
        }
    }
};

//RemoveSunToArray:
//Given an array of suns ,groundOfTheSuns and the index of the sun, remove the last sun of the arrayofsuns and remove its ground and update the array
void RemoveSunOfArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun, int indexOfSunRemoved, float groundOfTheSuns[SIZE_OF_SUN_ARR]) {
    if (indexOfSunRemoved < 0 || indexOfSunRemoved >= *indexOfNextSun) return; //checking if valid index

    for (int i = indexOfSunRemoved; i < (*indexOfNextSun) - 1; i++) {
        array_of_suns[i] = array_of_suns[i + 1]; //update the sun array (similar to mmemove())
        groundOfTheSuns[i] = groundOfTheSuns[i + 1]; //update the groundOfTheSuns array (similar to mmemove())
    }
    (*indexOfNextSun)--;

    // make the last index invalid (because it moved from the last position to the (last-1) position)
    array_of_suns[*indexOfNextSun].x = -1;
    array_of_suns[*indexOfNextSun].y = -1;
    groundOfTheSuns[*indexOfNextSun] = 0;
}

//collectSun:
//Given an array of suns, the mousePointer,the quantityofSun of the player, the index of the next sun and the array of the grounds of the suns, returns wheter one sun was collected(1) or not(0) and collects it
int collectSun(Rectangle array_of_suns[SIZE_OF_SUN_ARR],int *indexOfNextSun, float groundOfTheSuns[SIZE_OF_SUN_ARR])
{
    for (int i = 0; i < *indexOfNextSun; i++) {
        
        if (CheckCollisionPointRec(mousePoint, array_of_suns[i])) { //if mouse of player and the sun collided
            if (IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { //and if the player clicked on the sun
                RemoveSunOfArray(array_of_suns, indexOfNextSun, i, groundOfTheSuns); //collect the sun
                return 1;//Sun was collected
                break; // removes only one sun per click
            }
        }
    }
    return 0;//sun was not collected
}
//addSunToStorage:
//given the gamingSunStorage, add the value of one sun to the storage
void addSunToStorage(unsigned int *gamingSunStorage){
    *gamingSunStorage+=VALUE_OF_EACH_SUN;
}

//DrawSuns:
//Given the array of suns of the game and the index of the sun, draw the suns
void DrawSuns(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfNextSun){
    for(int i = 0; i < indexOfNextSun; i++){
        if (array_of_suns[i].x != -1 && array_of_suns[i].y != -1) { //if the sun is not an invalid sun
            DrawRectangle(array_of_suns[i].x, array_of_suns[i].y,
                        array_of_suns[i].width, array_of_suns[i].height, YELLOW);//draw it
        }
    }
}

//--------------------------------------------

//PLANTS FUNCTIONS-----

//UpdateHealthOfPlant: Given a plant, updates it's health accordingly with a given damage
void UpdateHealthOfPlant(Plant *plant, float damage){
    //verify if damage is a valid damage
    if(damage<0) return;
        plant->health-=damage;
}

//RemovePlantFromArr: Remove a certain plant from the array    
void RemovePlantFromArr(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],int rowOfPlantToBeRemoved, int columnOfPlantToBeRemoved ){
    plantArr[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved]=NULL_PLANT;//null the plant == remove
    occupationOfLawn[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved]=0;//update the occupation of the lawn
}

//UpdateExistanceTime:
//given a Plant array, update the existance time of each plant
void UpdateExistanceTime(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]){
    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
            plantArr[i][j].existanceTime=GetTime()-plantArr[i][j].creationTime;
        }
    }
}

//UpdateReferenceTime:
//given a Plant, the reference time for that especific plant is set as existanceTime
void UpdateReferenceTime(Plant *plant){
    plant->referenceTime=plant->existanceTime;
}

//DrawPlants:
//draw all plants until indexOfNextPlant within the array of Plant passed
void DrawPlants(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]){
    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
            //if the plant [i][j] isn't empty(NULLED)
            if(plantArr[i][j].format.x!=NULL_PLANT.format.x){
                DrawRectangleRec(plantArr[i][j].format,plantArr[i][j].color );
            }
        }
    }
}

    //PLANTS/SUNFLOWER-----------------

// GenerateSunSunflower:
// given the array of plants and the array of suns, add to the array of suns a sun near 
// the sunflower that generated it
void GenerateSunSunflower(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],float groundOfTheSuns[SIZE_OF_SUN_ARR], Rectangle arr_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun){
    for(int i=0; i<NUMBER_ROWS_LAWN; i++){
        for(int j=0; j<NUMBER_COLUMN_LAWN; j++){

            // if the plant exists and is a sunflower
            if (plantArr[i][j].type == TYPE_SUNFLOWER&&plantArr[i][j].format.x != NULL_PLANT.format.x)
            {
                // if it is time to generate a sun
                if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime))
                {
                    // update reference time properly, to enable tracking when it is time to generate another sun of that plant
                    UpdateReferenceTime(&plantArr[i][j]);

                    // check sun limit
                    if (*indexOfNextSun >= SIZE_OF_SUN_ARR) return;

                    // position sun near the sunflower
                    float x = plantArr[i][j].format.x + 22;
                    float y = plantArr[i][j].format.y - 5;

                    //add that sun to the array of suns
                    AddSunToArray(arr_of_suns, indexOfNextSun,lawn_array,i,j,groundOfTheSuns, (int)x, (int)y, 20, 20);

                }
            }
        }
    }
}

//PLANT/PEASHOOTER----------------------------------------

//addPeaToArr: Given the array of peas, the x and y coordinates of that pea, add a pea at the end of the array
void addPeaToArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],PeaShot pea, int *indexOfNextPea){
    if (*indexOfNextPea < 0 || *indexOfNextPea > SIZE_OF_PEASHOT_ARR) return; //if valid index
    peaShotsArr[*indexOfNextPea] = pea; //add pea
    (*indexOfNextPea)+=1; //update the index
}

//shootPea: Given the array of plants and the array of peas, first checks if its time to shoot a pea. If so, shoot with the proprieties combining with the peashooter and add 
//          that pea to the array of peas
void shootPea(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int *indexOfNextPea){
    for(int i=0; i<NUMBER_ROWS_LAWN; i++)
    {
        for(int j=0; j<NUMBER_COLUMN_LAWN; j++)
        {
            // if the plant is a green peashooter and is not a empty (nulled) plant
            if (plantArr[i][j].type == TYPE_GREEN_PEASHOOTER&&plantArr[i][j].format.x != NULL_PLANT.format.x)
            {
                // if it's time to shoot a pea
                if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime))
                {
                    // update reference time properly, to enable the tracking of the next time to shot a pea for that plant
                    UpdateReferenceTime(&plantArr[i][j]);

                    // shot the pea (accordingly to it's type, in that case, normal_green_peashooteer) at a position near the green peashooter
                    float x = plantArr[i][j].format.x + 5;
                    float y = plantArr[i][j].format.y + (LAWN_HEIGHT_VALUE/2);
                    PeaShot pea = NORMAL_GREEN_PEASHOT;
                    pea.format.x=x;
                    pea.format.y=y;
                    pea.rowOfShot=i;

                    //add that pea to the arr
                    addPeaToArr(peaShotsArr,pea,indexOfNextPea);
                }
            }
        }
    }
}

//DrawPeShots: Draw all Peas of array of peas until the last element
void DrawPeaShots (PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],int indexOfNextPea){
    for(int i=0;i<indexOfNextPea;i++)
    {
        DrawRectangleRec(peaShotsArr[i].format, peaShotsArr[i].color);
    }
}

//UpdatePeaShotPosition: Update, according to its velocity, the position of a pea
void UpdatePeaShotPosition(PeaShot *pea){
        pea->format.x+=pea->velocity;
}

//RemovePeaFromArr: Remove a certain pea from the array
void RemovePeaFromArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfPeaToBeRemoved, int *indexOfNextPea) {
    if (indexOfPeaToBeRemoved < 0 || indexOfPeaToBeRemoved >= *indexOfNextPea) return; // invalid index

    // calculate how many items must be moved
    int numToMove = *indexOfNextPea - indexOfPeaToBeRemoved - 1;

    if (numToMove > 0) {
        memmove(
            &peaShotsArr[indexOfPeaToBeRemoved],          // dest
            &peaShotsArr[indexOfPeaToBeRemoved + 1],      // origin
            numToMove * sizeof(PeaShot)                   // quantity of bytes
        );
    }

    (*indexOfNextPea)--; // decrement the index of next pea
}

//ZOMBIES----


//DrawZombie: Draw a given zombie
void DrawZombie(Zombie zombie){
    DrawRectangleRec(zombie.format, zombie.color);
}

//DrawZombieArr: Given the array of zombies and the indexOfNextZombies, draw them in the scren
void DrawZombieArr(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int indexOfNextZombie){
    for(int i=0;i<indexOfNextZombie;i++){
        DrawZombie(zombiesArr[i]);
    }
}

//AddZombiesToZombiesArr: Add a given zombie to the array of zombies
void AddZombieToZombiesArr(Zombie zombie, Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie){
    //checking if the zombies in the screen haven't passed the max of zombies
    if((*indexOfNextZombie)>SIZE_OF_ZOMBIES_ARR) return;
    //checking if the zombie isn't nulled
    if(zombie.format.x<NULL_ZOMBIE.format.x||
       zombie.format.y<NULL_ZOMBIE.format.y||
       zombie.health<NULL_ZOMBIE.health||zombie.velocity<NULL_ZOMBIE.velocity||
       zombie.rowOfZombie<NULL_ZOMBIE.rowOfZombie) return ;
    
    //adding the zombie created to the array
    zombiesArr[*indexOfNextZombie]=zombie;
    //updating the count of zombies
    (*indexOfNextZombie)+=1;
}

//AddZombiesToZomviesArrRandomly: Add a random zombie at a random position to the array
void AddZombieToZombiesArrRandomly( Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie,Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]){
    Zombie zombie = NORMAL_ZOMBIE;
    //generating zombie's row randomly
    int row = rand() % (NUMBER_ROWS_LAWN);
    zombie.rowOfZombie=row;
    //as the y coordinate of the lawns of each row are equal for the row,
    //we can use whichever column index we want.
    zombie.format.y = lawn_array[row][0].y ;
    //NOTICE: WE DO NOT CHANGE THE X COORDINATE, WE'RE ONLY INTERESTED
    //INT THE Y COORDINATE TO CHOSE THE ROW.
    AddZombieToZombiesArr(zombie, zombiesArr,indexOfNextZombie);
}

//UpdateZombiePosition: Given a zombie, update the position of that zombie according to its velocity
void UpdateZombiePosition(Zombie *zombie){
    //as zombie x coordinate starts at the right of the screen
    //we'll decrease its position by its velocity
    zombie->format.x-=zombie->velocity;
}

// RemoveZombie: Given the array of zombies, the indexOfNextZombie and the index of the zombie to be removed
//removes the zombie at that certain index and update the indexOfNextZombie
void RemoveZombie(Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR],int *indexOfNextZombie,int indexOfZombieToBeRemoved){
    if (indexOfZombieToBeRemoved < 0 ||indexOfZombieToBeRemoved >= *indexOfNextZombie) return; // invalid index

    // calculate how many items must be moved
    int numToMove = *indexOfNextZombie -indexOfZombieToBeRemoved - 1;

    if (numToMove > 0) {
        memmove(
            &zombiesArr[indexOfZombieToBeRemoved],          // dest
            &zombiesArr[indexOfZombieToBeRemoved + 1],      // origin
            numToMove * sizeof(Zombie)                   // quantity of bytes
        );
    }

    (*indexOfNextZombie)--; // decrement the index of next zombie
}

//UpdateZombieHealth:Given the zombie and the damage, update the zombie health
void UpdateZombieHealth(Zombie *zombie, int damage){
    zombie->health-=damage;
}

//PLANT/ZOMBIES/PEAS COLISIONS

//verifyPeaColisionWithZombie: given a Zombie and a pea, checks if they collided
bool verifyPeaShotColisionWithZombie(PeaShot pea, Zombie zombie){
    //if they are in the same row
 if(zombie.rowOfZombie==pea.rowOfShot){
        // if 
        //x coord of the pea is beetwen the:
        //x coord of the zombie && x coord of the zombie + its width 
        //&&
        //y coord of the pea is beetweern the:
        //y coord of the zombie && y coord of the zomvie + its height
        //they colided, return true
        if(((pea.format.x>=zombie.format.x) && (pea.format.x<=(zombie.format.x+zombie.format.width)))
            &&
            ((pea.format.y>=zombie.format.y) && (pea.format.y<=(zombie.format.y+zombie.format.height)))
        ){
            return true;
        }
    }
  
    return false;
}

//verifyPlantColisionWithZombie:given a Zombie and a plant, checks if they collided
bool verifyPlantColisionWithZombie(Plant plant, Zombie zombie){
    //if they are in the same row
 if(zombie.rowOfZombie==plant.rowOfPlant){
        // if 
        //x coord of the zombie is beetwen the:
        //x coord of the plant && x coord of the plant + its width 
        //&&
        
        //they colided, return true
        if(((zombie.format.x>=plant.format.x) && (zombie.format.x<=(plant.format.width+plant.format.x)))){
            return true;
        }
    }
    //else return false
    return false;
}

//----------

//updatePlantsAndZombiesGameplay: Update all projectiles thrown and also manages the 
//hits of each projectile in each zombie, as well as deals proprely with the zombies health
//if zombie has gone out of the screen, it will return true, else, false
int updatePlantsAndZombiesGameplay(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
                                 PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],
                                 Zombie zombieArr[SIZE_OF_ZOMBIES_ARR],
                                 bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],
                                 int *indexOfNextPea,
                                 int *indexOfNextZombie)
{
    // SHOOT NEW PEAS
    shootPea(plantArr, peaShotsArr, indexOfNextPea);

    // 1. UPDATE PEA POSITIONS

    for (int j = 0; j < *indexOfNextPea; j++) {
        UpdatePeaShotPosition(&peaShotsArr[j]);
    }

    for (int i = 0; i < *indexOfNextZombie; i++) 
    {
        //verifies if zombie has already died
        if (zombieArr[i].health <= 0) 
        {
                //if he has died, then i don't need to check the other things, jump to the next iteration
                RemoveZombie(zombieArr, indexOfNextZombie, i);
                i--; // Updates the index correctly
                continue;//jumping to the next iteration
        }

        //verifies if zombie has gone out of the screen, return true
            if (zombieArr[i].format.x < -10){
                return 1;
            } 

        // 2. CHECK COLLISIONS WITH PEA AND UPDATE ZOMBIE HEALTH

        for (int j = 0; j < *indexOfNextPea; j++) 
        {
                if (verifyPeaShotColisionWithZombie(peaShotsArr[j], zombieArr[i])) 
                {//if zombie was shot, update its health and remove that pea that shot him
                        UpdateZombieHealth(&zombieArr[i], peaShotsArr[j].damage);
                        RemovePeaFromArr(peaShotsArr,j,indexOfNextPea);
                        j--;        //as pea was removed, need to update the index of iteration of peas
                }

                //if the position of the pea passes the width of the screen, remove it from the array
                if(peaShotsArr[j].format.x>SCREEN_WIDTH)
                {
                        RemovePeaFromArr(peaShotsArr,j,indexOfNextPea);
                        j--;
                }
        }

        //3 CHECK ZOMBIE COLISION WITH PLANT AND UPDATE VARIABLES

        zombieArr[i].isAttacking=0;
        for(int r=0;r<NUMBER_ROWS_LAWN;r++)
        {
            for(int c=0;c<NUMBER_COLUMN_LAWN;c++)
            {
                //if the plant died, remove it from the screen
                if(plantArr[r][c].health<=0)
                {
                        RemovePlantFromArr(plantArr,occupationOfLawn,r,c);
                }

                //if a colision zombie/plant is happening
                if(verifyPlantColisionWithZombie(plantArr[r][c], zombieArr[i]))
                {
                        //update isAttacking propriety
                        //DONT UPDATE THE POSITION OF THE ZOMBIE IF HE'S ATTACKING
                        //Update healthOfPlant according to the damage that the zombie gives per frame
                        UpdateHealthOfPlant(&plantArr[r][c],zombieArr[i].damage);
                        zombieArr[i].isAttacking=1;
    
                }
                else
                {
                        if(zombieArr[i].isAttacking);
                }
            }
        }
    
        //update the zombie's position if he's not attakcing
        if(!zombieArr[i].isAttacking){
            UpdateZombiePosition(&zombieArr[i]);
        }
    }
    return 0;
}

//----------------------------------

//GAMING DECK----------------

//DrawMoldureOfSelectedCard:
//given a plant that is selected in the deck of cards, dray a moldure for it. If there isn't plant selected, return void
void DrawMoldureOfSelectedCard(Plant cardSelected){
    //if the cardSelected is nulled, return void, because there's no card selected
    if(cardSelected.format.x==0) return;
    //else
    DrawRectangleLinesEx(cardSelected.format,2.5f, DARKGREEN);
}

//DrawGamingDeck:
//given the deck of plants, the quantity of suns adn the card selected, draw the interface, checking if one card is being hovered and highlightening it, and updating the card selected(if needed)
void DrawGamingDeck(Plant DeckOfPlants [SIZE_OF_DECK], unsigned int quantityOfSun, Plant *cardSelected){
    //Drawing the sun counter
    int DECK_RECTANGLE_X_VALUECpy = DECK_RECTANGLE_X_VALUE;
    //Drawing the rectangle that subscribes the sun counter
    DrawRectangle(DECK_RECTANGLE_X_VALUE,DECK_RECTANGLE_Y_VALUE,DECK_ELEMENT_WIDTH_VALUE,DECK_ELEMENT_HEIGHT_VALUE,BROWN);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE,DECK_RECTANGLE_Y_VALUE,DECK_ELEMENT_WIDTH_VALUE,DECK_ELEMENT_HEIGHT_VALUE,BLACK);
    DrawRectangle(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+5,DECK_ELEMENT_WIDTH_VALUE-10,DECK_ELEMENT_HEIGHT_VALUE-10,DARKBROWN);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+5,DECK_ELEMENT_WIDTH_VALUE-10,DECK_ELEMENT_HEIGHT_VALUE-10,BLACK);
    DrawRectangle(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+DECK_ELEMENT_HEIGHT_VALUE-20,DECK_ELEMENT_WIDTH_VALUE-10,30,RAYWHITE);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+DECK_ELEMENT_HEIGHT_VALUE-20,DECK_ELEMENT_WIDTH_VALUE-10,30,BLACK);
    DrawText(TextFormat(" %d", quantityOfSun),DECK_RECTANGLE_X_VALUE+20,DECK_RECTANGLE_Y_VALUE+DECK_ELEMENT_HEIGHT_VALUE-20,20,BLACK);
    //Drawing the deck of plants
    for (int i=0;i<SIZE_OF_DECK;i++){
        DECK_RECTANGLE_X_VALUECpy+= DECK_ELEMENT_WIDTH_VALUE;
        Plant plantBoxOfCard ={
            .format={
                .x= DECK_RECTANGLE_X_VALUECpy,
                .y =DECK_RECTANGLE_Y_VALUE,
                .width= DECK_ELEMENT_WIDTH_VALUE,
                .height= DECK_ELEMENT_HEIGHT_VALUE
            },
            .color=DeckOfPlants[i].color,
            .cost=DeckOfPlants[i].cost,
            .type=DeckOfPlants[i].type,
            .existanceTime=0,
            .referenceTime=0,
            .actionTime = DeckOfPlants[i].actionTime,
            .creationTime = 0,
            .health=DeckOfPlants[i].health

        };
        DrawRectangleLines(DECK_RECTANGLE_X_VALUECpy,DECK_RECTANGLE_Y_VALUE,DECK_ELEMENT_WIDTH_VALUE,DECK_ELEMENT_HEIGHT_VALUE,BLACK);
        //if the box is being hovered,
        if (CheckCollisionPointRec(mousePoint, plantBoxOfCard.format)) {
            //highlight it
            DrawRectangleRec(plantBoxOfCard.format,GRAY);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)||IsGestureDetected(KEY_ENTER)){
                *cardSelected=plantBoxOfCard;
                
            }
        }else{
            //else, draw it normaly
            DrawRectangleRec(plantBoxOfCard.format,MAROON);
        }
        //Drawing the plants within the boxes
        DrawRectangle(DECK_RECTANGLE_X_VALUECpy+10,DECK_RECTANGLE_Y_VALUE+20,20,20,DeckOfPlants[i].color);
    }
}

// RemoveSelectedCard: 
// Checks if a selected card should be removed and removes it. A selected card is removed if 'W' or the right mouse button is pressed.
void RemoveSelectedCard(Plant *cardSelected) {
    if (IsKeyPressed(KEY_W) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        cardSelected->format.height = 0;
        cardSelected->format.width = 0;
        cardSelected->format.x = 0;  
        cardSelected->format.y = 0;
    }
}

//PutPlantToField:Plant[lawnRow*lawnColumns], Plant, int*, bool[lawnRows][lawnColumns],->void
//Given the array of plants in *the field,the card selected , the *sunStorage, the occupationLawn
//checks if plant can be put and properly put it
void PutPlantToField
(Plant plantArr [NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], 
    Plant *cardSelected, unsigned int *sunStorage,bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN])
    {
        //checks if there's a lawn being hovered
        bool isHovered =0;
        
        //row,column
        int r=0, c=0;
        for(int i=0;i<NUMBER_ROWS_LAWN;i++){
            for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                    isHovered =1;
                    r=i;
                    c=j;
                }   
            }
        }
        //  a plant is selected && check-colision with a block of lawn(for loop,checks if an element has lawnRecHover [i][j]=1,save that i and j) 
        //&& eventClick tapping
        if(isHovered&&cardSelected->format.x!=0&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            //          if the amount of suns is sufficient to put a plant in the field  &&
            //          the position chosen is free to be used (checks if occupationLawn[i][j]!=1)
            if(*sunStorage>=cardSelected->cost&&occupationOfLawn[r][c]!=1){
            //my selected card, by default, has, already, color, type, cost,height and width. But i doesn't have a proper x and y to be displayed on the lawn
            //              *add that plant to the array of plants
            Plant plant = *cardSelected;
            plant.format.x=lawnRectangles[r][c].x;
            plant.format.y=lawnRectangles[r][c].y;
            plant.creationTime=GetTime();
            plant.rowOfPlant = r;
            plantArr[r][c]=plant;
            //              *discount that amount of the sunStorage
            *sunStorage-=cardSelected->cost;
            //              *update the occupationLawn
            occupationOfLawn[r][c]=1;
            //              *reset the selectedCard
            cardSelected->format.x = 0;
            cardSelected->format.y = 0;
            cardSelected->format.width = 0;
            cardSelected->format.height = 0;

        }
    }


}
//--------------------------------

// ================================================================================================================================================================================================================================================
//MAIN================================================================================================================================================================================================================================================
//================================================================================================================================================================================================================================================


int main (void){
    //INITIALIZATION
//----------------------------------------------------------------------------------------------------------------------------------------------


//--screen
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,"raylib [core] example - basic screen manager");
    GAME_SCREEN currentScreen = LOGO;
    GAME_SCREEN previousScreen = HOMEPAGE;
//--------


//--textures



//----------------------------------

//--images



//----------------------------------



//--HomePage

    Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    GAME_SCREEN homePageOptions[HOME_PAGE_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
        //Filling the homePageOptions
        homePageOptions[0]= PLAY;
        homePageOptions[1]= LEADERBOARD;
        homePageOptions[2]= ABOUT;
        homePageOptions[3]= CONFIGURATIONS;
        homePageOptions[4]= EXIT;

//margin from title
const int marginFromTitle=110
        //Filling the homePageOptionsRec 
        for (int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
            homePageOptionsRec[i].height = BTN_HEIGHT;
            homePageOptionsRec[i].width = BTN_WIDTH;
            homePageOptionsRec[i].x=BTN_CENTERED_X;
            homePageOptionsRec[i].y = marginFromTitle+((SCREEN_HEIGHT-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY)*i;
        }

//---------



//--Gaming Menu
    Rectangle gamingMenuOptionsRec[GAMING_MENU_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool gamingMenuOptionsRecHover[GAMING_MENU_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    GAME_SCREEN gamingMenuOptions[GAMING_MENU_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
        //Filling the homePageOptions
        gamingMenuOptions[0]= RESUME;
        gamingMenuOptions[1]= CONFIGURATIONS;
        gamingMenuOptions[2]= EXIT;

        //Filling the gamingMenuOptionsRec 
        for (int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
            gamingMenuOptionsRec[i].height = BTN_HEIGHT;
            gamingMenuOptionsRec[i].width = BTN_WIDTH;

gamingMenuOptionsRec[i].x=BTN_X-CENTERED;

gamingMenuOptionsRec[i].y= marginFromTitle+((SCREEN_HEIGHT-marginFromTitle)/GAMING_MENU_OPTIONS_QUANTITY)*i;
        }

//---------------------------


//--exiting application

    SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works
    bool exitWindow = false;    // Flag to set window to exit
//---------


//--frameManagement
    int framesCounter = 0;          // Useful to count frames
    SetTargetFPS(TARGET_FPS);               // Set desired framerate (frames-per-second)
//--------------------------------



//--PLAYER 

    char playerName[MAX_SIZE_OF_NAME]={0};

    unsigned int sizeOfName = 0;//variable used to track the size of the name of the user

//-----------------


//DECK-----
    Plant DeckOfPlants [SIZE_OF_DECK]={0};
    //DO A FUNCTION INITDECK!
    DeckOfPlants[0] = PLANT_SUNFLOWER;
    DeckOfPlants[1] = PLANT_GREEN_PEASHOOTER;

    //used to track which card is selected. If card is all nulled, then there's no card selected
    Plant cardSelected = {0};

//---------------------------------------------

//PLANT ---------------------

    //used to track which plants are deployed in the field(lawn)
    Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]={0};

//----------------------



//LAWN--------------
//lawns of the game
    //used to verify if a plant already exists in a spot
    bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]={0};
    bool lawnRectanglesHover[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
    Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN];
        for(int i=0;i<NUMBER_ROWS_LAWN;i++){
            for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                lawnRectangles[i][j].x = 30+LAWN_X_VALUE*j;
                lawnRectangles[i][j].y = 80+LAWN_Y_VALUE*i;
                lawnRectangles[i][j].width = LAWN_WIDTH_VALUE;
                lawnRectangles[i][j].height = LAWN_HEIGHT_VALUE;
            }
        }
//-------------------------------------



//SUN----------------------

    double timOfLastSun = GetTime();
    //array to track the suns, if the x and y coordinates are "-1", then we consider it an empty sun
    Rectangle sunArray[SIZE_OF_SUN_ARR]={0};
    //array to track the quantity of sun that the player has
    unsigned int sunGamingStorage=0;
    //array to track when a sun hits the ground
    float groundOfTheSuns[SIZE_OF_SUN_ARR]={0};
    //indexToTrack the end of the array
    int indexOfNextSun = 0;
    //time of spawn of suns = 15s
    double spawnRateSun = 8.0;   
    //initializing the size of all suns
    for (int i=0;i<SIZE_OF_SUN_ARR;i++){
        sunArray[i].height = 20;
        sunArray[i].width = 20;
    }


//------------------------


//PEASHOOTER AND PEASHOT


    PeaShot peaShotsArr [SIZE_OF_PEASHOT_ARR];
    int indexOfNextPea = 0;

//---------------------


//ZOMBIE-----------------


    Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]={0};
    int indexOfNextZombie=0;
    double spawnRateZombie = 30.0;
    bool firstZombieSpawn =0;
    double timeOfLastZombie = GetTime();  //saves the actualTime
    double timeForFirstSpawnZombie=45.0;


//------------------------



// ====================================================================================================================================================================================================
//MAIN LOOP GAME====================================================================================================================================================================================
// ====================================================================================================================================================================================================
    


    while (!exitWindow)    // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();//tracking, effectively, the user's mouse
        
        //if user wants to end applications, redirect him to an exit confirmation
        if(WindowShouldClose()){
            currentScreen=  EXIT;
        }

        //Switch that UPDATES THE VARIABLES
        switch (currentScreen)
        {


            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                
                framesCounter++;    // Count frames

                // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
                if (framesCounter > 2)
                {
                    currentScreen = HOMEPAGE;
                }
            } break;


            case HOMEPAGE:
            {
                // TODO: Update HOMEPAGE screen variables here!
                //checks if an rectangle(option) is hovered, so that we can highlight that ractangle
                 for (int i = 0; i < HOME_PAGE_OPTIONS_QUANTITY; i++)
        {
            if (CheckCollisionPointRec(mousePoint, homePageOptionsRec[i])) {
                homePageOptionsRecHover[i] = 1;
                if(IsGestureDetected(GESTURE_TAP)){
                    previousScreen=currentScreen;
                    currentScreen = homePageOptions[i];
                }
            }
            else homePageOptionsRecHover[i] = 0;
        }
            } break;


            case PLAY:
            {
                // TODO: Update PLAY screen variables here!
                currentScreen = USER_DATA;//first thing to be done, is to ask for the User data          
            } break;



              case USER_DATA:
            {
                // TODO: Update USERDATA screen variables here!
                previousScreen=currentScreen;

                // Get char pressed (unicode character) on the queue
                int key = GetCharPressed();
                
                // Check if more characters have been pressed on the same frame
                while (key > 0) {
                    // NOTE: Only allow keys in range [32..125]
                    //MAX_SIZE_OF_NAME-1 to garantee the space for '\0'
                    if ((key >= 32) && (key <= 125) && (sizeOfName < MAX_SIZE_OF_NAME-1)) {
                        playerName[sizeOfName] = (char)key;
                        sizeOfName++;
                    }
                    key = GetCharPressed();  // Check next character in the queue
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (sizeOfName == 0){
                        sizeOfName = 0;
                    }else{
                        sizeOfName--;
                    }
                    playerName[sizeOfName] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && sizeOfName > 0) {
                    currentScreen = GAMEPLAY; 
                }
            } break;
            


            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!


                previousScreen=currentScreen;

                //if zombie has gone out of the screen
                if(updatePlantsAndZombiesGameplay(plantArr,peaShotsArr,zombieArr,occupationOfLawn,&indexOfNextPea,&indexOfNextZombie)){
                    currentScreen=END_GAME;
                }

                UpdateExistanceTime(plantArr);

                updateSunsPosition(sunArray,indexOfNextSun,groundOfTheSuns);

                for(int i=0;i<NUMBER_ROWS_LAWN;i++){
                    for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                        if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                            lawnRectanglesHover[i][j] = 1;
                        }   
                        else lawnRectanglesHover[i][j] = 0;
                    }
                }

                //used to spawn sun appropriately
                double timeSpawnSunTracking =GetTime();

                //used to spawn zombies appropriately
                double timeSpawnZombieTracking =GetTime();

                //spawn of the suns
                if((timeSpawnSunTracking-timOfLastSun>spawnRateSun)&&indexOfNextSun<SIZE_OF_SUN_ARR){
                    AddRandomlySunToArr(sunArray, &indexOfNextSun,lawnRectangles,groundOfTheSuns);
                    timOfLastSun=GetTime();
                }

                //wait more for the first zombie spawn
                if(firstZombieSpawn){
                    if((timeSpawnZombieTracking-timeOfLastZombie>timeForFirstSpawnZombie)){
                        AddZombieToZombiesArrRandomly(zombieArr,&indexOfNextZombie,lawnRectangles);
                        timeOfLastZombie=GetTime();
                    }
                
                //normal zombie spawn
                }else{
                    if((timeSpawnZombieTracking-timeOfLastZombie>spawnRateZombie)&&indexOfNextZombie<SIZE_OF_ZOMBIES_ARR){
                        AddZombieToZombiesArrRandomly(zombieArr,&indexOfNextZombie,lawnRectangles);
                        timeOfLastZombie=GetTime();
                    }
                }


                if(collectSun(sunArray,&indexOfNextSun,groundOfTheSuns)){
                    addSunToStorage(&sunGamingStorage);
                }


                PutPlantToField(plantArr,&cardSelected,&sunGamingStorage,occupationOfLawn,lawnRectangles);


                GenerateSunSunflower(plantArr,lawnRectangles,groundOfTheSuns,sunArray,&indexOfNextSun);

                //Menu, if esc pressed
                if(IsKeyPressed(KEY_ESCAPE)){
                    currentScreen = MENU;
                }
                
            } break;

           //CASE ENDGAME?


            case MENU:{

                for (int i = 0; i < GAMING_MENU_OPTIONS_QUANTITY; i++)
                {
                    if (CheckCollisionPointRec(mousePoint, gamingMenuOptionsRec[i])) {
                        gamingMenuOptionsRecHover[i] = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            previousScreen=currentScreen;
                            currentScreen = gamingMenuOptions[i];
                        }
                    }
                    else gamingMenuOptionsRecHover[i] = 0;
                }
            }break;


            case RESUME:{
                currentScreen = GAMEPLAY;
            }break;


            case ABOUT:
            {
                // TODO: Update ENDING screen variables here
            } break;


             case CONFIGURATIONS:
            {
                // TODO: Update ENDING screen variables here!
            } break;


             case EXIT:
            {
            //Confirming exit                
               
            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            if (IsKeyPressed(KEY_Y)) exitWindow = true;
            else if (IsKeyPressed(KEY_N)) {
                currentScreen=previousScreen;
            };
        
            } break;


            default: break;
        }
       

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawText("Pedrokas's gaming enterprise® \n\t\t\t\t\t\t\t\tpresents... ", 70, 150, 40, DARKGREEN);
                   

                } break;
                case HOMEPAGE:
                {
                    // TODO: Draw HOMEPAGE screen here!

                    //Background
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);

                    //homePageOptions
                    for(int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(homePageOptionsRec[i], WHITE);
                         DrawRectangleLines(homePageOptionsRec[i].x, homePageOptionsRec[i].y, homePageOptionsRec[i].width, homePageOptionsRec[i].height, BLACK);
                        //tracking hover over the options
                        if(homePageOptionsRecHover[i]==true){
                            DrawRectangleRec(homePageOptionsRec[i], RED);
                        }
                    }
                    DrawText("INF vs ZOMBIES", (SCREEN_WIDTH-MeasureText("INF vs ZOMBIES",40))/2, 20, 40, DARKGREEN);
                    DrawText("PLAY", BTN_CENTERED_X, 120, 20, DARKGREEN);
                    DrawText("LEADERBOARD", BTN_CENTERED_X, 180, 20, DARKGREEN);
                    DrawText("ABOUT", BTN_CENTERED_X, 240, 20, DARKGREEN);
                    DrawText("CONFIGURATIONS", BTN_CENTERED_X, 300, 20, DARKGREEN);
                    DrawText("EXIT", BTN_CENTERED_X, 360, 20, DARKGREEN);

                } break;
                case USER_DATA:
                {
                    // TODO: Draw USERDATA screen here!
                  
                    //Draw a fixed rectangle that fits 8 words
                    Rectangle textBox = { SCREEN_WIDTH/2 - 150, 180,  MeasureText("A",40)*8+30, 50 };
                    //if username more than 8 words, update the box
                    if(sizeOfName>8){
                        textBox.width = sizeOfName*MeasureText("A",40)+10;
                    }
                    
                    //Title
                    DrawText("Tell me your name!", 240, 140, 20, GRAY);

                    //TextInput
                    DrawRectangleRec(textBox, LIGHTGRAY);
                    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
                    DrawText(playerName, textBox.x + 5, textBox.y + 8, 40, MAROON);

                    //Text alone
                    DrawText(TextFormat("Press enter to confirm: %i/%i", sizeOfName, MAX_SIZE_OF_NAME-1), 315, 250, 20, DARKGRAY);


                    // Draw blinking underscore char
                    if (sizeOfName < MAX_SIZE_OF_NAME) {
                     DrawText("_",( textBox.x + 8 + MeasureText(playerName, 40)),(textBox.y + 12), 40, MAROON);
                    } 
                  

                } break;


                 case DECK_SELECTION:
                {
                    
                    // // TODO: Draw ENDING screen here!
                    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
                    // DrawText(TextFormat("Nice to meet you, %s :)", playerName), 20, 20, 40, DARKBLUE);
                    // DrawText("Select you deck", 120, 220, 20, DARKBLUE);
                    currentScreen = GAMEPLAY;

                } break;


                case GAMEPLAY:
                {
                    //TODO:Draw GAMEPLAY screen here!

                    //Lawn drawing
                    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
                        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                            //if "i" is odd, if "j" is odd, darkgreen, else, lightgreen
                            if(i&1){
                                if(j&1){
                                    DrawRectangleRec( lawnRectangles[i][j], DARKGREEN);
                                }else{
                                    DrawRectangleRec( lawnRectangles[i][j], GREEN);
                                }
                            }else{
                                if(j&1){
                                    DrawRectangleRec( lawnRectangles[i][j], GREEN);
                                }else{
                                    DrawRectangleRec( lawnRectangles[i][j], DARKGREEN);
                                }
                            }
                            //tracking hover over the options
                            if(lawnRectanglesHover[i][j]==true){
                                DrawRectangleLines( lawnRectangles[i][j].x,  lawnRectangles[i][j].y,  lawnRectangles[i][j].width,  lawnRectangles[i][j].height, BLACK);
                            }
                        }
                    }

                    //Plants
                    DrawPlants(plantArr);

                    //Peashots
                    DrawPeaShots(peaShotsArr,indexOfNextPea);

                    //Zombies
                    DrawZombieArr(zombieArr,indexOfNextZombie);

                    //Suns
                    DrawSuns(sunArray,indexOfNextSun);

                    //Gaming deck
                    DrawGamingDeck(DeckOfPlants,sunGamingStorage, &cardSelected);
                    DrawMoldureOfSelectedCard(cardSelected);
                    RemoveSelectedCard(&cardSelected);

                }break;


                //  case END_GAME:{
                //      
                //      DrawText("YOU LOST!", (SCREEN_WIDTH-50)/2, 20, 50, DARKGREEN);
                //     DrawRectangle((SCREEN_WIDTH-50)/2+,, 20,, WHITE);
                //     if(gamingMenuOptionsRecHover[i]==true){
                //              DrawRectangleRec(gamingMenuOptionsRec[i], RED);
                //             }
                //         }break;


                case MENU:{
                    
                    //Drawing Menu options
                    for(int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(gamingMenuOptionsRec[i], WHITE);
                         DrawRectangleLines(gamingMenuOptionsRec[i].x, gamingMenuOptionsRec[i].y, gamingMenuOptionsRec[i].width, gamingMenuOptionsRec[i].height, BLACK);
                         //tracking hover over the options
                         if(gamingMenuOptionsRecHover[i]==true){
                             DrawRectangleRec(gamingMenuOptionsRec[i], RED);
                            }
                        }
                        DrawText("INF vs ZOMBIES",  (SCREEN_WIDTH-MeasureText("INF vs ZOMBIES",40))/2, 20, 40, DARKGREEN);
                        DrawText("RESUME", BTN_CENTERED_X, 120, 20, DARKGREEN);
                        DrawText("CONFIGURATIONS", BTN_CENTERED_X, 180, 20, DARKGREEN);
                        DrawText("EXIT", BTN_CENTERED_X, 240, 20, DARKGREEN);


                }break;
                case ABOUT:
                {
                    
                    // TODO: Draw ABOUT screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
                    DrawText("This is the about screen, you can quit the program now!", 20, 20, 40, DARKBLUE);
                  

                } break;


                 case CONFIGURATIONS:
                {
                    
                    // TODO: Draw CONFIGURATIONS screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
                    DrawText("Configurations screen, you can quit the program now!", 20, 20, 40, DARKBLUE);

                } break;


                 case EXIT:
                {
                    
                    // TODO: Draw EXIT screen here!
                DrawRectangle(0, 100, SCREEN_WIDTH, 200, BLACK);
                DrawText("Are you sure you want to exit program :( ? [Y/N]", 40, 180, 30, WHITE);
            
                } break;
                default: break;
            }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // TODO: Unload all loaded data (textures, fonts, audio) here!


    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;

}