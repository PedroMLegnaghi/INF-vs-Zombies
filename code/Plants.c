#include "raylib.h"
#include "Plants.h"
#include "Player.h"
#define TOTAL_FRAMES 60 // Total de frames na spritesheet

float SHOOT_ANIMATION_DURATION = TOTAL_FRAMES / TARGET_FPS; // Duração da animação de tiro

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
            // Se a planta não está vazia (NULLED)
            if (plantArr[i][j].type != TYPE_NULL_PLANT)
            {

                Texture2D currentTexture;
                // Se tem uma animacao de acao
                if (plantArr[i][j].action_animation_duration > 0 && plantArr[i][j].isAction)
                {
                    float tempoDesdeQueComecouAShootar = plantArr[i][j].existanceTime - plantArr[i][j].actionStartTime;

                    // Verifica se está em acao e dentro da duração da animação
                    if (tempoDesdeQueComecouAShootar < plantArr[i][j].action_animation_duration)
                    {
                        // Usa a textura de acao
                        currentTexture = plantArr[i][j].actionTexture;

                        // Avança o quadro da animação baseado no tempo
                        // faco esse sistema (em vez de ir aumentando de 1 em um o frameCounter para evitar fazer depois quando framecounter== TOTAL_FRAMES um if e depois igualar a 0)
                        // em essencia, vai ser a mesma coisa, por o tempo Desde que comecou a atirar aumenta de um em um a cada unidade de tempo
                        int frameCount = (int)(tempoDesdeQueComecouAShootar / plantArr[i][j].frameTime) % TOTAL_FRAMES;
                        plantArr[i][j].currentFrame = frameCount;

                        // Calcula qual parte da spritesheet usar (nesse caso, estamos usando uma spritesheet com todos os frames na mesma linha)
                        // o int ira truncar a parte inteira, quando o frameCounter for > FramesPerRow, ele vai identificar automaticamente que a linha foi
                        // trocada, ou seja, a frameRow nao mais sera, por exemplo, 0, mas sim 1, e assim por diante
                        // o frameCol ira identificar a coluna, que sempre sera 0, pois estamos usando uma spritesheet com todos os frames na mesma linha
                        int frameRow = plantArr[i][j].currentFrame / plantArr[i][j].frames_per_row_action_animation;
                        int frameCol = plantArr[i][j].currentFrame % plantArr[i][j].frames_per_row_action_animation;

                        float frameWidth = (float)currentTexture.width / plantArr[i][j].frames_per_row_action_animation;
                        float frameHeight = (float)currentTexture.height / plantArr[i][j].frames_per_column_action_animation;

                        // Configura o retângulo fonte para o quadro correto
                        Rectangle frameRec = {
                            .x = frameCol * frameWidth,
                            .y = frameRow * frameHeight,
                            .width = frameWidth,
                            .height = frameHeight};

                        // Desenha o quadro atual da animação
                        DrawTexturePro(currentTexture, frameRec, plantArr[i][j].format, origin, 0.0f, WHITE);
                    }
                    else
                    {
                        plantArr[i][j].isAction = false; // Reseta a planta para acabar a acao
                    }
                }
                // ja que nao pode ser feita sua animacao de acao, se tem uma animacao idle, desenhá-la
                else if (plantArr[i][j].idle_animation_duration > 0)
                {
                    currentTexture = plantArr[i][j].idleTexture;

                    // Avança o quadro da animação idle
                    // Aqui, o tempo de existanceTime é usado para calcular o quadro atual, seguindo a mesma logica, mas nao precisando de uma variavel auxiliar, pois nao precisamos
                    // de um outro referencial
                    plantArr[i][j].currentFrame = ((int)(plantArr[i][j].existanceTime / plantArr[i][j].frameTime)) % TOTAL_FRAMES;

                    // Calcula qual parte da spritesheet usar (nesse caso, estamos usando uma spritesheet com todos os frames na mesma linha)
                    // o int ira truncar a parte inteira, quando o frameCounter for > FramesPerRow, ele vai identificar automaticamente que a linha foi
                    // trocada, ou seja, a frameRow nao mais sera, por exemplo, 0, mas sim 1, e assim por diante
                    // o frameCol ira identificar a coluna, que sempre sera 0, pois estamos usando uma spritesheet com todos os frames na mesma linha
                    int frameRow = plantArr[i][j].currentFrame / plantArr[i][j].frames_per_row_idle_animation;
                    int frameCol = plantArr[i][j].currentFrame % plantArr[i][j].frames_per_row_idle_animation;

                    float frameWidth = (float)currentTexture.width / plantArr[i][j].frames_per_row_idle_animation;
                    float frameHeight = (float)currentTexture.height / plantArr[i][j].frames_per_column_idle_animation;

                    // Configura o retângulo fonte para o quadro correto
                    Rectangle frameRec = {
                        .x = frameCol * frameWidth,
                        .y = frameRow * frameHeight,
                        .width = frameWidth,
                        .height = frameHeight};

                    // Desenha o quadro atual da animação
                    DrawTexturePro(currentTexture, frameRec, plantArr[i][j].format, origin, 0.0f, WHITE);
                }
                // senao, se nao tiver animacao idle, desenhar a imagem estatica mesmo
                else
                {
                    // Para outras plantas, mantém o desenho normal
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
                    // Se é quase a hora de gerar sol e ela ainda nao foi definida como gerando sol, iniciar a animacao de gerar sol
                    if (!plantArr[i][j].isAction && ((float)plantArr[i][j].actionTime - (float)(SHOOT_ANIMATION_DURATION - 0.4) <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime)))
                    {
                        // NOVO: Define a planta como gerando sol e salva o tempo inicial
                        plantArr[i][j].isAction = true;
                        plantArr[i][j].actionStartTime = plantArr[i][j].existanceTime;
                        plantArr[i][j].currentFrame = 0; // Reinicia o frame para a animação
                    }

                    // if it is time to generate a sun
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
                // Verifica se há um zumbi na mesma linha ou se é um boss
                if (((zombieArr[w].rowOfZombie == plantArr[i][j].rowOfPlant) &&
                     (zombieArr[w].format.x <= (SCREEN_WIDTH - DISTANCE_LAWN_RIGHT_EDGE - DISTANCE_LAWN_RIGHT_EDGE))) ||
                    (zombieArr[w].type == TYPE_GIGA_GARGANTUAR_BOSS_ZOMBIE &&
                     (zombieArr[w].format.x <= (SCREEN_WIDTH - DISTANCE_LAWN_RIGHT_EDGE - DISTANCE_LAWN_RIGHT_EDGE))))
                {

                    // Se a planta é um peashooter e não está nula
                    if (plantArr[i][j].type == TYPE_GREEN_PEASHOOTER && plantArr[i][j].format.x != NULL_PLANT.format.x)
                    {
                        if (*indexOfNextPea < SIZE_OF_PEASHOT_ARR)
                        {
                            // Se é quase a hora de atirar e ela ainda nao foi definida como atirando, iniciar a animacao de tiro
                            if (!plantArr[i][j].isAction && ((float)plantArr[i][j].actionTime - (float)(plantArr[i][j].action_animation_duration - 0.4) <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime)))
                            {
                                // NOVO: Define a planta como atirando e salva o tempo inicial
                                plantArr[i][j].isAction = true;
                                plantArr[i][j].actionStartTime = plantArr[i][j].existanceTime;
                                plantArr[i][j].currentFrame = 0; // Reinicia o frame para a animação
                            }

                            // se esta na hora de atirar
                            if (plantArr[i][j].actionTime <= (plantArr[i][j].existanceTime - plantArr[i][j].referenceTime))
                            {
                                // Cria o projétil
                                float x = plantArr[i][j].format.x + plantArr[i][j].format.width - 35;
                                float y = plantArr[i][j].format.y + 10;
                                PeaShot pea = plantArr[i][j].peashot;
                                pea.format.x = x;
                                pea.format.y = y;
                                pea.rowOfShot = i;
                                // Atualiza o tempo de referência
                                UpdateReferenceTime(&plantArr[i][j]);
                                // Adiciona a ervilha ao array
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
        return; // invalid index

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

    (*indexOfNextPea)--; // decrement the index of next pea
}

// about screen--
void DrawAboutScreen()
{
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
    // this multiplier stands for the spacing between the lines
    int lineHeight = bodyFontSize * 1.3f;
    int startY = 50;

    // Draw Background
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    // Draw title (centered)
    const char *title = "About INF vs ZOMBIES";
    int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (SCREEN_WIDTH - titleWidth) / 2, startY, titleFontSize, GREEN);

    // Draw text
    // const char *textPtr = aboutText;
    int currentY = startY + titleFontSize + 40;

    int j = 0; // index to iterate over the text
    while (aboutText[j] != '\0')
    {
        // Handle paragraph breaks
        if (aboutText[j] == '\n')
        {
            currentY += lineHeight; // Extra space for paragraphs
            j++;
            // if a breakline was found, skip to the next iteration
            continue;
        }

        char line[256];
        int i = 0;

        // Extract one line's worth of characters(continue until either Reach max line length (charsPerLine-1),
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