#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // necessary for mmemov
#define HOME_PAGE_OPTIONS_QUANTITY 5 //quantity of options in the Homepage
#define GAMING_MENU_OPTIONS_QUANTITY 3 //resume, configurations and exit
#define MAX_SUN_IN_SCREEN 100 //maximum quantity of suns in screen
#define screenWidth 800
#define screenHeight 580
#define targetFPS 60
#define numberLawnColumns 9
#define numberLawnRows 5
#define VALUE_OF_EACH_SUN 25
#define SIZE_OF_DECK 2
#define SIZE_OF_ZOMBIES_ARR 100

const int DISTANCE_RIGHT_EDGE = (0.025)*screenWidth;
const int DISTANCE_LEFT_EDGE = DISTANCE_RIGHT_EDGE;
const int DISTANCE_INFERIOR_EDGE = DISTANCE_RIGHT_EDGE;
const int DISTANCE_SUPERIOR_EDGE = DISTANCE_RIGHT_EDGE*2;
const int initialLawnWidthValue = (screenWidth-DISTANCE_RIGHT_EDGE-DISTANCE_LEFT_EDGE)/numberLawnColumns-12;
const int initialLawnHeightValue = (screenHeight-DISTANCE_INFERIOR_EDGE-DISTANCE_SUPERIOR_EDGE)/numberLawnRows-8;
const int initialLawnXValue = initialLawnWidthValue;
const int initialLawnYValue = initialLawnHeightValue;

const int xOfDeckRectangle = 10;
const int yOfDeckRectangle = 10;
const int widthOfEachElementOfDeck =initialLawnWidthValue/(SIZE_OF_DECK+1);
const int heightOfEachElementOfDeck = 60;
Vector2 mousePoint = { 0.0f, 0.0f }; //useful to track the user's mouse

const int SIZE_OF_PEASHOT_ARR = 300;
typedef enum GAME_SCREEN {LOGO = 0, HOMEPAGE, PLAY, LEADERBOARD, ABOUT, CONFIGURATIONS, EXIT //HOMEPAGE OPTIONS
    ,USER_DATA, DECK_SELECTION, GAMEPLAY,MENU, RESUME} GAME_SCREEN;//GAMEPLAY OPTIONS
    
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
typedef struct peaShot
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
    ACTION_TIME_PEASHOOTER =3,
    
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

