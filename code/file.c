//teremos um arquivo binario que contem o nome do jogador e a sua pontuacao. 
//teremos um arquivo de texto que recebe a quantidade de zombies que deve vir por horda, apos a ultima horda, o jogo acaba
#include "file.h"
#include "string.h"
#include "Textures.h"
#include "raylib.h"
#include "zombies.h"
#include "sun.h"
FILE *leaderBoardFile;
PLAYER leaderBoardTop5Players[5]={0};

FILE *zombiesHordesText;
// int zombiesQuantityPerHorde[]

//importZombiesFromFile:
//given a file, an array(buffer)adn the quantity of hordes, import the zombies to the array (buffer) and updates 
//the quantity of hordes according to the quantity of elementos of the current zombiesHordes
//assumes that the file was already opened correctly (with reading permission)
void importZombiesFromFile(FILE *file, int zombiesHordes[QUANTITY_MAX_HORDES], int *quantityOfHordes){
    int j=0;
   while(!(feof(file))&&j<QUANTITY_MAX_HORDES){// varre o arquivo            
	fscanf(file, "%d ", &zombiesHordes[j]);
    j++;
   }
   (*quantityOfHordes)=j;
   timeToDecreaseFromSpawnRateZombieEachHorde= spawnRateZombie/(*quantityOfHordes);
   timeToIncreaseSpawnRateSunEachHorde=spawnRateSun/(*quantityOfHordes);
}

//checksIfInTop5:
//given a player and thethe top5 players, check if it's reached the top five of the leaderboard(true) or not(false)
int checksIfInTop5(PLAYER player, PLAYER topFivePlayers[5]){
    if((player.pointsOfPlayer>topFivePlayers[4].pointsOfPlayer)||
        (player.pointsOfPlayer==0&&topFivePlayers[4].pointsOfPlayer==0)){
        return 1;
    } 
    else{
        return 0;
    }
}

//reorderTop5:
//given the points of a player and the points of the top5 players, checks if he's reached the top five and updates the top five
void reorderTop5(PLAYER player, PLAYER topFivePlayers[5]){
    int i=0;
    if(checksIfInTop5(player,topFivePlayers)){

        //searches for the correct index of the new player to be put in the top 5 leaderboard
        while(i<4&&player.pointsOfPlayer<=topFivePlayers[i].pointsOfPlayer){
            i++;
        }

        //reorder the rest of the players of the leaderboard
    // calculate how many items must be moved
    int numToMove = 4-i;

    if (numToMove < 5) {
        memmove(
            &topFivePlayers[i+1],          // dest
            &topFivePlayers[i],      // origin
            numToMove * sizeof(PLAYER)                   // quantity of bytes
        );
    }
        //add the new player to it's correct place
        topFivePlayers[i]=player;
    }
}
//importPlayersFromFile:
//given a file and an array(buffer), import it's top five players to the array (buffer)
//assumes that the file was already opened correctly (with reading permission)
//return the quantity of elements read from the file (accordingly to fread from stdio.h)
int importPlayersFromFile(FILE *file, PLAYER players [5]){
    return fread(players,sizeof(PLAYER),5,file);
}
//recordPlayersToFile
//given a file and a list of players, record them in the file
//assumes that the file was already opened correctly (with writing permission)
//return the quantity of elements written to the file (accordingly to fread from stdio.h)
int recordPlayersToFile(FILE *file,PLAYER players[5]){
    //rewind the file to overwrite the informations
    rewind(file);
    return fwrite(players,sizeof(PLAYER),5,file);
}

//drawTopFivePlayers:
//given the top five players, displays them in the screen
void drawTopFivePlayers(PLAYER players[5]){
    int maxCharactersText=40;
    int fontOfText = 20;
    //Box display where names will be put
    int BOX_DISPLAY_WIDTH =SCREEN_WIDTH/1.3;
    int marginFromTitle = 0.1*SCREEN_HEIGHT;
    int marginFromBottom = SCREEN_HEIGHT/20;
    int BOX_DISPLAY_CENTERED_X = (SCREEN_WIDTH-BOX_DISPLAY_WIDTH)/2;
    int BOX_DISPLAY_HEIGHT = SCREEN_HEIGHT-marginFromTitle-marginFromBottom;
    Rectangle BOX_DISPLAY_RECTANGLE = {.x=BOX_DISPLAY_CENTERED_X,.y=marginFromTitle,.width=BOX_DISPLAY_WIDTH,.height=BOX_DISPLAY_HEIGHT};
    //margin from title from homepage and menu
    int marginFromBoxDisplay=40;
    int TEXTBOX_HEIGHT =(SCREEN_WIDTH-marginFromTitle-marginFromBoxDisplay)/fontOfText;
    int TEXTBOX_INITIAL_Y=SCREEN_HEIGHT/2.2;
    int TEXTBOX_DYNAMIC_Y=TEXTBOX_INITIAL_Y;
    
    DrawTexturePro(TEXTURE_LEADERBOARD_BACKGROUND_IMG,TEXTURE_LEADERBOARD_BACKGROUND_IMG_SOURCE_REC,BOX_DISPLAY_RECTANGLE,origin,0.0f,WHITE);
    
    char playerDisplayFormat[maxCharactersText];
    for (int i = 0; i < 5; i++) {
        //if the player is invalid, don't display it
        if(players[i].playerName[0]=='\0'){
            continue;
        }
        snprintf(playerDisplayFormat, sizeof(playerDisplayFormat), "%dº: %s - %d pontos", i+1, players[i].playerName, players[i].pointsOfPlayer);
        int widthOfText = MeasureText(playerDisplayFormat, fontOfText);
        int TEXTBOX_ALONE_CENTERED_X = (SCREEN_WIDTH-widthOfText)/2;
        DrawText(playerDisplayFormat,TEXTBOX_ALONE_CENTERED_X,TEXTBOX_DYNAMIC_Y,fontOfText,BLACK);
        TEXTBOX_DYNAMIC_Y+=TEXTBOX_HEIGHT;
    }
}

//a cada jogo resetado (acabado) eu atualizo o array virtual, mas somente quando o user sai que eu gravo no arquivo
//o array dos top 5 jogadores (array virtual).
//uso esse array virtual para tudo, pois me baseio nele quando vou
//fazer um novo arquivo para o leaderboard a cada vez
//SO FECHO O ARQUIVO QUANDO O PROGRAMA ACABA (NA ABA DE DE-INITIALIZATION)