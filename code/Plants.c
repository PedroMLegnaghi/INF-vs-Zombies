#include "raylib.h"
#include "Plants.h"
#include "Player.h"

// used to track which plants are deployed in the field(lawn)
Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN] = {0};

//--PEASHOOTER AND PEASHOT
PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR] = {0};
int indexOfNextPea = 0;

// InitPlantArr: used to initializate the array of plants(which is used to track the plants inGame)
void InitPlantArr(void)
{
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
    {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
        {
            plantArr[i][j].type = TYPE_NULL_PLANT;
        }
    }
}

// UpdateHealthOfPlant: Given a plant, updates it's health accordingly with a given damage
void UpdateHealthOfPlant(Plant *plant, float damage)
{
    // verify if damage is a valid damage
    if (damage < 0)
        return;
    plant->health -= damage;
}

// RemovePlantFromArr: Remove a certain plant from the array
void RemovePlantFromArr(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], bool occupationOfLawn[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], int rowOfPlantToBeRemoved, int columnOfPlantToBeRemoved)
{
    if (plantArr[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved].type != TYPE_NULL_PLANT)
    {
        plantArr[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved] = NULL_PLANT; // null the plant == remove
        occupationOfLawn[rowOfPlantToBeRemoved][columnOfPlantToBeRemoved] = 0;  // update the occupation of the lawn
    }
    else
        return; // if the plant is already null, do nothing
}

// UpdateExistanceTime:
// given a Plant array and an int timeCorrection(when another event is actioned in the middle of the gameplay),
// update the existance time of each plant without counting timeCorrection seconds, as not to count unvalid seconds(in menu, for example)
// if no timeCorrection needed, then timeCorrection==0
void UpdateExistanceTime(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float timeCorrection)
{
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
    {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
        {
            if (plantArr[i][j].type != TYPE_NULL_PLANT)
            {
                plantArr[i][j].existanceTime = GetTime() - plantArr[i][j].creationTime - timeCorrection;
            }
        }
    }
}

// UpdateReferenceTime:
// given a Plant, the reference time for that especific plant is set as existanceTime
void UpdateReferenceTime(Plant *plant)
{
    plant->referenceTime = plant->existanceTime;
}

// DrawPlants:
// draw all plants until indexOfNextPlant within the array of Plant passed

