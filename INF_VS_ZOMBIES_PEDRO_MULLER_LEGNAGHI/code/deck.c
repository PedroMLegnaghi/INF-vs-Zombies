#include "deck.h"
#include "stdio.h"

Gaming_Deck DeckOfPlants[SIZE_OF_DECK] = {0};
Gaming_Deck *cardSelected = NULL;
Rectangle sunDisplayInGamingBarRectangle = {0};

// Constants of deck display
int DECK_RECTANGLE_X_VALUE;
int DECK_RECTANGLE_Y_VALUE;
int DECK_ELEMENT_WIDTH_VALUE;
int DECK_ELEMENT_HEIGHT_VALUE;
void InitDeckOfPlants(void)
{
    // Constants of deck display
    DECK_RECTANGLE_X_VALUE = 10;
    DECK_RECTANGLE_Y_VALUE = 10;
    DECK_ELEMENT_WIDTH_VALUE = LAWN_WIDTH_VALUE;
    DECK_ELEMENT_HEIGHT_VALUE = LAWN_HEIGHT_VALUE / 2;
    DeckOfPlants[0].plant = PLANT_SUNFLOWER;
    DeckOfPlants[1].plant = PLANT_GREEN_PEASHOOTER;
    DeckOfPlants[2].plant = PLANT_WALLNUT;
    DeckOfPlants[3].plant = PLANT_POTATO_MINE;
    DeckOfPlants[4].plant = SHOVEL_REMOVE_PLANTS;

    DeckOfPlants[0].cooldown = COOLDOWN_OF_SUNFLOWER;
    DeckOfPlants[1].cooldown = COOLDOWN_OF_GREEN_PEASHOOTER;
    DeckOfPlants[2].cooldown = COOLDOWN_OF_WALLNUT;
    DeckOfPlants[3].cooldown = COOLDOWN_OF_POTATO_MINE;

    sunDisplayInGamingBarRectangle = (Rectangle){
        .x = (DECK_RECTANGLE_X_VALUE + 5) + 20,
        .y = (DECK_RECTANGLE_Y_VALUE + 5) + 3,
        .height = DECK_ELEMENT_WIDTH_VALUE - 50,
        .width = DECK_ELEMENT_HEIGHT_VALUE + 10};
}

// DrawMoldureOfSelectedCard:
// given a plant that is selected in the deck of cards, dray a moldure for it. If there isn't plant selected, return void
void DrawMoldureOfSelectedCard()
{
    // if the cardSelected is nulled, return void, because there's no card selected
    if (cardSelected == NULL || (*cardSelected).plant.format.x == 0)
        return;
    // else
    DrawRectangleLinesEx((*cardSelected).plant.format, 2.5f, DARKGREEN);
}