//struct of plants, used majorly to load the image of each plant and track it in the game (each functionality will be added separately)
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
    //sound?
    //png?
    
}Plant;
const Plant NULL_PLANT ={0};
const Plant PLANT_SUNFLOWER={
    .format.height= initialLawnHeightValue-20,
    .format.width= initialLawnWidthValue-20,
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
    //----
};
const Plant PLANT_GREEN_PEASHOOTER={
    .format.height= initialLawnHeightValue-20,
    .format.width= initialLawnWidthValue-20,
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
const Zombie NORMAL_ZOMBIE={
    .color=GRAY,
    .velocity = 3,
    .health =100,
    .rowOfZombie=-1,
    .format={
        //make zombie appear from outside of the window
        .x=screenWidth+30,
        .y=0,
        .width=20,
        .height=initialLawnYValue
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

//Functions-------------------------------------

//SUN FUNCTIONS---
//AddSunToArray:Rectangle[], int,Rectangle [], Rectangle->void
//Given an array of suns, the index of the next sun, the array of lawns, the array of grounds of suns and the proprieties of Rectangle, add that sun to the array of suns 
void AddSunToArray(Rectangle array_of_suns[MAX_SUN_IN_SCREEN],  int *indexOfNextSun,Rectangle lawn_array[numberLawnRows][numberLawnColumns],int rowOfGround,int columnOfGround,float groundOfTheSuns[MAX_SUN_IN_SCREEN], int x, int y, int width, int height) {
    array_of_suns[*indexOfNextSun].x = x;
        array_of_suns[*indexOfNextSun].y =  y;
        array_of_suns[*indexOfNextSun].width = width;
        array_of_suns[*indexOfNextSun].height = height;
        groundOfTheSuns[*indexOfNextSun] =  lawn_array[rowOfGround][columnOfGround].y;
        *indexOfNextSun+=1;
        
    }
    //AddRandomlySunToArr:Rectangle[], int, Rectangle [] [], float->img(void)
    //Given an array of suns, the index of the next sun , the array of lawns of the game and the groundOfTheSuns array, add that sun to the array with x and y position of a random lawn of the game 
    void AddRandomlySunToArr(Rectangle array_of_suns[MAX_SUN_IN_SCREEN],  int *indexOfNextSun, Rectangle lawn_array[numberLawnRows][numberLawnColumns],float groundOfTheSuns[MAX_SUN_IN_SCREEN]) {
        if (*indexOfNextSun >= MAX_SUN_IN_SCREEN) return;
        //chooses a number beetwen 0 and numberLawnRows
        int row = rand() % (numberLawnRows);
        //chooses a number beetwen 0 and numberLawnColumns
        int column = rand() % (numberLawnColumns);
        //we set this "y" position as the final "y" position of the sun, so it starts at 0 and goes until it hit it's "y" position (it's ground)
        // groundOfTheSuns[*indexOfNextSun] =  lawn_array[row][column].y;

        //starts at "y"=-20 and goes until it hits its ground (groundArray)
        AddSunToArray(array_of_suns,indexOfNextSun,lawn_array,row,column,groundOfTheSuns,lawn_array[row][column].x,-20,20,20);
        
        
    }

    //updateSunsPosition:Rectangle[] int Rectangle []->void
    //given the array of the suns of the game ,groundOfTheSuns and the index of the next sun, update their position, until they hit their ground
    void updateSunsPosition(Rectangle array_of_suns[MAX_SUN_IN_SCREEN], int indexOfSun,float groundOfTheSuns[MAX_SUN_IN_SCREEN]){
        for(int i=0;i<indexOfSun;i++){
            //if the sun hasn't hit it's ground, (we use "lesser than" here to avoid rounding problems)
            if((array_of_suns[i].y<groundOfTheSuns[i]))
            //update its position
            array_of_suns[i].y+=0.5;
            if (array_of_suns[i].y > groundOfTheSuns[i]) {
                array_of_suns[i].y = groundOfTheSuns[i]; // evita ultrapassar o chão
            }
        }
    };

    //RemoveSunToArray:Rectangle[], int, float []->img(void)
    //Given an array of suns ,groundOfTheSuns and the index of the sun, remove the last sun of the arrayofsuns and remove its ground and update the array
    void RemoveSunOfArray(Rectangle array_of_suns[MAX_SUN_IN_SCREEN], int *indexOfNextSun, int index, float groundOfTheSuns[MAX_SUN_IN_SCREEN]) {
        if (index < 0 || index >= *indexOfNextSun) return;

        for (int i = index; i < (*indexOfNextSun) - 1; i++) {
            array_of_suns[i] = array_of_suns[i + 1];
            groundOfTheSuns[i] = groundOfTheSuns[i + 1];
        }
        (*indexOfNextSun)--;

        // Invalida o último (agora vazio)
        array_of_suns[*indexOfNextSun].x = -1;
        array_of_suns[*indexOfNextSun].y = -1;
        groundOfTheSuns[*indexOfNextSun] = 0;
    }

    //collectSun:Rectangle [], Vector2, int, float []->int
    //Given an array of suns, the mousePointer,the quantityofSun of the player, the index of the next sun and the array of the grounds of the suns, returns wheter one sun was collected(1) or not(0) and collects it
    int collectSun(Rectangle array_of_suns[MAX_SUN_IN_SCREEN],int *indexOfNextSun, float groundOfTheSuns[MAX_SUN_IN_SCREEN])
    {
        for (int i = 0; i < *indexOfNextSun; i++) {
            if (CheckCollisionPointRec(mousePoint, array_of_suns[i])) {
                if (IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    RemoveSunOfArray(array_of_suns, indexOfNextSun, i, groundOfTheSuns);
                    return 1;//Sun was collected
                    break; // removes only one sun per click
                }
            }
        }
        return 0;//sun was not collected
    }
    //addSunToStorage:unsigned int -> void
    //given the gamingSunStorage, add the value of one sun to the storage
    void addSunToStorage(unsigned int *gamingSunStorage){
        *gamingSunStorage+=VALUE_OF_EACH_SUN;
    }

    //DrawSuns:Rectangle[], int->img(void)
    //Given the array of suns of the game and the index of the sun, draw the suns
    void DrawSuns(Rectangle array_of_suns[MAX_SUN_IN_SCREEN], int indexOfNextSun){
        for(int i = 0; i < indexOfNextSun; i++){
            if (array_of_suns[i].x != -1 && array_of_suns[i].y != -1) {
                DrawRectangle(array_of_suns[i].x, array_of_suns[i].y,
                            array_of_suns[i].width, array_of_suns[i].height, YELLOW);
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
        
  void RemovePlantFromArr(Plant plantArr[numberLawnRows][numberLawnColumns],bool occupationOfLawn[numberLawnRows][numberLawnColumns],int rowOfPlantToBeRemoved, int columnOfPlantToBeRemoved ){
        plantArr[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved]=NULL_PLANT;
        occupationOfLawn[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved]=0;
    }
    //UpdateExistanceTime:Plant *->void
    //given a Plant array, update the existance time of each plant
   void UpdateExistanceTime(Plant plantArr[numberLawnRows][numberLawnColumns]){
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColumns;j++){
               plantArr[i][j].existanceTime=GetTime()-plantArr[i][j].creationTime;
            }
        }
    }
    //UpdateReferenceTime: Plant ->void
    //given a Plant, reference time for that especific plant is set as existanceTime
    void UpdateReferenceTime(Plant *plant){
        plant->referenceTime=plant->existanceTime;
            
    }
    //DrawPlants:Plant[numberLawnColumns*numberLawnRows]->void
    //draw all plants until indexOfNextPlant within the array of Plant passed
    void DrawPlants(Plant plantArr[numberLawnRows][numberLawnColumns]){
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColumns;j++){
                //if the plant [i][j] isn't empty (empty = coordinate x==0)
                if(plantArr[i][j].format.x!=0){
                    DrawRectangleRec(plantArr[i][j].format,plantArr[i][j].color );
                }
            }
        }
    }
    // GenerateSunSunflower: Plant* Rectangle int->void
    // given the array of plants and the array of suns, add to the array of suns a sun near 
    // the sunflower that generated it
   void GenerateSunSunflower(Plant plantArr[numberLawnRows][numberLawnColumns],Rectangle lawn_array[numberLawnRows][numberLawnColumns],float groundOfTheSuns[MAX_SUN_IN_SCREEN], Rectangle arr_of_suns[MAX_SUN_IN_SCREEN], int *indexOfNextSun){
    for(int i=0; i<numberLawnRows; i++){
        for(int j=0; j<numberLawnColumns; j++){
            // if the plant exists and is a sunflower
            if (plantArr[i][j].format.x != 0 && plantArr[i][j].type == TYPE_SUNFLOWER){
                // if it's time to generate a sun
                if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime)){
                    // update reference time properly
                    UpdateReferenceTime(&plantArr[i][j]);

                    // check sun limit
                    if (*indexOfNextSun >= MAX_SUN_IN_SCREEN) return;

                    // position sun near the sunflower
                    float x = plantArr[i][j].format.x + 22;
                    float y = plantArr[i][j].format.y - 5;
                    AddSunToArray(arr_of_suns, indexOfNextSun,lawn_array,i,j,groundOfTheSuns, (int)x, (int)y, 20, 20);

                }
            }
        }
    }
}
//PEASHOOTER----------------------------------------