void DrawPlants(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN])
{
    Vector2 origin = {0, 0};

    for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
    {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
        {
            if (plantArr[i][j].type != TYPE_NULL_PLANT)
            {

                Texture2D currentTexture;

                // if the plant has an action animation && is currently in action (isAction flag was already activated)
                if (plantArr[i][j].action_animation_duration > 0 && plantArr[i][j].isAction)
                {
                    // The time is the existanceTime minus the actionStartTime, which was set to the time when the action started
                    float timeSinceActionStarted = plantArr[i][j].existanceTime - plantArr[i][j].actionStartTime;

                    // if the animation hasn't finished
                    if (timeSinceActionStarted < plantArr[i][j].action_animation_duration)
                    {
                        currentTexture = plantArr[i][j].actionTexture;

                        // Updates the frame of the animation base on the time since the action started
                        // Uses this system of taking the rest of that division, because then we don't have to worry about the case where the number of frames exceeds the number of frames in the animation
                        // also, this'll work when the animation is in a spritesheet where the frames aren't all in the same row
                        // timeSinceActionStarted-plantArr[i][j].frameTime = int number, we're taking in which frame the animation is currently in
                        int frameCount = (int)(timeSinceActionStarted / plantArr[i][j].frameTime) % plantArr[i][j].frames_per_row_action_animation;
                        plantArr[i][j].currentFrame = frameCount;

                        // Calculates which part of the spritesheet to use (in this case, we are using a spritesheet with all frames on the same line)
                        // the int will truncate the integer part, when the frameCounter is > FramesPerRow, it will automatically identify that the line has been
                        // changed, that is, the frameRow will no longer be, for example, 0, but 1, and so on
                        // the frameCol will identify the column, which will always be 0, since we are using a spritesheet with all frames on the same line
                        int frameRow = plantArr[i][j].currentFrame / plantArr[i][j].frames_per_row_action_animation;
                        int frameCol = plantArr[i][j].currentFrame % plantArr[i][j].frames_per_row_action_animation;

                        float frameWidth = (float)currentTexture.width / plantArr[i][j].frames_per_row_action_animation;
                        float frameHeight = (float)currentTexture.height / plantArr[i][j].frames_per_column_action_animation;

                        // Configures the source rectangle for the correct frame
                        Rectangle frameSourceRec = {
                            .x = frameCol * frameWidth,
                            .y = frameRow * frameHeight,
                            .width = frameWidth,
                            .height = frameHeight};

                        // Draws the current frame of the animation
                        DrawTexturePro(currentTexture, frameSourceRec, plantArr[i][j].format, origin, 0.0f, WHITE);
                    }
                    else
                    {
                        plantArr[i][j].isAction = false; // If the action animation has finished, reset the isAction flag
                    }
                }
                // if it isn't the time for the action animation, or if the plant doesn't have an action animation,
                // check if it has an idle animation and draw it
                else if (plantArr[i][j].idle_animation_duration > 0)
                {
                    currentTexture = plantArr[i][j].idleTexture;

                    // Here, instead of actionTime, we use existanceTime, because it isn't relevant for the idle animation when the idle started
                    plantArr[i][j].currentFrame = ((int)(plantArr[i][j].existanceTime / plantArr[i][j].frameTime)) % plantArr[i][j].frames_per_row_idle_animation;

                    // same process as above, but for the idle animation
                    int frameRow = plantArr[i][j].currentFrame / plantArr[i][j].frames_per_row_idle_animation;
                    int frameCol = plantArr[i][j].currentFrame % plantArr[i][j].frames_per_row_idle_animation;

                    float frameWidth = (float)currentTexture.width / plantArr[i][j].frames_per_row_idle_animation;
                    float frameHeight = (float)currentTexture.height / plantArr[i][j].frames_per_column_idle_animation;

                    Rectangle frameSourceRec = {
                        .x = frameCol * frameWidth,
                        .y = frameRow * frameHeight,
                        .width = frameWidth,
                        .height = frameHeight};

                    DrawTexturePro(currentTexture, frameSourceRec, plantArr[i][j].format, origin, 0.0f, WHITE);
                }
                // if the plant doesn't have any action or idle animation, draw it's normal (static) texture
                else
                {
                    Rectangle PlantTextureSourceRectangle = {
                        .width = plantArr[i][j].texture.width,
                        .height = plantArr[i][j].texture.height,
                        .y = 0,
                        .x = 0};

                    DrawTexturePro(plantArr[i][j].texture, PlantTextureSourceRectangle,
                                   plantArr[i][j].format, origin, 0.0f, WHITE);
                }
            }
        }
    }
}

// PLANTS/SUNFLOWER-----------------

// GenerateSunSunflower:
// given the array of plants and the array of suns, add to the array of suns a sun near
// the sunflower that generated it
void GenerateSunSunflower(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], Rectangle lawn_array[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], float groundOfTheSuns[SIZE_OF_SUN_ARR], Rectangle arr_of_suns[SIZE_OF_SUN_ARR], int *indexOfNextSun)
{
    for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
    {
        for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
        {
            if (plantArr[i][j].type != TYPE_NULL_PLANT)
            {
                // if the plant exists and is a sunflower
                if (plantArr[i][j].type == TYPE_SUNFLOWER && plantArr[i][j].format.x != NULL_PLANT.format.x)
                {
                    // If it is almost time to generate a sun and it hasn't been defined as generating a sun yet
                    if (!plantArr[i][j].isAction && ((float)plantArr[i][j].actionTime - (float)(plantArr[i][j].actionTime - 0.4) <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime)))
                    {
                        // Set the plant as generating a sun and save the time it started generating a sun
                        plantArr[i][j].isAction = true;
                        plantArr[i][j].actionStartTime = plantArr[i][j].existanceTime;
                        plantArr[i][j].currentFrame = 0;
                    }

                    // if it is actually the time to generate a sun
                    if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime))
                    {
                        // update reference time properly, to enable tracking when it is time to generate another sun of that plant
                        UpdateReferenceTime(&plantArr[i][j]);

                        // check sun limit
                        if (*indexOfNextSun >= SIZE_OF_SUN_ARR)
                            return;

                        // position sun near the sunflower
                        float x = plantArr[i][j].format.x + 22;
                        float y = plantArr[i][j].format.y - 5;

                        // add that sun to the array of suns
                        AddSunToArray(arr_of_suns, indexOfNextSun, lawn_array, i, j, groundOfTheSuns, (int)x, (int)y);
                        plantArr[i][j].actionStartTime = plantArr[i][j].existanceTime; // save the time it started generating a sun
                    }
                }
            }
        }
    }
}
// PLANT/PEASHOOTER----------------------------------------

