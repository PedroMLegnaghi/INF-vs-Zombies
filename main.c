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
#define CONFIGURATIONS_OPTIONS_QUANTITY 13//quantity of options in the configurations menu, 6 options + 6 corresponding rectangles mute/unmute +1 Back to menu option
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define TARGET_FPS 60
#define NUMBER_COLUMN_LAWN 9
#define NUMBER_ROWS_LAWN 5
#define VALUE_OF_EACH_SUN 25
#define SUN_HEIGHT 30
#define SUN_WIDTH 30

//Sizes of arrays
#define SIZE_OF_SUN_ARR 100 //maximum quantity of suns in screen
#define SIZE_OF_DECK 3+1    //maximum size of deck (3 plants + 1 shovel) 
#define SIZE_OF_ZOMBIES_ARR 100 //maximum quantity of zombies in screen
#define SIZE_OF_PEASHOT_ARR 1000 //maximum quantity of peashots in screen

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

//ScreenRectangle(used to display textures)
const Rectangle SCREEN_RECTANGLE={
    .height=SCREEN_HEIGHT,
    .width=SCREEN_WIDTH,
    .x=0,
    .y=0
};

// ================================================================================================================================================
// ================================================================================================================================================
// ================================================================================================================================================

//================================================================================================================================================
//ENUM=====================================================================================================================================
//================================================================================================================================================


//enumaration to reference to the gameScreens of the game
typedef enum GAME_SCREEN {LOGO = 0, HOMEPAGE, PLAY, LEADERBOARD, ABOUT, CONFIGURATIONS, EXIT 
    ,USER_DATA, DECK_SELECTION, GAMEPLAY,END_GAME,MENU, RESUME} GAME_SCREEN;
 
//enumeration to reference the options of the configurations menu
typedef enum{
    CONFIGURATIONS_BTN_ZOMBIES=0,
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
    CONFIGURATIONS_BTN_GOBACK} CONFIGURATIONS_OPTIONS;

//enumeration to reference the cost of each plant.
//model: COST_(type_of_plant)
typedef enum COST_OF_PLANT{
    COST_SUNFLOWER = 50,
    COST_PEASHOOTER = 100,
    COST_WALLNUT = 75
    
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
    TYPE_WALLNUT,
    TYPE_SHOVEL,
    TYPE_NULL_PLANT
}TYPE_OF_PLANT;

//enumaration to reference the HEALTH_OF_PLANT
//model: HEALTH_OF_(type_of_plant)
typedef enum {
    HEALTH_OF_SUNFLOWER = 100,
    HEALTH_OF_GREEN_PEASHOOTER = 100,
    HEALTH_OF_WALLNUT = 1000
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
    Texture2D texture;
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
    Texture2D texture;
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
    Texture2D texture;
    PeaShot peashot;
    //sound?
    
}Plant;

//----------------------------
//================================================================================================================================================
//================================================================================================================================================
//================================================================================================================================================


//================================================================================================================================================
//STRUCT CONSTANTS=====================================================================================================================================
//================================================================================================================================================

//PLANT
const Plant NULL_PLANT ={.actionTime=0,.color=0,.cost=0,.creationTime=0,.existanceTime=0,.format=0,.health=0,.peashot=0,.referenceTime=0,.rowOfPlant=0,.texture=0,.type=TYPE_NULL_PLANT};


//------------------

//PEASHOT
const PeaShot NULL_PEA={0};

//----------------

//ZOMBIE