//addPeaToArr: Given the array of peas, the x and y coordinates of that pea, add a pea at the end of the array
void addPeaToArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],PeaShot pea, int *indexOfNextPea){
    if (*indexOfNextPea < 0 || *indexOfNextPea > SIZE_OF_PEASHOT_ARR) return;
    peaShotsArr[*indexOfNextPea] = pea;
    (*indexOfNextPea)+=1;
}
//shootPea: Given the array of plants and the array of peas, first checks if its time to shoot a pea. If so, shoot with the proprieties combining with the peashooter and add 
//          that pea to the array of peas
void shootPea(Plant plantArr[numberLawnRows][numberLawnColumns],PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int *indexOfNextPea){
    for(int i=0; i<numberLawnRows; i++){
        for(int j=0; j<numberLawnColumns; j++){
            // if the plant is a green peashooter, its shot is green with its rules
            if (plantArr[i][j].format.x != 0 && plantArr[i][j].type == TYPE_GREEN_PEASHOOTER){
                // if it's time to shoot a pea
                if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime)){
                    // update reference time properly
                    UpdateReferenceTime(&plantArr[i][j]);
                    // position pea near the green peashooter
                    float x = plantArr[i][j].format.x + 5;
                    float y = plantArr[i][j].format.y + (initialLawnHeightValue/2);
                    PeaShot pea = NORMAL_GREEN_PEASHOT;
                    pea.format.x=x;
                    pea.format.y=y;
                    pea.rowOfShot=i;
                    addPeaToArr(peaShotsArr,pea,indexOfNextPea);
                }
            }
        }
    }
}

