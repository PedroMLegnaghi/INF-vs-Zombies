#include "zombies.h"

Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]={0};

int quantityOfMaxHordes=10;

int quantityOfHordes =0;

//used to track when to reset a zombie horde
int zombiesCreatedSinceLastHorde=0;

int indexOfCurrentHorde=0;

//used to track the next zombie to be spawned
int indexOfNextZombie=0;

int zombiesQuantityPerHorde[QUANTITY_MAX_HORDES]={0};

double spawnRateZombie = 4.0;

//used to know if it is the first time to spawn a zombie
bool firstZombieSpawn =1;

//reference for spawn of zombies
double timeOfLastZombie = 0;

double timeForFirstSpawnZombie=35.0;

//used to spawn zombies appropriately
double timeSpawnZombieTracking =0;

//InitZombiesArrs:
//given an array of zombies, initialize it (all 0); Also, initialize the array that controls the hordes this function is used not to have a conflict with shooting peas mechanic (memory leak)
void InitZombiesArrs(Zombie zombieArr[SIZE_OF_ZOMBIES_ARR]){
    for(int i=0;i<SIZE_OF_ZOMBIES_ARR;i++){
        zombieArr[i].rowOfZombie=-1;
    }

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

//ResetZombieHorde:
//resets all variables related to the creation and administration of a zombie horde
void ResetZombieHorde(){
    indexOfCurrentHorde++;
    zombiesCreatedSinceLastHorde=0;
}

//LastZombieOfHordeSpawned:
//returns if the last zombie of the horde has spawned
int LastZombieOfHordeSpawned(){
    if(zombiesCreatedSinceLastHorde==zombiesQuantityPerHorde[indexOfCurrentHorde]){
        return 1;
    }else{
        return 0;
    }
}

//LastZombieOfHordeDied:
//returns if the last zombie of the horde has died
int LastZombieOfHordeDied(){
    //as the mechanic of removing zombies decrements the index of the next zombie each time, when
    //that index is 0 and it is not the firstZombieSpawn, then it's in definitely the last zombie of the horde
    if(indexOfNextZombie==0&&!firstZombieSpawn&&LastZombieOfHordeSpawned()){
        return 1;
    }else{
        return 0;
    }
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