// addPeaToArr: Given the array of peas, the x and y coordinates of that pea, add a pea at the end of the array
void addPeaToArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], PeaShot pea, int *indexOfNextPea)
{
    if (*indexOfNextPea < 0 || *indexOfNextPea > SIZE_OF_PEASHOT_ARR)
        return;                         // if valid index
    peaShotsArr[*indexOfNextPea] = pea; // add pea
    (*indexOfNextPea) += 1;             // update the index
}

// shootPea: Given the array of plants and the array of peas, first checks if its time to shoot a pea. If so, shoot with the proprieties combining with the peashooter and add
//           that pea to the array of peas
void shootPea(Plant plantArr[NUMBER_ROWS_LAWN][NUMBER_COLUMN_LAWN], PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int *indexOfNextPea, Zombie zombieArr[SIZE_OF_ZOMBIES_ARR])
{
    for (int w = 0; w < SIZE_OF_ZOMBIES_ARR; w++)
    {
        for (int i = 0; i < NUMBER_ROWS_LAWN; i++)
        {
            for (int j = 0; j < NUMBER_COLUMN_LAWN; j++)
            {
                // if the zombie is alive and is in the same row as the plant, or if it is the boss
                if ((zombieArr[w].health > 0 && (zombieArr[w].rowOfZombie == plantArr[i][j].rowOfPlant) &&
                     (zombieArr[w].format.x <= (SCREEN_WIDTH - DISTANCE_LAWN_RIGHT_EDGE))) ||
                    (zombieArr[w].type == TYPE_GIGA_GARGANTUAR_BOSS_ZOMBIE &&
                     (zombieArr[w].format.x <= (SCREEN_WIDTH - DISTANCE_LAWN_RIGHT_EDGE))))
                {

                    // If the plant is a peashooter and is not null
                    if (plantArr[i][j].type == TYPE_GREEN_PEASHOOTER && plantArr[i][j].format.x != NULL_PLANT.format.x)
                    {
                        if (*indexOfNextPea < SIZE_OF_PEASHOT_ARR)
                        {
                            // If it is almost time to shoot a pea and it hasn't been defined as shooting yet
                            if (!plantArr[i][j].isAction && ((float)plantArr[i][j].actionTime - (float)(plantArr[i][j].action_animation_duration - 0.4) <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime)))
                            {
                                // set the plant == shooting (isAction =true), save the time of the shooting start
                                plantArr[i][j].isAction = true;
                                plantArr[i][j].actionStartTime = plantArr[i][j].existanceTime;
                                plantArr[i][j].currentFrame = 0;
                            }

                            // if it is the time to shoot the pea
                            if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime))
                            {
                                // shoot the pea
                                float x = plantArr[i][j].format.x + plantArr[i][j].format.width - 35;
                                float y = plantArr[i][j].format.y + 10;
                                PeaShot pea = plantArr[i][j].peashot;
                                pea.format.x = x;
                                pea.format.y = y;
                                pea.rowOfShot = i;
                                UpdateReferenceTime(&plantArr[i][j]);
                                addPeaToArr(peaShotsArr, pea, indexOfNextPea);
                            }
                        }
                    }
                }
            }
        }
    }
}

