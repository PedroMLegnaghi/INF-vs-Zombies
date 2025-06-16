#include "gameplay.h"
#include "Sound.h"

// Flag to indicate if the game has (at least once) ended
// [used to save unnecessary gameplay updates and redeclarations after the game has ended]
bool gameHasEnded = false; 

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

//resetGameplay: Reset all variables related to the gameplay, such as the plants, peas, zombies, and occupation of the lawn
void resetGameplay(){
    // Reset plants
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++) {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++) {
            plantArr[i][j] = (Plant){0};  // Reset each plant to default values
        }
    }
    
    // Reset peas
    for (int i = 0; i < SIZE_OF_PEASHOT_ARR; i++) {
        peaShotsArr[i] = (PeaShot){0};  // Reset each pea to default values
    }
    indexOfNextPea = 0;  
    
    // Reset zombies
    for (int i = 0; i < SIZE_OF_ZOMBIES_ARR; i++) {
        zombieArr[i] = (Zombie){0};  // Reset each zombie to default values
    }
    firstZombieSpawn=1; // Reset the first zombie spawn flag
    timeOfLastZombie = 0; // Reset the last zombie spawn time
    timeSpawnZombieTracking = 0; // Reset the zombie spawn tracking time
    indexOfNextZombie = 0;
    
    // Reset lawn occupation
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++) {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++) {
            occupationOfLawn[i][j] = false;  // Reset each lawn position to unoccupied
        }
    }

    // Reset suns
    for (int i = 0; i < SIZE_OF_SUN_ARR; i++) {
        sunArray[i] = (Rectangle){0};  // Reset each sun to default values
        groundOfTheSuns[i] = 0;
    }
    indexOfNextSun = 0;
    sunGamingStorage = 0;
    timeOfLastSun = 0; // Reset the last sun spawn time
    timeSpawnSunTracking = 0; // Reset the sun spawn tracking time

    //Reset player
    for(int i = 0; i < MAX_SIZE_OF_NAME; i++) {
        player.playerName[i] = '\0';  // Reset each character in the player name to null terminator
    }
    player.pointsOfPlayer= 0;

    //reset gameEnded flag
    gameHasEnded=false;
}

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

    //2. ZOMBIES
    for (int i = 0; i < *indexOfNextZombie; i++) 
    {
        //verifies if zombie has already died
        if (zombieArr[i].health <= 0) 
        {
                //if he has died, then i don't need to check the other things, jump to the next iteration
                RemoveZombie(zombieArr, indexOfNextZombie, i);
                
                player.pointsOfPlayer+=50;

                i--; // Updates the index correctly
                continue;//jumping to the next iteration
        }

        //verifies if zombie has gone out of the screen, return true
            if (zombieArr[i].format.x <= 0){
                return 1;
            } 

        // 3. CHECK COLLISIONS WITH PEA AND UPDATE ZOMBIE HEALTH

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

        //4 CHECK ZOMBIE COLISION WITH PLANT AND UPDATE VARIABLES

        zombieArr[i].isAttacking=0;
        for(int r=0;r<NUMBER_ROWS_LAWN;r++)
        {
            for(int c=0;c<NUMBER_COLUMN_LAWN;c++)
            {
                // if(plantArr[r][c].type!=TYPE_NULL_PLANT)
                // {
                //     RemovePlantFromArr(plantArr,occupationOfLawn,r,c);
                // }
                
                //if a colision zombie/plant is happening
                if(verifyPlantColisionWithZombie(plantArr[r][c], zombieArr[i]))
                {
                    //update isAttacking propriety
                    //DONT UPDATE THE POSITION OF THE ZOMBIE IF HE'S ATTACKING
                    //Update healthOfPlant according to the damage that the zombie gives per frame
                    UpdateHealthOfPlant(&plantArr[r][c],zombieArr[i].damage);
                    zombieArr[i].isAttacking=1;
                    //if the plant died, remove it from the screen
                        if(plantArr[r][c].health<=0){
                            PlaySound(zombieAtePlant);
                            RemovePlantFromArr(plantArr,occupationOfLawn,r,c);
                        }
    
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
