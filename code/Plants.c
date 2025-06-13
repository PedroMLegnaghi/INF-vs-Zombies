#include "raylib.h"
#include "Plants.h"

//used to track which plants are deployed in the field(lawn)
Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN]={0};

//--PEASHOOTER AND PEASHOT
PeaShot peaShotsArr [SIZE_OF_PEASHOT_ARR]={0};
int indexOfNextPea = 0;


// InitPlantArr: used to initializate the array of plants(which is used to track the plants inGame)
void InitPlantArr(void){
    for(int i=0;i<NUMBER_ROWS_LAWN;i++){
        for(int j=0;j<NUMBER_COLUMN_LAWN;j++){
            plantArr[i][j].type=TYPE_NULL_PLANT;
        }
    }
}
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