// DrawPeShots: Draw all Peas of array of peas until the last element
void DrawPeaShots(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfNextPea)
{
    Vector2 origin = {0, 0};
    for (int i = 0; i < indexOfNextPea; i++)
    {
        Rectangle texture_source_rectangle = {
            .width = peaShotsArr[i].texture.width,
            .height = peaShotsArr[i].texture.height,
            .x = 0,
            .y = 0};
        DrawTexturePro(peaShotsArr[i].texture, texture_source_rectangle, peaShotsArr[i].format, origin, 0.0f, WHITE);
    }
}

// UpdatePeaShotPosition: Update, according to its velocity, the position of a pea
void UpdatePeaShotPosition(PeaShot *pea)
{
    pea->format.x += pea->velocity;
}

// RemovePeaFromArr: Remove a certain pea from the array
void RemovePeaFromArr(PeaShot peaShotsArr[SIZE_OF_PEASHOT_ARR], int indexOfPeaToBeRemoved, int *indexOfNextPea)
{
    if (indexOfPeaToBeRemoved < 0 || indexOfPeaToBeRemoved >= *indexOfNextPea)
        return;

    // calculate how many items must be moved
    int numToMove = *indexOfNextPea - indexOfPeaToBeRemoved - 1;

    if (numToMove > 0)
    {
        memmove(
            &peaShotsArr[indexOfPeaToBeRemoved],     // dest
            &peaShotsArr[indexOfPeaToBeRemoved + 1], // origin
            numToMove * sizeof(PeaShot)              // quantity of bytes
        );
    }

    (*indexOfNextPea)--;
}

// about screen--
void DrawAboutScreen()
{
    const char *aboutText =
        "\n\n\n\n\n\n\n"
        "INF vs ZOMBIES is a fun and strategic tower defense game inspired by Plants vs Zombies, "
        "developed as a project for the Algorithms and Programming course at the Federal University "
        "of Rio Grande do Sul (UFRGS).\n"
        "Players must defend their base from waves of zombies by placing different plants, each with "
        "unique abilities, along a grid. Strategy and quick thinking are key to surviving the "
        "endless undead hordes!\n"
        "Built from scratch in C using the Raylib library, this project was designed to recreate the "
        "fun of the original game while strengthening my C programming skills, problem-solving "
        "ability, and proactive development approach.\n"
        "Whether you're a fan of tower defense games or just curious about game development in C, "
        "INF vs ZOMBIES offers a mix of challenge and creativity, with a little programming twist!\n"
        "Ready to stop the zombie invasion? Let the battle begin!\n"
        "(Educational project for UFRGS - Instituto de Informatica)";

    // Text settings
    int titleFontSize = 50;
    int bodyFontSize = 20;
    int charsPerLine = 115;

    // this multiplier is the spacing between the lines
    int lineHeight = bodyFontSize;
    int startY = 50;

    // Draw Background
    DrawTexturePro(TEXTURE_ABOUT_BACKGROUND_IMG, TEXTURE_ABOUT_BACKGROUND_IMG_SOURCE_REC,
                   (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0.0f, WHITE);

    // Draw text
    int currentY = startY + titleFontSize + 40;

    int j = 0; // index to iterate over the text
    while (aboutText[j] != '\0')
    {
        // Handle paragraph breaks
        if (aboutText[j] == '\n')
        {
            currentY += lineHeight; // Extra space for paragraphs
            j++;
            // if a breakline was found, skip to the next iteration (we don't need to draw, just break the line)
            continue;
        }

        char line[charsPerLine];
        int i = 0;

        // Extract one line's worth of characters (continue until either Reach max line length (charsPerLine-1),
        // hit a newline character or reach the end of the string)
        while (i < charsPerLine - 1 && (aboutText[j] != '\0') && aboutText[j] != '\n')
        {
            line[i] = aboutText[j];
            i++;
            j++;
        }

        // ending adequately the line of the frase
        line[i] = '\0';

        // Draw the line (centered)
        int textWidth = MeasureText(line, bodyFontSize);
        DrawText(line, SCREEN_WIDTH / 2 - textWidth / 2, currentY, bodyFontSize, WHITE);

        // Move to next line
        currentY += lineHeight;

        // Skip newline characters (already handled above)
        if (aboutText[i] == '\n')
            j++;
    }
}