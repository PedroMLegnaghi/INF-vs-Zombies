#include "Player.h"

//--PLAYER
PLAYER player = {
    .playerName = {0},
    .pointsOfPlayer = 0};

// handlePlayerNameInput:
// Handles the input of the player's name. When player presses enter key, returns 1, else, returns 0.
int handlePlayerNameInput(void)
{
    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0)
    {
        // NOTE: Only allow keys in range [32..125]
        // MAX_SIZE_OF_NAME-1 to garantee the space for '\0'
        if ((key >= 32) && (key <= 125) && (strlen(player.playerName) < MAX_SIZE_OF_NAME - 1))
        {
            player.playerName[strlen(player.playerName)] = (char)key;
        }
        key = GetCharPressed(); // Check next character in the queue
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        player.playerName[strlen(player.playerName) - 1] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER) && strlen(player.playerName) > 0)
    {
        return 1;
    }

    return 0;
}