// DrawGamingDeck_SelectCard:
// given the deck of plants, the quantity of suns adn the card selected, draw the interface, checking if one card is being hovered and highlightening it, and updating the card selected(if needed)
void DrawGamingDeck_SelectCard()
{
    int fontSize = 20;
    Vector2 origin = {0, 0};
    // Drawing the sun counter
    int DECK_RECTANGLE_X_VALUECpy = DECK_RECTANGLE_X_VALUE;
    // Drawing the rectangle that subscribes the sun counter
    // big square box
    DrawRectangle(DECK_RECTANGLE_X_VALUE, DECK_RECTANGLE_Y_VALUE, DECK_ELEMENT_WIDTH_VALUE, DECK_ELEMENT_HEIGHT_VALUE + 30, BROWN);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE, DECK_RECTANGLE_Y_VALUE, DECK_ELEMENT_WIDTH_VALUE, DECK_ELEMENT_HEIGHT_VALUE + 30, BLACK);
    // box of the sun
    DrawRectangle(DECK_RECTANGLE_X_VALUE + 5, DECK_RECTANGLE_Y_VALUE + 5, DECK_ELEMENT_WIDTH_VALUE - 10, DECK_ELEMENT_HEIGHT_VALUE + 20, DARKBROWN);
    DrawTexturePro(TEXTURE_SUN_IMG, TEXTURE_SUN_IMG_SOURCE_REC, sunDisplayInGamingBarRectangle, origin, 0.0f, RAYWHITE);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE + 5, DECK_RECTANGLE_Y_VALUE + 5, DECK_ELEMENT_WIDTH_VALUE - 10, DECK_ELEMENT_HEIGHT_VALUE + 20, BLACK);
    // quantity of sun
    DrawRectangle(DECK_RECTANGLE_X_VALUE + 5, DECK_RECTANGLE_Y_VALUE + DECK_ELEMENT_HEIGHT_VALUE + 10, DECK_ELEMENT_WIDTH_VALUE - 10, 30, RAYWHITE);
    DrawRectangleLines(DECK_RECTANGLE_X_VALUE + 5, DECK_RECTANGLE_Y_VALUE + DECK_ELEMENT_HEIGHT_VALUE + 10, DECK_ELEMENT_WIDTH_VALUE - 10, 30, BLACK);
    DrawText(TextFormat(" %d", sunGamingStorage), DECK_RECTANGLE_X_VALUE + 15, DECK_RECTANGLE_Y_VALUE + DECK_ELEMENT_HEIGHT_VALUE + 10, 20, BLACK);

    // used to attribute keys to options
    int keyOfCard = KEY_ONE;

    // Drawing the deck of plants
    for (int i = 0; i < SIZE_OF_DECK; i++)
    {
        DECK_RECTANGLE_X_VALUECpy += DECK_ELEMENT_WIDTH_VALUE;
        DeckOfPlants[i].plant.format.x = DECK_RECTANGLE_X_VALUECpy;
        DeckOfPlants[i].plant.format.y = DECK_RECTANGLE_Y_VALUE;
        DeckOfPlants[i].plant.format.width = DECK_ELEMENT_WIDTH_VALUE;
        DeckOfPlants[i].plant.format.height = DECK_ELEMENT_HEIGHT_VALUE;
        DeckOfPlants[i].plant.existanceTime = 0;
        DeckOfPlants[i].plant.referenceTime = 0;
        DeckOfPlants[i].plant.creationTime = 0;

        Rectangle textureSourceRectanglePlant = {
            .height = DeckOfPlants[i].plant.texture.height,
            .width = DeckOfPlants[i].plant.texture.width,
            .x = 0,
            .y = 0};
        DrawRectangleLines(DECK_RECTANGLE_X_VALUECpy, DECK_RECTANGLE_Y_VALUE, DECK_ELEMENT_WIDTH_VALUE, DECK_ELEMENT_HEIGHT_VALUE, BLACK);

        // user pressing number 1,2,3... selects the cards
        if (IsKeyPressed(keyOfCard))
        {
            cardSelected = &DeckOfPlants[i];
        }

        // if the box is being hovered,
        if (CheckCollisionPointRec(mousePoint, DeckOfPlants[i].plant.format))
        {

            // highlight it
            DrawRectangleRec(DeckOfPlants[i].plant.format, GRAY);

            // if its being hovered and has also been clicked, selected card = that plant
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER))
            {
                cardSelected = &DeckOfPlants[i];
            }
        }
        else
        {
            // else, draw it normaly
            DrawRectangleRec(DeckOfPlants[i].plant.format, BROWN);
        }
        // if the card is available, draw it normaly
        if ((GetTime() - (DeckOfPlants[i].timeToTrackCooldown) > (DeckOfPlants[i].cooldown)))
        {
            // Drawing the plants within the boxes
            DrawTexturePro(DeckOfPlants[i].plant.texture, textureSourceRectanglePlant, DeckOfPlants[i].plant.format, origin, 0.0f, WHITE);
        }
        else
        {
            // if the card isn't available, draw it with gray filter
            DrawTexturePro(DeckOfPlants[i].plant.texture, textureSourceRectanglePlant, DeckOfPlants[i].plant.format, origin, 0.0f, GRAY);
        }

        // If the card being drawn is not the shovel, draw the price of the card (if it is the shovel, the price - that doesn't exist - is not drawn)
        if (i != SIZE_OF_DECK - 1)
        {
            char priceOfCard[5] = {0};
            snprintf(priceOfCard, sizeof(priceOfCard), "%d", DeckOfPlants[i].plant.cost);
            DrawRectangle(DeckOfPlants[i].plant.format.x, DeckOfPlants[i].plant.format.height + 10, DeckOfPlants[i].plant.format.width, fontSize + 5, BROWN);
            DrawText(priceOfCard, DeckOfPlants[i].plant.format.x + (DeckOfPlants[i].plant.format.width - fontSize * 2) / 2, DeckOfPlants[i].plant.format.height + (float)fontSize * 0.75, fontSize, BLACK);
            DrawRectangleLines(DeckOfPlants[i].plant.format.x, DeckOfPlants[i].plant.format.height + 10, DeckOfPlants[i].plant.format.width, fontSize + 5, BLACK);
        }
        // updating the key of the cards accordingly to its order in the InGameDeck
        keyOfCard++;
    }
}

