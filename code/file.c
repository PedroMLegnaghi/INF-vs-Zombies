#include "file.h"
#include "string.h"
#include "Textures.h"
#include "raylib.h"
#include "zombies.h"
#include "sun.h"
FILE *leaderBoardFile;
PLAYER leaderBoardTop5Players[5] = {0};

FILE *zombiesHordesText;

// importZombiesFromFile:
// given a file, an array(buffer) and the quantity of hordes, import the zombies to the array (buffer) and updates
// the quantity of hordes according to the quantity of elements of the current zombiesHordes
// assumes that the file was already opened correctly (with reading permission)
void importZombiesFromFile(FILE *file, int zombiesHordes[QUANTITY_MAX_HORDES], int *quantityOfHordes)
{
    int j = 0;
    while (!(feof(file)) && j < QUANTITY_MAX_HORDES)
    {
        fscanf(file, "%d ", &zombiesHordes[j]);
        j++;
    }
    (*quantityOfHordes) = j;

    // variables used to update the spawn rate of zombies and sun, making the game more difficult as the game progresses
    timeToDecreaseFromSpawnRateZombieEachHorde = spawnRateZombie / (*quantityOfHordes);
    timeToIncreaseSpawnRateSunEachHorde = 2 * spawnRateSun / (*quantityOfHordes);
}

// checksIfInTop5:
// given a player and thethe top5 players, check if it's reached the top five of the leaderboard(true) or not(false)
int checksIfInTop5(PLAYER player, PLAYER topFivePlayers[5])
{
    for (int i = 0; i < 5; i++)
    {
        if (topFivePlayers[i].playerName[0] == '\0' ||
            player.pointsOfPlayer > topFivePlayers[i].pointsOfPlayer)
        {
            return 1;
        }
    }
    return 0;
}

// reorderTop5:
// given the points of a player and the points of the top5 players, checks if he's reached the top five and updates the top five
void reorderTop5(PLAYER player, PLAYER topFivePlayers[5])
{
    if (!checksIfInTop5(player, topFivePlayers))
        return;

    int insertPosition = -1;

    // searches for the correct index of the new player to be put in the top 5 leaderboard
    for (int i = 0; i < 5; i++)
    {
        if (topFivePlayers[i].playerName[0] == '\0' ||
            player.pointsOfPlayer > topFivePlayers[i].pointsOfPlayer)
        {
            insertPosition = i;
            break;
        }
    }

    if (insertPosition == -1)
        return;

    // reorder the rest of the players of the leaderboard
    // calculate how many items must be moved
    int numToMove = 4 - insertPosition;
    if (numToMove > 0)
    {
        memmove(
            &topFivePlayers[insertPosition + 1], // destino
            &topFivePlayers[insertPosition],     // origem
            numToMove * sizeof(PLAYER)           // bytes
        );
    }

    // add the new player to it's correct place
    topFivePlayers[insertPosition] = player;
}

// importPlayersFromFile:
// given a file and an array(buffer), import it's top five players to the array (buffer)
// assumes that the file was already opened correctly (with reading permission)
// return the quantity of elements read from the file (accordingly to fread from stdio.h)
int importPlayersFromFile(FILE *file, PLAYER players[5])
{
    return fread(players, sizeof(PLAYER), 5, file);
}

// recordPlayersToFile
// given a file and a list of players, record them in the file
// assumes that the file was already opened correctly (with writing permission)
// return the quantity of elements written to the file (accordingly to fread from stdio.h)
int recordPlayersToFile(FILE *file, PLAYER players[5])
{
    // rewind the file to overwrite the informations
    rewind(file);
    return fwrite(players, sizeof(PLAYER), 5, file);
}

// drawTopFivePlayers:
// given the top five players, displays them in the screen
void drawTopFivePlayers(PLAYER players[5])
{
    int maxCharactersText = 40;
    int fontOfText = 40;

    // margin from title from homepage and menu
    int TEXTBOX_HEIGHT = fontOfText + 10;
    int TEXTBOX_INITIAL_Y = SCREEN_HEIGHT / 8 + (SCREEN_HEIGHT / 20);
    int TEXTBOX_DYNAMIC_Y = TEXTBOX_INITIAL_Y;

    DrawTexturePro(TEXTURE_LEADERBOARD_BACKGROUND_IMG, TEXTURE_LEADERBOARD_BACKGROUND_IMG_SOURCE_REC, SCREEN_RECTANGLE, origin, 0.0f, WHITE);

    char playerDisplayFormat[maxCharactersText];
    for (int i = 0; i < 5; i++)
    {
        // if the player is invalid, don't display it
        if (players[i].playerName[0] == '\0')
        {
            continue;
        }
        snprintf(playerDisplayFormat, sizeof(playerDisplayFormat), "%dº: %s - %d pontos", i + 1, players[i].playerName, players[i].pointsOfPlayer);
        int widthOfText = MeasureText(playerDisplayFormat, fontOfText);
        int TEXTBOX_ALONE_CENTERED_X = (SCREEN_WIDTH - widthOfText) / 2 + (SCREEN_WIDTH / 42);
        DrawText(playerDisplayFormat, TEXTBOX_ALONE_CENTERED_X, TEXTBOX_DYNAMIC_Y, fontOfText, WHITE);
        TEXTBOX_DYNAMIC_Y += TEXTBOX_HEIGHT;
    }
}