//DrawPeShots: Draw all Peas of array of peas until the last element
void DrawPeaShots (PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],int indexOfNextPea){
    for(int i=0;i<indexOfNextPea;i++){
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
void AddZombieToZombiesArrRandomly( Zombie zombiesArr[SIZE_OF_ZOMBIES_ARR], int *indexOfNextZombie,Rectangle lawn_array[numberLawnRows][numberLawnColumns]){
    Zombie zombie = NORMAL_ZOMBIE;
    //generating zombie's row randomly
    int row = rand() % (numberLawnRows);
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
//UpdateZombiesArrPosition: Given the array of zombies and the index of the next zombie,
// CHECKS THE ZOMBIE STATUS(STOP/MOVING) (CHECKCOLISION FUNCTION)update the positio of each zombie
// void UpdateZombiesArrPosition(Zombie zombieArr[SIZE_OF_ZOMBIES_ARR], int indexOfNextZombie){
//     for(int i=0;i<indexOfNextZombie;i++){
//     UpdateZombiePosition(&zombieArr[i]);
// }
// }
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
    //else return false
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
//UpdateZombiesAndProjectiles: Update all projectiles thrown and also manages the 
//hits of each projectile in each zombie, as well as deals proprely with the zombies health
void updateZombiesAndProjectiles(Plant plantArr[numberLawnRows][numberLawnColumns],
                                 PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR],
                                 Zombie zombieArr[SIZE_OF_ZOMBIES_ARR],
                                 bool occupationOfLawn[numberLawnRows][numberLawnColumns],
                                 int *indexOfNextPea,
                                 int *indexOfNextZombie)
{
    // SHOOT NEW PEAS
    shootPea(plantArr, peaShotsArr, indexOfNextPea);

    // 1. UPDATE PEA POSITIONS
    for (int j = 0; j < *indexOfNextPea; j++) {
        UpdatePeaShotPosition(&peaShotsArr[j]);
    }
    // PRIMEIRO: Remova todos os zumbis mortos ou fora da tela
// for (int i = 0; i < *indexOfNextZombie; i++) {
// }
    // 2. CHECK COLLISIONS WITH PEA AND UPDATE ZOMBIE HEALTH
for (int i = 0; i < *indexOfNextZombie; i++) {
        if (zombieArr[i].health <= 0 || zombieArr[i].format.x < -10) {
            RemoveZombie(zombieArr, indexOfNextZombie, i);
            i--; // Atualiza o índice corretamente
            continue;
        }
        for (int j = 0; j < *indexOfNextPea; j++) {
            if (verifyPeaShotColisionWithZombie(peaShotsArr[j], zombieArr[i])) {
                UpdateZombieHealth(&zombieArr[i], peaShotsArr[j].damage);
                RemovePeaFromArr(peaShotsArr,j,indexOfNextPea);
                j--;
            }
            //if the position of the pea passes the width of the screen, remove it from the array
            if(peaShotsArr[j].format.x>screenWidth){
                RemovePeaFromArr(peaShotsArr,j,indexOfNextPea);
                j--;
            }
        }
        //2.3 CHECK ZOMBIE COLISION WITH PLANT AND UPDATE VARIABLES
        
        zombieArr[i].isAttacking=0;
        for(int r=0;r<numberLawnRows;r++){
            for(int c=0;c<numberLawnColumns;c++){
                //if the plant died, remove it from the screen
                if(plantArr[r][c].health<=0){
                    RemovePlantFromArr(plantArr,occupationOfLawn,r,c);
                }
                //if a colision zombie/plant is happening
                if(verifyPlantColisionWithZombie(plantArr[r][c], zombieArr[i])){
                    //update isAttacking propriety
                    //DONT UPDATE THE POSITION OF THE ZOMBIE IF HE'S ATTACKING
                    //Update healthOfPlant according to the damage that the zombie gives per frame
                    UpdateHealthOfPlant(&plantArr[r][c],zombieArr[i].damage);
                    zombieArr[i].isAttacking=1;
                    break;
                }
                if(zombieArr[i].isAttacking) break;
            }
        }
        //2.1 VERIFY IF ZOMBIE HAS GONE OUT OF THE SCREEN
     
        //update the zombie's position if he's not attakcing
        if(!zombieArr[i].isAttacking){
            UpdateZombiePosition(&zombieArr[i]);
        }
    }
    
}

//----------------------------------
//GAMING DECK----------------
    //DrawGamingDeck:Plant [], int, Vector2, Rectangle ->void
    //given the deck of plants, the quantity of suns adn the card selected, draw the interface, checking if one card is being hovered and highlightening it, and updating the card selected(if needed)
    void DrawGamingDeck(Plant DeckOfPlants [SIZE_OF_DECK], unsigned int quantityOfSun, Plant *cardSelected){
        //Drawing the sun counter
        int xOfDeckRectangleCpy = xOfDeckRectangle;
        DrawRectangle(xOfDeckRectangle,yOfDeckRectangle,widthOfEachElementOfDeck,heightOfEachElementOfDeck,MAROON);
        DrawRectangleLines(xOfDeckRectangle,yOfDeckRectangle,widthOfEachElementOfDeck,heightOfEachElementOfDeck,BLACK);
        DrawText(TextFormat(" %d", quantityOfSun),xOfDeckRectangle,yOfDeckRectangle+15,20,YELLOW);
        //Drawing the deck of plants
        for (int i=0;i<SIZE_OF_DECK;i++){
            xOfDeckRectangleCpy+= widthOfEachElementOfDeck;
            Plant plantBoxOfCard ={
                .format={
                    .x= xOfDeckRectangleCpy,
                    .y =yOfDeckRectangle,
                    .width= widthOfEachElementOfDeck,
                    .height= heightOfEachElementOfDeck
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
            DrawRectangleLines(xOfDeckRectangleCpy,yOfDeckRectangle,widthOfEachElementOfDeck,heightOfEachElementOfDeck,BLACK);
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
            DrawRectangle(xOfDeckRectangleCpy+10,yOfDeckRectangle+20,20,20,DeckOfPlants[i].color);
        }
    }
//DrawMoldureOfSelectedCard:Plant->void
//given a plant that is selected in the deck of cards, dray a moldure for it. If there isn't plant selected, return void
void DrawMoldureOfSelectedCard(Plant cardSelected){
    //if the cardSelected is nulled, return void, because there's no card selected
    if(cardSelected.format.x==0) return;
    //else
    DrawRectangleLinesEx(cardSelected.format,2.5f, DARKGREEN);
}

// RemoveSelectedCard: Rectangle * -> void
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
(Plant plantArr [numberLawnRows][numberLawnColumns], 
    Plant *cardSelected, unsigned int *sunStorage,bool occupationOfLawn[numberLawnRows][numberLawnColumns],Rectangle lawnRectangles[numberLawnRows][numberLawnColumns])
    {
        //checks if there's a lawn being hovered
        bool isHovered =0;
        
        //row,column
        int r=0, c=0;
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColumns;j++){
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
//-------------/------------------------------------


int main (void){
    //initialization
    // TODO: Initialize all required variables and load all required data here!
//--screen
   srand(time(NULL));
    InitWindow(screenWidth, screenHeight,"raylib [core] example - basic screen manager");
    GAME_SCREEN currentScreen = LOGO;
    GAME_SCREEN previousScreen = HOMEPAGE;
//--------

//--HomePage
Rectangle homePageOptionsRec[HOME_PAGE_OPTIONS_QUANTITY]={0};//initializing array of rectangles that refer to the options of the game in the landpage(PLAY, leaderboard, about, configurations and exit)
bool homePageOptionsRecHover[HOME_PAGE_OPTIONS_QUANTITY]={0};//array that tells if an option is hovered 
GAME_SCREEN homePageOptions[HOME_PAGE_OPTIONS_QUANTITY] ={0};//array to navigate over the options of the game
    //Filling the homePageOptions
    homePageOptions[0]= PLAY;
    homePageOptions[1]= LEADERBOARD;
    homePageOptions[2]= ABOUT;
    homePageOptions[3]= CONFIGURATIONS;
    homePageOptions[4]= EXIT;

    //Filling the homePageOptionsRec 
    for (int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
        homePageOptionsRec[i].height = 40;
        homePageOptionsRec[i].width = 240;
        homePageOptionsRec[i].x=(screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30;
        homePageOptionsRec[i].y = 110+60*i;
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
        gamingMenuOptionsRec[i].height = 40;
        gamingMenuOptionsRec[i].width = 240;
        gamingMenuOptionsRec[i].x=(screenWidth-MeasureText("INF vs ZOMBIES", 20))/2-30;
        gamingMenuOptionsRec[i].y = 110+60*i;
    }
    //--exiting application
    SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works
    bool exitWindow = false;    // Flag to set window to exit
    //---------
    double startTimeForNaturalSunSpawn = GetTime();
    double startTimeForZombieSpawn = GetTime();  //saves the actualTime
    int framesCounter = 0;          // Useful to count frames
    SetTargetFPS(targetFPS);               // Set desired framerate (frames-per-second)

//--PLAY variables
const unsigned int MAX_SIZE_OF_NAME = 14+1;
char playerName[MAX_SIZE_OF_NAME];
    for (int i=0;i<MAX_SIZE_OF_NAME;i++)
        playerName[i]='\0';
    unsigned int sizeOfName = 0;//variable used to track the size of the name of the user
//DECK-----
Plant DeckOfPlants [SIZE_OF_DECK]={0};
//DO A FUNCTION INITDECK!
DeckOfPlants[0] = PLANT_SUNFLOWER;
DeckOfPlants[1] = PLANT_GREEN_PEASHOOTER;
   
//used to track which card is selected. If card is all nulled, then there's no card selected
Plant cardSelected = {0};
//used to track which plants are deployed in the field(lawn)
Plant plantArr[numberLawnRows][numberLawnColumns]={0};

    //LAWN--------------
    //lawns of the game
    //used to verify if a plant already exists in a spot
    bool occupationOfLawn[numberLawnRows][numberLawnColumns]={0};
    bool lawnRectanglesHover[numberLawnRows][numberLawnColumns];
    Rectangle lawnRectangles[numberLawnRows][numberLawnColumns];
        for(int i=0;i<numberLawnRows;i++){
            for(int j=0;j<numberLawnColumns;j++){
                lawnRectangles[i][j].x = 30+initialLawnXValue*j;
                lawnRectangles[i][j].y = 80+initialLawnYValue*i;
                lawnRectangles[i][j].width = initialLawnWidthValue;
                lawnRectangles[i][j].height = initialLawnHeightValue;
            }
        }

    //SUN
    //array to track the suns, if the x and y coordinates are "-1", then we consider it an empty sun
    Rectangle sunArray[MAX_SUN_IN_SCREEN]={0};
    //array to track the quantity of sun that the player has
    unsigned int sunGamingStorage=100000;
    //array to track when a sun hits the ground
    float groundOfTheSuns[MAX_SUN_IN_SCREEN]={0};
    //indexToTrack the end of the array
    int indexOfNextSun = 0;
    //time of spawn of suns = 15s
    double spawnRateSun = 4.0;   
    //initializing the size of all suns
    for (int i=0;i<MAX_SUN_IN_SCREEN;i++){
        sunArray[i].height = 20;
        sunArray[i].width = 20;
    }
//---------

//PEASHOOTER AND PEASHOT
PeaShot peaShotsArr [SIZE_OF_PEASHOT_ARR];
int indexOfNextPea = 0;

    //--------------------------------------------------------------------------------------
//ZOMBIE
Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]={0};
int indexOfNextZombie=0;
double spawnRateZombie = 4.0;
    // Main game loop
    while (!exitWindow)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();//tracking, effectively, the user's mouse
        
        //if user wants to end applications, redirect him to an exit confirmation
        if(WindowShouldClose()){
            currentScreen=  EXIT;
        }

        switch (currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                
                framesCounter++;    // Count frames

                // Wait for 7 seconds (420 frames) before jumping to HOMEPAGE screen
                if (framesCounter > 5)
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
                // TODO: Update USERDATA screen variables here!-------------------
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
                    currentScreen = GAMEPLAY;  // Ou qualquer próxima tela desejada
                }


               
            } break;
            
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!
                previousScreen=currentScreen;
                updateZombiesAndProjectiles(plantArr,peaShotsArr,zombieArr,occupationOfLawn,&indexOfNextPea,&indexOfNextZombie);
                UpdateExistanceTime(plantArr);
                updateSunsPosition(sunArray,indexOfNextSun,groundOfTheSuns);
                for(int i=0;i<numberLawnRows;i++){
                    for(int j=0;j<numberLawnColumns;j++){
                        if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j])) {
                            lawnRectanglesHover[i][j] = 1;
                        }   
                        else lawnRectanglesHover[i][j] = 0;
                    }
                }
                double timeSpawnSunTracking =GetTime();
                double timeSpawnZombieTracking =GetTime();
                //spawn of the suns
                if((timeSpawnSunTracking-startTimeForNaturalSunSpawn>spawnRateSun)&&indexOfNextSun<MAX_SUN_IN_SCREEN){
                    AddRandomlySunToArr(sunArray, &indexOfNextSun,lawnRectangles,groundOfTheSuns);
                    startTimeForNaturalSunSpawn=GetTime();
                }
                if((timeSpawnZombieTracking-startTimeForZombieSpawn>spawnRateZombie)&&indexOfNextZombie<SIZE_OF_ZOMBIES_ARR){
                    AddZombieToZombiesArrRandomly(zombieArr,&indexOfNextZombie,lawnRectangles);
                    startTimeForZombieSpawn=GetTime();
                }
                if(collectSun(sunArray,&indexOfNextSun,groundOfTheSuns)){
                    addSunToStorage(&sunGamingStorage);
                }
                PutPlantToField(plantArr,&cardSelected,&sunGamingStorage,occupationOfLawn,lawnRectangles);
                GenerateSunSunflower(plantArr,lawnRectangles,groundOfTheSuns,sunArray,&indexOfNextSun);
                if(IsKeyPressed(KEY_ESCAPE)){
                    currentScreen = MENU;
                }
                
            } break;
            case MENU:{
                    // TODO: Draw HOMEPAGE screen here!
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
                // TODO: Update ENDING screen variables here!
                

                // Press enter to return to HOMEPAGE screen
               
            } break;
             case CONFIGURATIONS:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to HOMEPAGE screen
               
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
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw LOGO screen here!
                    DrawText("Pedrokas's gaming enterprise® \n\t\t\t\t\t\t\t\tpresents... ", 70, 150, 40, DARKGREEN);

                } break;
                case HOMEPAGE:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw HOMEPAGE screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
                    for(int i=0;i<HOME_PAGE_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(homePageOptionsRec[i], WHITE);
                         DrawRectangleLines(homePageOptionsRec[i].x, homePageOptionsRec[i].y, homePageOptionsRec[i].width, homePageOptionsRec[i].height, BLACK);
                        //tracking hover over the options
                        if(homePageOptionsRecHover[i]==true){
                            DrawRectangleRec(homePageOptionsRec[i], RED);
                        }
                    }
                    DrawText("INF vs ZOMBIES", (screenWidth-MeasureText("INF vs ZOMBIES", 40))/2, 20, 40, DARKGREEN);
                    DrawText("PLAY", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 120, 20, DARKGREEN);
                    DrawText("LEADERBOARD", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 180, 20, DARKGREEN);
                    DrawText("ABOUT", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 240, 20, DARKGREEN);
                    DrawText("CONFIGURATIONS", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 300, 20, DARKGREEN);
                    DrawText("EXIT", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 360, 20, DARKGREEN);
                } break;
                case USER_DATA:
                {
                    // TODO: Draw USERDATA screen here!
                  
                    ClearBackground(RAYWHITE);
                    Rectangle textBox = { screenWidth/2 - 150, 180,  MeasureText("A",40)*8+30, 50 };
                    if(sizeOfName>8){
                        textBox.width = sizeOfName*MeasureText("A",40)+10;
                    }
                    
                    DrawText("Tell me your name!", 240, 140, 20, GRAY);

                    DrawRectangleRec(textBox, LIGHTGRAY);
                    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);

                    DrawText(playerName, textBox.x + 5, textBox.y + 8, 40, MAROON);

                    DrawText(TextFormat("Press enter to confirm: %i/%i", sizeOfName, MAX_SIZE_OF_NAME-1), 315, 250, 20, DARKGRAY);

                    if (sizeOfName < MAX_SIZE_OF_NAME) {
                        // Draw blinking underscore char
                     DrawText("_",( textBox.x + 8 + MeasureText(playerName, 40)),(textBox.y + 12), 40, MAROON);
                    } 
                  

                } break;
                 case DECK_SELECTION:
                {
                    // ClearBackground(RAYWHITE);
                    // // TODO: Draw ENDING screen here!
                    // DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
                    // DrawText(TextFormat("Nice to meet you, %s :)", playerName), 20, 20, 40, DARKBLUE);
                    // DrawText("Select you deck", 120, 220, 20, DARKBLUE);
                    currentScreen = GAMEPLAY;

                } break;
                case GAMEPLAY:
                {
                    for(int i=0;i<numberLawnRows;i++){
                        for(int j=0;j<numberLawnColumns;j++){
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
                    DrawGamingDeck(DeckOfPlants,sunGamingStorage, &cardSelected);
                    DrawMoldureOfSelectedCard(cardSelected);
                    DrawPlants(plantArr);
                    DrawPeaShots(peaShotsArr,indexOfNextPea);
                    DrawZombieArr(zombieArr,indexOfNextZombie);
                    DrawSuns(sunArray,indexOfNextSun);
                    RemoveSelectedCard(&cardSelected);
                }break;
                case MENU:{
                    for(int i=0;i<GAMING_MENU_OPTIONS_QUANTITY;i++){
                        DrawRectangleRec(gamingMenuOptionsRec[i], WHITE);
                         DrawRectangleLines(gamingMenuOptionsRec[i].x, gamingMenuOptionsRec[i].y, gamingMenuOptionsRec[i].width, gamingMenuOptionsRec[i].height, BLACK);
                         //tracking hover over the options
                         if(gamingMenuOptionsRecHover[i]==true){
                             DrawRectangleRec(gamingMenuOptionsRec[i], RED);
                            }
                        }
                        DrawText("INF vs ZOMBIES", (screenWidth-MeasureText("INF vs ZOMBIES", 40))/2, 20, 40, DARKGREEN);
                        DrawText("RESUME", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 120, 20, DARKGREEN);
                        DrawText("CONFIGURATIONS", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 180, 20, DARKGREEN);
                        DrawText("EXIT", (screenWidth-MeasureText("INF vs ZOMBIES", 20))/2, 240, 20, DARKGREEN);
                }break;
                case ABOUT:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                 case CONFIGURATIONS:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to HOMEPAGE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                 case EXIT:
                {
                    ClearBackground(RAYWHITE);
                    // TODO: Draw EXIT screen here!
                DrawRectangle(0, 100, screenWidth, 200, BLACK);
                DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);
            
                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;

}