// RemoveSelectedCard:
// Removes the selected card
void RemoveSelectedCard()
{
    // OBS: can't null the cooldown tracking variable here, because if a card is removed, it wasn't necessarily put in the field
    cardSelected = NULL;
}

// PutPlantToField:
// Given the array of plants in the field,the card selected , the sunStorage, the occupationLawn
// checks if plant can be put in the field and properly put it
void PutPlantToField()
{
    // checks if there's a lawn being hovered
    bool isHovered = 0;

    // row,column
    int r = 0, c = 0;
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
    {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
        {
            if (CheckCollisionPointRec(mousePoint, lawnRectangles[i][j]))
            {
                isHovered = 1;
                r = i;
                c = j;
            }
        }
    }
    //  if a plant is selected && check-colision with a block of lawn(for loop,checks if an element has lawnRecHover [i][j]=1,save that i and j)
    //&& eventClick tapping
    if (cardSelected != NULL && isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // if it is a shovel that is being selected, simply just remove the plant in the spot selected
        if ((*cardSelected).plant.type == TYPE_SHOVEL)
        {
            PlaySound(SOUND_SHOVEL);
            RemovePlantFromArr(plantArr, occupationOfLawn, r, c);
        }

        //     else if the amount of suns is sufficient to put a plant in the field  &&
        //          the position chosen is free to be used (checks if occupationLawn[i][j]!=1) &&
        //          this card is available (time of last use is greater than the cooldown of that plant)
        else if (cardSelected != NULL && sunGamingStorage >= (*cardSelected).plant.cost && occupationOfLawn[r][c] != 1 && ((GetTime() - (*cardSelected).timeToTrackCooldown) > (float)(*cardSelected).cooldown))
        {
            // my selected card, by default, has, already, color, type, cost,height and width. But it doesn't have a proper x and y to be displayed on the lawn
            //               add that plant to the array of plants
            PlaySound(SOUND_PLANTING_PLANT);
            Plant plant = (*cardSelected).plant;
            plant.format.x = lawnRectangles[r][c].x + (lawnRectangles[r][c].width - plant.format.width) / 2;
            plant.format.y = lawnRectangles[r][c].y + (lawnRectangles[r][c].height - plant.format.height) / 2;
            plant.creationTime = GetTime();
            plant.rowOfPlant = r;
            plantArr[r][c] = plant;
            //              discount that amount of the sunStorage
            sunGamingStorage -= (*cardSelected).plant.cost;
            //              update the occupationLawn
            occupationOfLawn[r][c] = 1;

            //              update the cooldown tracking variable
            (*cardSelected).timeToTrackCooldown = GetTime();
            //              reset the cardSelected
            RemoveSelectedCard();
        }
    }
}
