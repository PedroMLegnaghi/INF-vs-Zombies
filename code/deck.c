 #include "deck.h"

Plant DeckOfPlants [SIZE_OF_DECK]={0};
Plant cardSelected = {0};
Rectangle sunDisplayInGamingBarRectangle={0};
 void InitDeckOfPlants(void){
     DeckOfPlants[0] = PLANT_SUNFLOWER;
     DeckOfPlants[1] = PLANT_GREEN_PEASHOOTER;
     DeckOfPlants[2] = PLANT_WALLNUT;
     DeckOfPlants[3] = SHOVEL_REMOVE_PLANTS;
     sunDisplayInGamingBarRectangle=(Rectangle){
        .x=(DECK_RECTANGLE_X_VALUE+5)+17,
        .y=(DECK_RECTANGLE_Y_VALUE+5)+3,
        .height=DECK_ELEMENT_WIDTH_VALUE-40,
        .width=DECK_ELEMENT_HEIGHT_VALUE-30
     };
    }

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
    //big square box
    DrawRectangle(DECK_RECTANGLE_X_VALUE,DECK_RECTANGLE_Y_VALUE,DECK_ELEMENT_WIDTH_VALUE,DECK_ELEMENT_HEIGHT_VALUE,BROWN);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE,DECK_RECTANGLE_Y_VALUE,DECK_ELEMENT_WIDTH_VALUE,DECK_ELEMENT_HEIGHT_VALUE,BLACK);
    //box of the sun
    DrawRectangle(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+5,DECK_ELEMENT_WIDTH_VALUE-10,DECK_ELEMENT_HEIGHT_VALUE-10,DARKBROWN);
    DrawTexturePro(TEXTURE_SUN_IMG,TEXTURE_SUN_IMG_SOURCE_REC,sunDisplayInGamingBarRectangle,origin,0.0f,RAYWHITE);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+5,DECK_ELEMENT_WIDTH_VALUE-10,DECK_ELEMENT_HEIGHT_VALUE-10,BLACK);
    //quantity of sun
    DrawRectangle(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+DECK_ELEMENT_HEIGHT_VALUE-20,DECK_ELEMENT_WIDTH_VALUE-10,30,RAYWHITE);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE+5,DECK_RECTANGLE_Y_VALUE+DECK_ELEMENT_HEIGHT_VALUE-20,DECK_ELEMENT_WIDTH_VALUE-10,30,BLACK);
    DrawText(TextFormat(" %d", quantityOfSun),DECK_RECTANGLE_X_VALUE+15,DECK_RECTANGLE_Y_VALUE+DECK_ELEMENT_HEIGHT_VALUE-20,20,BLACK);

    

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