const Zombie NULL_ZOMBIE={0};
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
void AddSunToArray(Rectangle array_of_suns[SIZE_OF_SUN_ARR],  int *indexOfNextSun,Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],int rowOfGround,int columnOfGround,float groundOfTheSuns[SIZE_OF_SUN_ARR], int x, int y) {
    array_of_suns[*indexOfNextSun].x = x;
        array_of_suns[*indexOfNextSun].y =  y;
        array_of_suns[*indexOfNextSun].width = SUN_WIDTH;
        array_of_suns[*indexOfNextSun].height = SUN_HEIGHT;
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

    AddSunToArray(array_of_suns,indexOfNextSun,lawn_array,row,column,groundOfTheSuns,lawn_array[row][column].x,-20);
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
void DrawSuns(Rectangle array_of_suns[SIZE_OF_SUN_ARR], int indexOfNextSun, Texture2D sunTexture){
    Vector2 origin ={0,0};
    Rectangle sunTextureSourceRectangle = {
        .height=sunTexture.height,
        .width = sunTexture.width,
        .x=0,
        .y=0
    };
    for(int i = 0; i < indexOfNextSun; i++){
        if (array_of_suns[i].x != -1 && array_of_suns[i].y != -1) { //if the sun is not an invalid sun
            // DrawRectangle(array_of_suns[i].x, array_of_suns[i].y,
            //             array_of_suns[i].width, array_of_suns[i].height, YELLOW);//draw it
            DrawTexturePro(sunTexture,sunTextureSourceRectangle,array_of_suns[i],origin,0.0f,WHITE);
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
    if(plantArr[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved].type!=TYPE_NULL_PLANT){
        plantArr[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved]=NULL_PLANT;//null the plant == remove
        occupationOfLawn[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved]=0;//update the occupation of the lawn
    }
}

//UpdateExistanceTime:
//given a Plant array and an int timeCorrection(when another event is actioned in the middle of the gameplay), 
//update the existance time of each plant without counting timeCorrection seconds, as not to count unvalid seconds(in menu, for example)
//if no timeCorrection needed, then timeCorrection==0
void UpdateExistanceTime(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],float timeCorrection){
    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
            if(plantArr[i][j].type!=TYPE_NULL_PLANT){
            plantArr[i][j].existanceTime=GetTime()-plantArr[i][j].creationTime-timeCorrection;
            }
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
            if(plantArr[i][j].type!=TYPE_NULL_PLANT){
                Vector2 origin = {0,0};
                Rectangle PlantTextureSourceRectangle = {
                    .width=plantArr[i][j].texture.width,
                    .height=plantArr[i][j].texture.height,
                    .y=0,
                    .x=0
                };
                 DrawTexturePro(plantArr[i][j].texture,PlantTextureSourceRectangle,plantArr[i][j].format,origin,0.0f,WHITE);
                // DrawRectangleRec(plantArr[i][j].format,plantArr[i][j].color );
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
                if(plantArr[i][j].type!=TYPE_NULL_PLANT){
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
                        AddSunToArray(arr_of_suns, indexOfNextSun,lawn_array,i,j,groundOfTheSuns, (int)x, (int)y);

                    }
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
void shootPea(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int *indexOfNextPea, Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]){
    for(int w=0;w<SIZE_OF_ZOMBIES_ARR;w++){
    for(int i=0; i<NUMBER_ROWS_LAWN; i++)
    {
        for(int j=0; j<NUMBER_COLUMN_LAWN; j++)
        {
            //if there's a zombie in the same row of the plant
                if((zombieArr[w].rowOfZombie==plantArr[i][j].rowOfPlant)&&zombieArr[w].format.x<=SCREEN_WIDTH-DISTANCE_LAWN_RIGHT_EDGE-DISTANCE_LAWN_RIGHT_EDGE){

                // if the plant is a green peashooter and is not a empty (nulled) plant
                if (plantArr[i][j].type == TYPE_GREEN_PEASHOOTER&&plantArr[i][j].format.x != NULL_PLANT.format.x)
                {
                    if(*indexOfNextPea<SIZE_OF_PEASHOT_ARR){
                        // if it's time to shoot a pea
                            if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime))
                            {
                                // update reference time properly, to enable the tracking of the next time to shot a pea for that plant
                                UpdateReferenceTime(&plantArr[i][j]);
                                
                                // shot the pea (accordingly to it's type, in that case, normal_green_peashooteer) at a position near the green peashooter
                                float x = plantArr[i][j].format.x+plantArr[i][j].format.width;
                                float y = plantArr[i][j].format.y+15;
                                PeaShot pea = plantArr[i][j].peashot;
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
        }
    }
}
    
//DrawPeShots: Draw all Peas of array of peas until the last element
void DrawPeaShots (PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],int indexOfNextPea){
    Vector2 origin ={0,0};
    for(int i=0;i<indexOfNextPea;i++)
    {
        Rectangle texture_source_rectangle = {
            .width = peaShotsArr[i].texture.width,
            .height = peaShotsArr[i].texture.height,
            .x=0,
            .y=0
        };
        DrawTexturePro(peaShotsArr[i].texture,texture_source_rectangle,peaShotsArr[i].format,origin,0.0f,WHITE);
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
    Vector2 origin ={0,0};
    Rectangle TEXTURE_ZOMBIE_IMG_SOURCE_REC = {.height=zombie.texture.height,.width=zombie.texture.width,.x=0,.y=0};
    
    DrawTexturePro(zombie.texture,TEXTURE_ZOMBIE_IMG_SOURCE_REC,zombie.format,origin,0.0f,WHITE);

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
void AddZombieToZombiesArrRandomly( Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR],Zombie zombieToBeGenerated, int *indexOfNextZombie,Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]){
    Zombie zombie = zombieToBeGenerated;
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

    if(plant.type!=TYPE_NULL_PLANT){
    
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
                                 int *indexOfNextZombie,
                                Sound peaImpactWithZombieSound,
                                Sound zombieAtePlant)
{
    // SHOOT NEW PEAS
    shootPea(plantArr, peaShotsArr, indexOfNextPea,zombieArr);

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
            if (zombieArr[i].format.x <= 0){
                return 1;
            } 

        // 2. CHECK COLLISIONS WITH PEA AND UPDATE ZOMBIE HEALTH

        for (int j = 0; j < *indexOfNextPea; j++) 
        {
                if (verifyPeaShotColisionWithZombie(peaShotsArr[j], zombieArr[i])) 
                {//if zombie was shot, update its health and remove that pea that shot him
                        PlaySound(peaImpactWithZombieSound);
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
                if(plantArr[r][c].health<=0&&plantArr[r][c].type!=TYPE_NULL_PLANT)
                {
                    PlaySound(zombieAtePlant);
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
    Vector2 origin = {0,0};
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

    

    //used to attribute keys to options
    int keyOfCard = KEY_ONE;

    //Drawing the deck of plants
    for (int i=0;i<SIZE_OF_DECK;i++){
        DECK_RECTANGLE_X_VALUECpy+= DECK_ELEMENT_WIDTH_VALUE;
        Plant plantBoxOfCard =DeckOfPlants[i];
        plantBoxOfCard.format.x= DECK_RECTANGLE_X_VALUECpy;
        plantBoxOfCard.format.y =DECK_RECTANGLE_Y_VALUE;
        plantBoxOfCard.format.width= DECK_ELEMENT_WIDTH_VALUE;
        plantBoxOfCard.format.height= DECK_ELEMENT_HEIGHT_VALUE;
        plantBoxOfCard.existanceTime=0;
        plantBoxOfCard.referenceTime=0;
        plantBoxOfCard.creationTime = 0;

        Rectangle textureSourceRectanglePlant = {
            .height=DeckOfPlants[i].texture.height,
            .width=DeckOfPlants[i].texture.width,
            .x=0,
            .y=0
        };
        DrawRectangleLines(DECK_RECTANGLE_X_VALUECpy,DECK_RECTANGLE_Y_VALUE,DECK_ELEMENT_WIDTH_VALUE,DECK_ELEMENT_HEIGHT_VALUE,BLACK);
        
        //user pressing number 1,2,3... selects the cards
        if(IsKeyPressed(keyOfCard)){
            *cardSelected=plantBoxOfCard;
        }
        
        //if the box is being hovered,
        if (CheckCollisionPointRec(mousePoint, plantBoxOfCard.format)) {
            
            //highlight it
            DrawRectangleRec(plantBoxOfCard.format,GRAY);

            //if its being hovered and also has been clicked, selected card = that plant
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)||IsKeyPressed(KEY_ENTER)){
                *cardSelected=plantBoxOfCard;
            }
        }else{
            //else, draw it normaly
            DrawRectangleRec(plantBoxOfCard.format,BROWN);
        }
        //Drawing the plants within the boxes
        DrawTexturePro(plantBoxOfCard.texture,textureSourceRectanglePlant,plantBoxOfCard.format,origin,0.0f,WHITE);

        //updating the key of the cards accordingly to its order in the InGameDeck
        keyOfCard++;
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
    Plant *cardSelected, unsigned int *sunStorage,bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],Rectangle lawnRectangles[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN],Sound soundOfPlantingPlant,Sound shovelSound)
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
            //if it is a shovel that is being selected, siimply just remove de plant in the spot selected
            if((*cardSelected).type==TYPE_SHOVEL){
                PlaySound(shovelSound);
                RemovePlantFromArr(plantArr,occupationOfLawn,r,c);
            }

            //     else if the amount of suns is sufficient to put a plant in the field  &&
            //          the position chosen is free to be used (checks if occupationLawn[i][j]!=1)
            else if(*sunStorage>=cardSelected->cost&&occupationOfLawn[r][c]!=1){
            //my selected card, by default, has, already, color, type, cost,height and width. But i doesn't have a proper x and y to be displayed on the lawn
            //              *add that plant to the array of plants
            PlaySound(soundOfPlantingPlant);
            Plant plant = *cardSelected;
            plant.format.x=lawnRectangles[r][c].x+(lawnRectangles[r][c].width-plant.format.width)/2;
            plant.format.y=lawnRectangles[r][c].y+(lawnRectangles[r][c].height-plant.format.height)/2;
            plant.creationTime=GetTime();
            plant.rowOfPlant = r;
            plantArr[r][c]=plant;
            //              *discount that amount of the sunStorage
            *sunStorage-=cardSelected->cost;
            //              *update the occupationLawn
            occupationOfLawn[r][c]=1;
            
        }
        //              *reset the selectedCard
        cardSelected->format.x = 0;
        cardSelected->format.y = 0;
        cardSelected->format.width = 0;
        cardSelected->format.height = 0;
    }
}
//--------------------------------
//about screen--
    void DrawAboutScreen() {
        const char *aboutText = 
            "About INF vs ZOMBIES\n\n"
            "INF vs ZOMBIES is a fun and strategic tower defense game inspired by Plants vs Zombies, "
            "developed as a project for the Algorithms and Programming course at the Federal University "
            "of Rio Grande do Sul (UFRGS).\n\n"
            "Players must defend their base from waves of zombies by placing different plants, each with "
            "unique abilities, along a grid. Strategy and quick thinking are key to surviving the "
            "endless undead hordes!\n\n"
            "Built from scratch in C using the Raylib library, this project was designed to recreate the "
            "fun of the original game while strengthening my C programming skills, problem-solving "
            "ability, and proactive development approach.\n\n"
            "Whether you're a fan of tower defense games or just curious about game development in C, "
            "INF vs ZOMBIES offers a mix of challenge and creativity, with a little programming twist!\n\n"
            "Ready to stop the zombie invasion? Let the battle begin!\n\n"
            "(Educational project for UFRGS - Instituto de Informatica)";

        
        // Text settings
        int titleFontSize = 30;
        int bodyFontSize = 20;
        int charsPerLine = 60; 
        //this multiplier stands for the spacing between the lines
        int lineHeight = bodyFontSize * 1.3f;
        int startY = 50;
        
        // Draw Background
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
        
        // Draw title (centered)
        const char *title = "About INF vs ZOMBIES";
        int titleWidth = MeasureText(title, titleFontSize);
        DrawText(title, (SCREEN_WIDTH- titleWidth)/2, startY, titleFontSize, GREEN);
        
        // Draw text
        // const char *textPtr = aboutText;
        int currentY = startY + titleFontSize + 40;
        
        int j = 0; //index to iterate over the text
        while (aboutText[j]!='\0') {
            // Handle paragraph breaks
            if (aboutText[j] == '\n') {
                currentY += lineHeight; // Extra space for paragraphs
                j++;
                //if a breakline was found, skip to the next iteration
                continue;
            }
            
            char line[256];
            int i=0;

            // Extract one line's worth of characters(continue until either Reach max line length (charsPerLine-1),
            //hit a newline character or reach the end of the string)
            while (i < charsPerLine-1 && (aboutText[j]!='\0') && aboutText[j] != '\n') {
                line[i] = aboutText[j];
                i++;
                j++;
            }

            //ending adequately the line of the frase
            line[i] = '\0';
            
            // Draw the line (centered)
            int textWidth = MeasureText(line, bodyFontSize);
            DrawText(line, SCREEN_WIDTH/2 - textWidth/2, currentY, bodyFontSize, WHITE);
            
            // Move to next line
            currentY += lineHeight;
            
            // Skip newline characters (already handled above)
            if (aboutText[i] == '\n') j++;
        }
        
        // Draw return instruction
        const char *returnText = "Press ESC to return";
        DrawText(returnText, SCREEN_WIDTH - MeasureText(returnText, 18) - 20, 
                SCREEN_HEIGHT - 30, 18, GRAY);
    }
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

//--audio
    InitAudioDevice();

    //btn
    Sound SOUND_BTN_CLICK = LoadSound("./resources/sound/buttons/buttonclick.ogg");
    Sound SOUND_BTN_HOVER = LoadSound("./resources/sound/buttons/ceramic.ogg");

    //gamestages--
    Sound SOUND_LOST_MUSIC = LoadSound("./resources/sound/gameStages/losemusic.ogg");
    Sound SOUND_PAUSE = LoadSound("./resources/sound/gameStages/pause.ogg");
    Sound SOUND_ZOMBIES_COMING = LoadSound("./resources/sound/gameStages/thezombiesarecomming.ogg");
        SetSoundVolume(SOUND_ZOMBIES_COMING,1.0f);
    Sound SOUND_WIN = LoadSound("./resources/sound/gameStages/win.ogg");

    //plants
    Sound SOUND_PEASHOT_IMPACT = LoadSound("./resources/sound/plant/juicy.ogg");
    Sound SOUND_PLANTING_PLANT = LoadSound("./resources/sound/plant/plant.ogg");
    Sound SOUND_COLLECTING_SUN = LoadSound("./resources/sound/plant/points.ogg");
    Sound SOUND_SHOVEL = LoadSound("./resources/sound/plant/shovel.ogg");

    //soundtracks
    Sound SOUND_HOMEPAGE_MENU = LoadSound("./resources/sound/soundtracks/homepage.mp3");
        SetSoundVolume(SOUND_HOMEPAGE_MENU,0.4f);

    Sound SOUND_GAMEPLAY = LoadSound("./resources/sound/soundtracks/gameplay.mp3");
        SetSoundVolume(SOUND_GAMEPLAY,0.2f);

    //zombies
    Sound SOUND_ZOMBIE_SPAWN = LoadSound("./resources/sound/zombies/groan3.ogg");
    Sound SOUND_ZOMBIE_EAT_PLANT = LoadSound("./resources/sound/zombies/bigchomp.ogg");
    

    


//--textures

Vector2 origin = {0,0};
    //background
        Texture2D TEXTURE_BACKGROUND_IMG = LoadTexture("./resources/sprites/coloredBackGroundPVZ.png");
            Rectangle TEXTURE_BACKGROUND_IMG_SOURCE_REC = {.height=TEXTURE_BACKGROUND_IMG.height,.width=TEXTURE_BACKGROUND_IMG.width,.x=0,.y=0};

        Texture2D TEXTURE_GAMING_BACKGROUND_IMG = LoadTexture("./resources/sprites/dayBackgroundGame.png");
            Rectangle TEXTURE_GAMING_BACKGROUND_IMG_SOURCE_REC = {.height=TEXTURE_GAMING_BACKGROUND_IMG.height,.width=TEXTURE_GAMING_BACKGROUND_IMG.width,.x=0,.y=0};

        Texture2D TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG = LoadTexture("./resources/sprites/confirmingQuit.png");
            Rectangle TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG_SOURCE_REC = {.height=TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG.height,.width=TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG.width,.x=0,.y=0};

        Texture2D TEXTURE_CONFIGURATIONS_BACKGROUND_IMG = LoadTexture("./resources/sprites/configurationsBackground.png");
            Rectangle TEXTURE_CONFIGURATIONS_BACKGROUND_IMG_SOURCE_REC = {.height=TEXTURE_CONFIGURATIONS_BACKGROUND_IMG.height,.width=TEXTURE_CONFIGURATIONS_BACKGROUND_IMG.width,.x=0,.y=0};


    //sun
        Texture2D TEXTURE_SUN_IMG = LoadTexture("./resources/sprites/sun.png");

    //lawn
        //dirt
            Texture2D TEXTURE_DIRT_IMG = LoadTexture("./resources/sprites/dirt.png");
                Rectangle TEXTURE_DIRT_IMG_SOURCE_REC = {.height=TEXTURE_DIRT_IMG.height,.width=TEXTURE_DIRT_IMG.width,.x=0,.y=0};

        //grass
            Texture2D TEXTURE_GRASS_IMG = LoadTexture("./resources/sprites/grass.png");
                Rectangle TEXTURE_GRASS_IMG_SOURCE_REC = {.height=TEXTURE_GRASS_IMG.height,.width=TEXTURE_GRASS_IMG.width,.x=0,.y=0};

    //buttons
            Texture2D TEXTURE_EXIT_BTN_IMG = LoadTexture("./resources/sprites/exit-button.png");
                Rectangle TEXTURE_EXIT_BTN_IMG_SOURCE_REC = {.height=TEXTURE_EXIT_BTN_IMG.height,.width=TEXTURE_EXIT_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_LEADERBOARD_BTN_IMG = LoadTexture("./resources/sprites/leaderboard-button.png");
                Rectangle TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC = {.height=TEXTURE_LEADERBOARD_BTN_IMG.height,.width=TEXTURE_LEADERBOARD_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_PLAY_BTN_IMG = LoadTexture("./resources/sprites/play-button.png");
                Rectangle TEXTURE_PLAY_BTN_IMG_SOURCE_REC = {.height=TEXTURE_PLAY_BTN_IMG.height,.width=TEXTURE_PLAY_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_CONFIGURATIONS_BTN_IMG = LoadTexture("./resources/sprites/configurations-button.png");
                Rectangle TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC = {.height=TEXTURE_CONFIGURATIONS_BTN_IMG.height,.width=TEXTURE_CONFIGURATIONS_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_ABOUT_BTN_IMG = LoadTexture("./resources/sprites/about-button.png");
                Rectangle TEXTURE_ABOUT_BTN_IMG_SOURCE_REC = {.height=TEXTURE_ABOUT_BTN_IMG.height,.width=TEXTURE_ABOUT_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_RESUME_BTN_IMG = LoadTexture("./resources/sprites/resume-button.png");
                Rectangle TEXTURE_RESUME_BTN_IMG_SOURCE_REC = {.height=TEXTURE_RESUME_BTN_IMG.height,.width=TEXTURE_RESUME_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_GOBACK_BTN_IMG = LoadTexture("./resources/sprites/goBack-button.png");
                Rectangle TEXTURE_GOBACK_BTN_IMG_SOURCE_REC = {.height=TEXTURE_GOBACK_BTN_IMG.height,.width=TEXTURE_GOBACK_BTN_IMG.width,.x=0,.y=0};
            
            Texture2D TEXTURE_ZOMBIES_BTN_IMG = LoadTexture("./resources/sprites/zombies-button-transparent.png");
                Rectangle TEXTURE_ZOMBIES_BTN_IMG_SOURCE_REC = {.height=TEXTURE_ZOMBIES_BTN_IMG.height,.width=TEXTURE_ZOMBIES_BTN_IMG.width,.x=0,.y=0};
            
            Texture2D TEXTURE_PLANTS_BTN_IMG = LoadTexture("./resources/sprites/plants-button-transparent.png");
                Rectangle TEXTURE_PLANTS_BTN_IMG_SOURCE_REC = {.height=TEXTURE_PLANTS_BTN_IMG.height,.width=TEXTURE_PLANTS_BTN_IMG.width,.x=0,.y=0};
            
            Texture2D TEXTURE_PEASHOTS_BTN_IMG = LoadTexture("./resources/sprites/peashots-button-transparent.png");
                Rectangle TEXTURE_PEASHOTS_BTN_IMG_SOURCE_REC = {.height=TEXTURE_PEASHOTS_BTN_IMG.height,.width=TEXTURE_PEASHOTS_BTN_IMG.width,.x=0,.y=0};
            
            Texture2D TEXTURE_BACKGROUND_BTN_IMG = LoadTexture("./resources/sprites/background-button-transparent.png");
                Rectangle TEXTURE_BACKGROUND_BTN_IMG_SOURCE_REC = {.height=TEXTURE_BACKGROUND_BTN_IMG.height,.width=TEXTURE_BACKGROUND_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_SOUNDEFFECTS_BTN_IMG = LoadTexture("./resources/sprites/soundeffects-button-transparent.png");
                Rectangle TEXTURE_SOUNDEFFECTS_BTN_IMG_SOURCE_REC = {.height=TEXTURE_SOUNDEFFECTS_BTN_IMG.height,.width=TEXTURE_SOUNDEFFECTS_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_SUNS_BTN_IMG = LoadTexture("./resources/sprites/suns-button-transparent.png");
                Rectangle TEXTURE_SUNS_BTN_IMG_SOURCE_REC = {.height=TEXTURE_SUNS_BTN_IMG.height,.width=TEXTURE_SUNS_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_UNMUTE_BTN_IMG = LoadTexture("./resources/sprites/unmute-button-transparent.png");
                Rectangle TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC = {.height=TEXTURE_UNMUTE_BTN_IMG.height,.width=TEXTURE_UNMUTE_BTN_IMG.width,.x=0,.y=0};

            Texture2D TEXTURE_MUTE_BTN_IMG = LoadTexture("./resources/sprites/mute-button-transparent.png");
                Rectangle TEXTURE_MUTE_BTN_IMG_SOURCE_REC = {.height=TEXTURE_MUTE_BTN_IMG.height,.width=TEXTURE_MUTE_BTN_IMG.width,.x=0,.y=0};
                
            

        //zombies
            Texture2D TEXTURE_NORMAL_ZOMBIE_IMG = LoadTexture("./resources/sprites/zombie.png");

        //plants

            //sunflower
            Texture2D TEXTURE_SUNFLOWER_IMG = LoadTexture("./resources/sprites/sunflower.png");

            //Green peashooter
            Texture2D TEXTURE_GREEN_PEASHOOTER_IMG = LoadTexture("./resources/sprites/peashooter.png");
                //Green peashot
                Texture2D TEXTURE_GREEN_PEASHOT_IMG = LoadTexture("./resources/sprites/peashooter-proj.png");
            
            //Wallnut
            Texture2D TEXTURE_WALLNUT_IMG = LoadTexture("./resources/sprites/wallnut.png");

            //Shovel
            Texture2D TEXTURE_SHOVEL_IMG = LoadTexture("./resources/sprites/shovel.png");

//----------------------------------

const int PLANT_WIDTH = LAWN_WIDTH_VALUE-40;

const Plant SHOVEL_REMOVE_PLANTS = {
    .actionTime=0,
    .color=0,
    .cost=0,
    .creationTime=0,
    .existanceTime=0,
    .format.height= LAWN_HEIGHT_VALUE-20,
    .format.width= PLANT_WIDTH,
    .health=0,
    .peashot=0,
    .referenceTime=0,
    .rowOfPlant=0,
    .texture=TEXTURE_SHOVEL_IMG,
    .type=TYPE_SHOVEL};

const Plant PLANT_SUNFLOWER={
    .format.height= LAWN_HEIGHT_VALUE-20,
    .format.width= PLANT_WIDTH,
    .format.x= 0,
    .format.y= 0,
    .type = TYPE_SUNFLOWER,
    .cost = COST_SUNFLOWER,
    .color = BROWN, 
    .actionTime=ACTION_TIME_SUNFLOWER,
    .existanceTime=0,
    .referenceTime=0,
    .existanceTime=0,
    .health=HEALTH_OF_SUNFLOWER,
    .texture = TEXTURE_SUNFLOWER_IMG,
    .rowOfPlant=-1
    
};

const PeaShot NORMAL_GREEN_PEASHOT={
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
const Plant PLANT_GREEN_PEASHOOTER={
    .format.height= LAWN_HEIGHT_VALUE-20,
    .format.width= PLANT_WIDTH,
    .format.x= 0,
    .format.y= 0,
    .type = TYPE_GREEN_PEASHOOTER,
    .cost = COST_PEASHOOTER,
    .color = BLUE, 
    .actionTime=ACTION_TIME_PEASHOOTER,
    .existanceTime=0,
    .referenceTime=0,
    .existanceTime=0,
    .health=HEALTH_OF_GREEN_PEASHOOTER,
    .peashot = NORMAL_GREEN_PEASHOT,
    .texture = TEXTURE_GREEN_PEASHOOTER_IMG,
    .rowOfPlant=-1
};

const Plant PLANT_WALLNUT={
    .format.height= LAWN_HEIGHT_VALUE-20,
    .format.width= PLANT_WIDTH,
    .format.x= 0,
    .format.y= 0,
    .type = TYPE_WALLNUT,
    .cost = COST_WALLNUT,
    .color = BLUE, 
    .existanceTime=0,
    .referenceTime=0,
    .existanceTime=0,
    .health=HEALTH_OF_WALLNUT,
    .texture = TEXTURE_WALLNUT_IMG
};
const Zombie NORMAL_ZOMBIE={
    .color=GRAY,
    .velocity = 0.5,
    .health =100,
    .rowOfZombie=-10,
    .format={
        //make zombie appear from outside of the window
        .x=SCREEN_WIDTH+30,
        .y=0,
        .width=60,
        .height=LAWN_Y_VALUE
    },
    .isAttacking=0,
    //damage=20/second
    .damage =0.33,
    .texture = TEXTURE_NORMAL_ZOMBIE_IMG
};

const int marginFromTitle=0.3*SCREEN_HEIGHT;
//Btn display
int BTN_WIDTH =SCREEN_WIDTH/3.5;
//margin from title from homepage and menu
int BTN_HEIGHT =(SCREEN_WIDTH-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY;
int BTN_CENTERED_X_FOR_TWO_BUTTONS = (SCREEN_WIDTH-BTN_WIDTH*2)/2;
int BTN_ALONE_CENTERED_X = (SCREEN_WIDTH-BTN_WIDTH)/2;
int BTN_INITIAL_Y=marginFromTitle;
int BTN_DYNAMIC_Y=BTN_INITIAL_Y;

//--about section
Rectangle BTN_GOBACK={.x=SCREEN_WIDTH/30,.y=SCREEN_HEIGHT/30,.width=BTN_WIDTH,.height=BTN_HEIGHT};
bool BTN_GOBACK_HOVER =0;

//--HomePage

    Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
    // bool homePageOptionsSoundHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option outputed its sound
    GAME_SCREEN homePageOptions[HOME_PAGE_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
        //Filling the homePageOptions
        homePageOptions[0]= PLAY;
        homePageOptions[1]= LEADERBOARD;
        homePageOptions[2]= ABOUT;
        homePageOptions[3]= CONFIGURATIONS;
        homePageOptions[4]= EXIT;

        //If index is odd, option goes to the right, if index is even, option to the left

        //Filling the homePageOptionsRec 
        for (int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
            //if its even
            if(!(i&1)){
                homePageOptionsRec[i].height = BTN_HEIGHT;
                homePageOptionsRec[i].width = BTN_WIDTH;
                homePageOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS;
                homePageOptionsRec[i].y = BTN_DYNAMIC_Y;
            }else{
                //if its odd
                homePageOptionsRec[i].height = BTN_HEIGHT;
                homePageOptionsRec[i].width = BTN_WIDTH;
                homePageOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
                homePageOptionsRec[i].y =BTN_DYNAMIC_Y;
                BTN_DYNAMIC_Y+=BTN_HEIGHT;
                // (((SCREEN_HEIGHT-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY))*i
            }
        }
        //if the home_page_option_quantity is an odd number, then the last option will be displayed alone, so,
        //we need to centralize it
        if(HOME_PAGE_OPTIONS_QUANTITY&1){
            homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY-1].x=BTN_ALONE_CENTERED_X;
        }

//---------
//Configurations Menu

    Rectangle configurationsOptionsRec[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refers to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
    bool configurationsOptionsRecHover[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered
    bool mutedSounds[CONFIGURATIONS_OPTIONS_QUANTITY]={0}; 
    // bool configurationsOptionsSoundHover[CONFIGURATIONS_OPTIONS_QUANTITY]={0};//array that tells if an option outputed its sound
    
    CONFIGURATIONS_OPTIONS configurationsOptions[CONFIGURATIONS_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
    //Filling the configurationsOptions
    //backgroundMusic ->mute/unmute
    //zombies ->mute/unmute
    //plants ->mute/unmute
    //peashots ->mute/unmute
    //sun ->mute/unmute
    //soundEffects ->mute/unmute
    
    configurationsOptions[CONFIGURATIONS_SOUND_BACKGROUND]= CONFIGURATIONS_SOUND_BACKGROUND;
    configurationsOptions[CONFIGURATIONS_BTN_BACKGROUND]= CONFIGURATIONS_BTN_BACKGROUND;
    configurationsOptions[CONFIGURATIONS_SOUND_ZOMBIES]= CONFIGURATIONS_SOUND_ZOMBIES;
    configurationsOptions[CONFIGURATIONS_BTN_ZOMBIES]= CONFIGURATIONS_BTN_ZOMBIES;
    configurationsOptions[CONFIGURATIONS_SOUND_PLANTS]= CONFIGURATIONS_SOUND_PLANTS;
    configurationsOptions[CONFIGURATIONS_BTN_PLANTS]= CONFIGURATIONS_BTN_PLANTS;
    configurationsOptions[CONFIGURATIONS_SOUND_PEASHOTS]= CONFIGURATIONS_SOUND_PEASHOTS;
    configurationsOptions[CONFIGURATIONS_BTN_PEASHOTS]= CONFIGURATIONS_BTN_PEASHOTS;
    configurationsOptions[CONFIGURATIONS_SOUND_SUNS]= CONFIGURATIONS_SOUND_SUNS;
    configurationsOptions[CONFIGURATIONS_BTN_SUNS]= CONFIGURATIONS_BTN_SUNS;
    configurationsOptions[CONFIGURATIONS_SOUND_SOUNDEFFECTS]= CONFIGURATIONS_SOUND_SOUNDEFFECTS;
    configurationsOptions[CONFIGURATIONS_BTN_SOUNDEFFECTS]= CONFIGURATIONS_BTN_SOUNDEFFECTS;
    configurationsOptions[CONFIGURATIONS_BTN_GOBACK]= CONFIGURATIONS_BTN_GOBACK;

    //Filling the configurationsOptionsRec
    BTN_DYNAMIC_Y=BTN_INITIAL_Y;
        //If index is odd, option goes to the right, if index is even, option to the left
        for (int i=0;i<CONFIGURATIONS_OPTIONS_QUANTITY;i++){
            //if its even
            if(!(i&1)){
                configurationsOptionsRec[i].height = BTN_HEIGHT;
                configurationsOptionsRec[i].width = BTN_WIDTH;
                configurationsOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS;
                configurationsOptionsRec[i].y = BTN_DYNAMIC_Y;
            }else{
                //if its odd
                configurationsOptionsRec[i].height = BTN_HEIGHT;
                configurationsOptionsRec[i].width = BTN_WIDTH;
                configurationsOptionsRec[i].x=BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
                configurationsOptionsRec[i].y =BTN_DYNAMIC_Y;
                BTN_DYNAMIC_Y+=BTN_HEIGHT;
                // (((SCREEN_HEIGHT-marginFromTitle)/HOME_PAGE_OPTIONS_QUANTITY))*i
            }
        }
        configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK]=BTN_GOBACK;
        configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK].x=0;


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

gamingMenuOptionsRec[i].x=BTN_ALONE_CENTERED_X;

gamingMenuOptionsRec[i].y= marginFromTitle+((SCREEN_HEIGHT-marginFromTitle)/GAMING_MENU_OPTIONS_QUANTITY)*i;
        }

//---------------------------



//--exiting application
    //--exit confirmation
        Rectangle GOBACK_BTN_CPY=BTN_GOBACK;
            GOBACK_BTN_CPY.x = BTN_CENTERED_X_FOR_TWO_BUTTONS;
            GOBACK_BTN_CPY.y =(SCREEN_HEIGHT-BTN_HEIGHT*2)/2;
            bool GOBACK_BTN_CPY_HOVER =0;

        Rectangle EXIT_BTN_CPY=BTN_GOBACK;
            EXIT_BTN_CPY.x = BTN_CENTERED_X_FOR_TWO_BUTTONS+BTN_WIDTH;
            EXIT_BTN_CPY.y =(SCREEN_HEIGHT-BTN_HEIGHT*2)/2;
            bool EXIT_BTN_CPY_HOVER =0;
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
    DeckOfPlants[2] = PLANT_WALLNUT;
    DeckOfPlants[3] = SHOVEL_REMOVE_PLANTS;

    //used to track which card is selected. If card is all nulled, then there's no card selected
    Plant cardSelected = {0};

//---------------------------------------------

//PLANT ---------------------

    //used to track which plants are deployed in the field(lawn)
    Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]={0};
    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
            plantArr[i][j].type=TYPE_NULL_PLANT;
        }
    }

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

    double timeOfLastSun = 0;
    //array to track the suns, if the x and y coordinates are "-1", then we consider it an empty sun
    Rectangle sunArray[SIZE_OF_SUN_ARR]={0};
    //array to track the quantity of sun that the player has
    unsigned int sunGamingStorage=800;
    //array to track when a sun hits the ground
    float groundOfTheSuns[SIZE_OF_SUN_ARR]={0};
    //indexToTrack the end of the array
    int indexOfNextSun = 0;
    //time of spawn of suns = 15s
    double spawnRateSun = 8.0;   
    //used to spawn sun appropriately
    double timeSpawnSunTracking =0;
   


//------------------------


//PEASHOOTER AND PEASHOT


    PeaShot peaShotsArr [SIZE_OF_PEASHOT_ARR];
    int indexOfNextPea = 0;

//---------------------


//ZOMBIE-----------------


    Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]={0};
    //initializing zombie array not to conflict with shooting peas mechanic
        for(int i=0;i<SIZE_OF_ZOMBIES_ARR;i++){
            zombieArr[i].rowOfZombie=-1;
        }
    int indexOfNextZombie=0;
    double spawnRateZombie = 5.0;
    bool firstZombieSpawn =1;
    double timeOfLastZombie = 0;  //saves the actualTime
    double timeForFirstSpawnZombie=30.0;
    //used to spawn zombies appropriately
    double timeSpawnZombieTracking =0;


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

                //playing intromusic once and in loops of its lasting size
                if(!IsSoundPlaying(SOUND_HOMEPAGE_MENU)){
                    PlaySound(SOUND_HOMEPAGE_MENU);
                }


                 for (int i = 0; i < HOME_PAGE_OPTIONS_QUANTITY; i++)
        {
                    if (CheckCollisionPointRec(mousePoint, homePageOptionsRec[i])) {
                        if(!homePageOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        homePageOptionsRecHover[i] = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            currentScreen = homePageOptions[i];
                        }
                    }
                    else {homePageOptionsRecHover[i] = 0;}
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
                    timeOfLastZombie=GetTime();
                    timeOfLastSun=GetTime();
                
                }
            } break;
            
            bool menuWasACTIONED=0;
            float timeSpentAtMenu=0;

            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                
                previousScreen=currentScreen;
                if(!IsSoundPlaying(SOUND_GAMEPLAY)){
                    
                    PlaySound(SOUND_GAMEPLAY);
                }
                //if zombie has gone out of the screen
                if(updatePlantsAndZombiesGameplay(plantArr,peaShotsArr,zombieArr,occupationOfLawn,&indexOfNextPea,&indexOfNextZombie,SOUND_PEASHOT_IMPACT,SOUND_ZOMBIE_EAT_PLANT)){
                    PlaySound(SOUND_LOST_MUSIC);
                    StopSound(SOUND_GAMEPLAY);
                    currentScreen=END_GAME;

                }


                updateSunsPosition(sunArray,indexOfNextSun,groundOfTheSuns);

                for(int i=0;i<NUMBER_ROWS_LAWN;i++){
                    for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                        if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                            lawnRectanglesHover[i][j] = 1;
                        }   
                        else lawnRectanglesHover[i][j] = 0;
                    }
                }
                if(!menuWasACTIONED){
                //used to spawn sun appropriately
                 timeSpawnSunTracking =GetTime();

                //used to spawn zombies appropriately
                 timeSpawnZombieTracking =GetTime();

                UpdateExistanceTime(plantArr,0);
                }else{
                //used to spawn sun appropriately
                 timeSpawnSunTracking =GetTime()-timeSpentAtMenu;

                //used to spawn zombies appropriately
                 timeSpawnZombieTracking =GetTime()-timeSpentAtMenu;

                UpdateExistanceTime(plantArr,timeSpentAtMenu);
                }

                //spawn of the suns
                if((timeSpawnSunTracking-timeOfLastSun>spawnRateSun)&&indexOfNextSun<SIZE_OF_SUN_ARR){
                    AddRandomlySunToArr(sunArray, &indexOfNextSun,lawnRectangles,groundOfTheSuns);
                    timeOfLastSun=GetTime();
                }
                
                //wait more for the first zombie spawn
                if(firstZombieSpawn){
                    if((timeSpawnZombieTracking-timeOfLastZombie>timeForFirstSpawnZombie)){
                        PlaySound(SOUND_ZOMBIES_COMING);
                        AddZombieToZombiesArrRandomly(zombieArr,NORMAL_ZOMBIE,&indexOfNextZombie,lawnRectangles);
                        timeOfLastZombie=GetTime();
                        firstZombieSpawn=0;
                        menuWasACTIONED=0;
                    }
                
                //normal zombie spawn
                }else{
                    if((timeSpawnZombieTracking-timeOfLastZombie>spawnRateZombie)&&indexOfNextZombie<SIZE_OF_ZOMBIES_ARR){
                        PlaySound(SOUND_ZOMBIE_SPAWN);
                        AddZombieToZombiesArrRandomly(zombieArr,NORMAL_ZOMBIE,&indexOfNextZombie,lawnRectangles);
                        timeOfLastZombie=GetTime();
                        menuWasACTIONED=0;
                    }
                }


                if(collectSun(sunArray,&indexOfNextSun,groundOfTheSuns)){
                    PlaySound(SOUND_COLLECTING_SUN);
                    addSunToStorage(&sunGamingStorage);
                }


                PutPlantToField(plantArr,&cardSelected,&sunGamingStorage,occupationOfLawn,lawnRectangles,SOUND_PLANTING_PLANT,SOUND_SHOVEL);


                GenerateSunSunflower(plantArr,lawnRectangles,groundOfTheSuns,sunArray,&indexOfNextSun);

                //Menu, if esc pressed
                if(IsKeyPressed(KEY_ESCAPE)){
                    PlaySound(SOUND_PAUSE);
                    currentScreen = MENU;
                    menuWasACTIONED=1;
                }
            } break;

           //CASE ENDGAME?


            case MENU:{

                for (int i = 0; i < GAMING_MENU_OPTIONS_QUANTITY; i++)
                {
                    if (CheckCollisionPointRec(mousePoint, gamingMenuOptionsRec[i])) {
                         if(!gamingMenuOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        gamingMenuOptionsRecHover[i] = 1;
                        
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            timeSpentAtMenu=GetTime()-timeSpawnSunTracking;
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
                if (CheckCollisionPointRec(mousePoint, BTN_GOBACK)) {
                        if(!BTN_GOBACK_HOVER){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        BTN_GOBACK_HOVER = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            previousScreen=currentScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            currentScreen = HOMEPAGE;
                        }
                    }
                    else {BTN_GOBACK_HOVER = 0;}
            } break;


             case CONFIGURATIONS:
            {

                //playing intromusic once and in loops of its lasting size
                if(previousScreen==HOMEPAGE){
                    if(!IsSoundPlaying(SOUND_HOMEPAGE_MENU)){
                        PlaySound(SOUND_HOMEPAGE_MENU);
                    }
                }else if(previousScreen==GAMEPLAY){
                    if(!IsSoundPlaying(SOUND_GAMEPLAY)){
                        PlaySound(SOUND_GAMEPLAY);
                    }
                }


                 for (int i = 0; i < CONFIGURATIONS_OPTIONS_QUANTITY; i++)
        {
                    if (CheckCollisionPointRec(mousePoint, configurationsOptionsRec[i])) {
                        if(!configurationsOptionsRecHover[i]){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        configurationsOptionsRecHover[i] = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            if(configurationsOptions[i]==CONFIGURATIONS_BTN_GOBACK){
                                StopSound(SOUND_HOMEPAGE_MENU);
                                currentScreen = previousScreen;
                                previousScreen=currentScreen;

                                //if the sound isn't muted, then mute it
                            }else if(!mutedSounds[i]){
                                // TODO: Handle other configuration options
                                //mute the respective sound according to the type of the button
                                switch (configurationsOptions[i])
                                {
                                case CONFIGURATIONS_SOUND_ZOMBIES:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,0.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_PLANTS:{
                                    SetSoundVolume(SOUND_PLANTING_PLANT,0.0f);
                                    SetSoundVolume(SOUND_SHOVEL,0.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,0.0f);
                                }break;

                                 case CONFIGURATIONS_SOUND_BACKGROUND:{
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.0f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_PEASHOTS:{
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_SUNS:{
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_SOUNDEFFECTS:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,0.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,0.0f);
                                    SetSoundVolume(SOUND_PLANTING_PLANT,0.0f);
                                    SetSoundVolume(SOUND_SHOVEL,0.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,0.0f);
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.0f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.0f);
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.0f);
                                }break;
                                default:
                                    break;
                                }
                                //update the variable
                                mutedSounds[i]=1;

                                //if the sound is muted, unmute it
                            } else if(mutedSounds[i]){
                                switch (configurationsOptions[i])
                                {
                                case CONFIGURATIONS_SOUND_ZOMBIES:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,1.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,1.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_PLANTS:{
                                    SetSoundVolume(SOUND_PLANTING_PLANT,1.0f);
                                    SetSoundVolume(SOUND_SHOVEL,1.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,1.0f);
                                }break;

                                 case CONFIGURATIONS_SOUND_BACKGROUND:{
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.4f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.4f);
                                }break;

                                case CONFIGURATIONS_SOUND_PEASHOTS:{
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,1.0f);
                                }break;

                                case CONFIGURATIONS_SOUND_SUNS:{
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.6f);
                                }break;

                                case CONFIGURATIONS_SOUND_SOUNDEFFECTS:{
                                    SetSoundVolume(SOUND_ZOMBIE_SPAWN,1.0f);
                                    SetSoundVolume(SOUND_ZOMBIE_EAT_PLANT,1.0f);
                                    SetSoundVolume(SOUND_PLANTING_PLANT,1.0f);
                                    SetSoundVolume(SOUND_SHOVEL,1.0f);
                                    SetSoundVolume(SOUND_PEASHOT_IMPACT,1.0f);
                                    SetSoundVolume(SOUND_HOMEPAGE_MENU,0.4f);
                                    SetSoundVolume(SOUND_GAMEPLAY,0.4f);
                                    SetSoundVolume(SOUND_COLLECTING_SUN,0.6f);
                                }break;
                                
                                default:
                                    break;
                                }
                                mutedSounds[i]=0;
                            }
                            //and update the soundsmuted variavel
                        }
                    }
                    else {configurationsOptionsRecHover[i] = 0;}
            }} break;


             case EXIT:
            {
            //Confirming exit                
               
            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            if (CheckCollisionPointRec(mousePoint, GOBACK_BTN_CPY)) {
                        if(!GOBACK_BTN_CPY_HOVER){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        GOBACK_BTN_CPY_HOVER = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            currentScreen = previousScreen;
                            StopSound(SOUND_HOMEPAGE_MENU);
                            previousScreen=currentScreen;
                        }
            }
            else if (CheckCollisionPointRec(mousePoint, EXIT_BTN_CPY)) {
                        if(!EXIT_BTN_CPY_HOVER){
                            PlaySound(SOUND_BTN_HOVER);
                        }
                        EXIT_BTN_CPY_HOVER = 1;
                        if(IsGestureDetected(GESTURE_TAP)){
                            PlaySound(SOUND_BTN_CLICK);
                            StopSound(SOUND_HOMEPAGE_MENU);
                           exitWindow = true;
                        }
                        }
              else{  GOBACK_BTN_CPY_HOVER = 0;
            EXIT_BTN_CPY_HOVER=0;}
                
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
                    
                    DrawTexturePro(TEXTURE_BACKGROUND_IMG,TEXTURE_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    
                    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
                    // DrawTexturePro(NORMAL_GREEN_PEASHOT.texture,TEXTURE_GREEN_PEASHOT_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);

                    
                    DrawTexturePro(TEXTURE_PLAY_BTN_IMG,TEXTURE_PLAY_BTN_IMG_SOURCE_REC,homePageOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_LEADERBOARD_BTN_IMG,TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC,homePageOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_ABOUT_BTN_IMG,TEXTURE_ABOUT_BTN_IMG_SOURCE_REC,homePageOptionsRec[2],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,homePageOptionsRec[3],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,homePageOptionsRec[4],origin,0.0f,WHITE);

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
                    DrawTexturePro(TEXTURE_GAMING_BACKGROUND_IMG,TEXTURE_GAMING_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    
                    //Lawn drawing
                    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
                        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
                            //if "i" is odd, if "j" is odd, dirt, else, grass
                            if(i&1){
                                if(j&1){
                                    DrawTexturePro(TEXTURE_DIRT_IMG,TEXTURE_DIRT_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                    // DrawRectangleRec( lawnRectangles[i][j], dirt);
                                }else{
                                    // DrawRectangleRec( lawnRectangles[i][j], grass);
                                    DrawTexturePro(TEXTURE_GRASS_IMG,TEXTURE_GRASS_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                }
                            }else{
                                if(j&1){
                                    DrawTexturePro(TEXTURE_GRASS_IMG,TEXTURE_GRASS_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
                                }else{
                                    DrawTexturePro(TEXTURE_DIRT_IMG,TEXTURE_DIRT_IMG_SOURCE_REC,lawnRectangles[i][j],origin,0.0f,WHITE);
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
                    DrawSuns(sunArray,indexOfNextSun,TEXTURE_SUN_IMG);

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
                    //Background 
                    DrawTexturePro(TEXTURE_BACKGROUND_IMG,TEXTURE_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    //Drawing Menu options
                    DrawTexturePro(TEXTURE_RESUME_BTN_IMG,TEXTURE_RESUME_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,gamingMenuOptionsRec[2],origin,0.0f,WHITE);
                }break;


                case ABOUT:
                {
                    
                    // TODO: Draw ABOUT screen here!
                    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
                    ClearBackground(RAYWHITE);
                    DrawAboutScreen();
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,BTN_GOBACK,origin,0.0f,WHITE);
                    // DrawText("INF vs ZOMBIES is a fun and strategic tower defense game inspired by Plants vs Zombies, developed as a project for the Algorithms and Programming course at the Federal University of Rio Grande do Sul (UFRGS).\nPlayers must defend their base from waves of zombies by placing different plants, each with unique abilities, along a grid. Strategy and quick thinking are key to surviving the endless undead hordes!\nBuilt from scratch in C using the Raylib library, this project was designed to recreate the fun of the original game while strengthening my C programming skills, problem-solving ability, and proactive development approach. Every line of code was crafted to deepen my understanding of efficient logic, clean structure, and real-time game mechanics.\nWhether you're a fan of tower defense games or just curious about game development in C, INF vs ZOMBIES offers a mix of challenge and creativity—with a little programming twist!\nReady to stop the zombie invasion? Let the battle begin!\n(Educational project for UFRGS - Instituto de Informática)", 20, 20, 40, BLACK);
                  

                } break;


                 case CONFIGURATIONS:
                {
                    // TODO: Draw CONFIGURATIONS screen here!
                    //Background
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BACKGROUND_IMG,TEXTURE_CONFIGURATIONS_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);

                    
                    DrawTexturePro(TEXTURE_PLAY_BTN_IMG,TEXTURE_PLAY_BTN_IMG_SOURCE_REC,homePageOptionsRec[0],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_LEADERBOARD_BTN_IMG,TEXTURE_LEADERBOARD_BTN_IMG_SOURCE_REC,homePageOptionsRec[1],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_ABOUT_BTN_IMG,TEXTURE_ABOUT_BTN_IMG_SOURCE_REC,homePageOptionsRec[2],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_CONFIGURATIONS_BTN_IMG,TEXTURE_CONFIGURATIONS_BTN_IMG_SOURCE_REC,homePageOptionsRec[3],origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,homePageOptionsRec[4],origin,0.0f,WHITE);

                    DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND],origin,0.0f,WHITE);
                    if(mutedSounds[CONFIGURATIONS_SOUND_BACKGROUND]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_BACKGROUND],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_BACKGROUND_BTN_IMG,TEXTURE_BACKGROUND_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_BACKGROUND],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_ZOMBIES]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_ZOMBIES],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_ZOMBIES],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_ZOMBIES_BTN_IMG,TEXTURE_ZOMBIES_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_ZOMBIES],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_PLANTS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PLANTS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PLANTS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_PLANTS_BTN_IMG,TEXTURE_PLANTS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_PLANTS],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_PEASHOTS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PEASHOTS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_PEASHOTS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_PEASHOTS_BTN_IMG,TEXTURE_PEASHOTS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_PEASHOTS],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_SUNS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SUNS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SUNS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_SUNS_BTN_IMG,TEXTURE_SUNS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_SUNS],origin,0.0f,WHITE);  

                    if(mutedSounds[CONFIGURATIONS_SOUND_SOUNDEFFECTS]){
                        DrawTexturePro(TEXTURE_UNMUTE_BTN_IMG,TEXTURE_UNMUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SOUNDEFFECTS],origin,0.0f,WHITE);
                    }else{
                        DrawTexturePro(TEXTURE_MUTE_BTN_IMG,TEXTURE_MUTE_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_SOUND_SOUNDEFFECTS],origin,0.0f,WHITE);
                    }
                    DrawTexturePro(TEXTURE_SOUNDEFFECTS_BTN_IMG,TEXTURE_SOUNDEFFECTS_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_SOUNDEFFECTS],origin,0.0f,WHITE);
      
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,configurationsOptionsRec[CONFIGURATIONS_BTN_GOBACK],origin,0.0f,WHITE);                      
                } break;


                 case EXIT:
                {
                    // TODO: Draw EXIT screen here!
                    DrawTexturePro(TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG,TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG_SOURCE_REC,SCREEN_RECTANGLE,origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_GOBACK_BTN_IMG,TEXTURE_GOBACK_BTN_IMG_SOURCE_REC,GOBACK_BTN_CPY,origin,0.0f,WHITE);
                    DrawTexturePro(TEXTURE_EXIT_BTN_IMG,TEXTURE_EXIT_BTN_IMG_SOURCE_REC,EXIT_BTN_CPY,origin,0.0f,WHITE);
            
                } break;
                default: break;
            }

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadTexture(TEXTURE_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_GAMING_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_SUN_IMG);
    UnloadTexture(TEXTURE_GRASS_IMG);
    UnloadTexture(TEXTURE_DIRT_IMG);
    UnloadTexture(TEXTURE_EXIT_BTN_IMG);
    UnloadTexture(TEXTURE_PLAY_BTN_IMG);
    UnloadTexture(TEXTURE_LEADERBOARD_BTN_IMG);
    UnloadTexture(TEXTURE_CONFIGURATIONS_BTN_IMG);
    UnloadTexture(TEXTURE_ABOUT_BTN_IMG);
    UnloadTexture(TEXTURE_RESUME_BTN_IMG);
    UnloadTexture(TEXTURE_SUNFLOWER_IMG);
    UnloadTexture(TEXTURE_NORMAL_ZOMBIE_IMG);
    UnloadTexture(TEXTURE_GREEN_PEASHOOTER_IMG);
    UnloadTexture(TEXTURE_GREEN_PEASHOT_IMG);
    UnloadTexture(TEXTURE_WALLNUT_IMG);
    UnloadTexture(TEXTURE_SHOVEL_IMG);
    UnloadTexture(TEXTURE_GOBACK_BTN_IMG);
    UnloadTexture(TEXTURE_EXIT_BTN_IMG);
    UnloadTexture(TEXTURE_CONFIGURATIONS_BTN_IMG);
    UnloadTexture(TEXTURE_CONFIRMING_QUIT_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_CONFIGURATIONS_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_PLAY_BTN_IMG);
    UnloadTexture(TEXTURE_GAMING_BACKGROUND_IMG);
    UnloadTexture(TEXTURE_MUTE_BTN_IMG);
    UnloadTexture(TEXTURE_UNMUTE_BTN_IMG);
    UnloadTexture(TEXTURE_SUNS_BTN_IMG);
    UnloadTexture(TEXTURE_ZOMBIES_BTN_IMG);
    UnloadTexture(TEXTURE_PLANTS_BTN_IMG);
    UnloadTexture(TEXTURE_PEASHOTS_BTN_IMG);
    UnloadTexture(TEXTURE_ABOUT_BTN_IMG);
    
    //unload sounds
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




    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //-------------------------------------------------------------------------------------

    return 0;

}