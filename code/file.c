//teremos um arquivo binario que contem o nome do jogador e a sua pontuacao. 
//teremos um arquivo de texto que recebe a quantidade de zombies que deve vir por horda, apos a ultima horda, o jogo acaba
#include "file.h"
#include "string.h"
#include "Textures.h"
#include "raylib.h"
FILE *leaderBoardFile;
PLAYER leaderBoardTop5Players[5]={0};

//checksIfInTop5:
//given a player and thethe top5 players, check if it's reached the top five of the leaderboard(true) or not(false)
int checksIfInTop5(PLAYER player, PLAYER topFivePlayers[5]){
    if(player.pointsOfPlayer>topFivePlayers[4].pointsOfPlayer){
        return 1;
    } else{
        return 0;
    }
}

//reorderTop5:
//given the points of a player and the points of the top5 players, checks if he's reached the top five and updates the top five
void reorderTop5(PLAYER player, PLAYER topFivePlayers[5]){
    int i=4;
    if(checksIfInTop5(player,topFivePlayers)){

        //searches for the correct index of the new player to be put in the top 5 leaderboard
        while(player.pointsOfPlayer>topFivePlayers[i].pointsOfPlayer){
            i--;
        }

        //reorder the rest of the players of the leaderboard
        for (int j=i;j<4;j++){
            topFivePlayers[j+1]=topFivePlayers[j];
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
    return fwrite(players,sizeof(PLAYER),5,file);
}

//drawTopFivePlayers:
//given the top five players, displays them in the screen
void drawTopFivePlayers(PLAYER players[5]){
    // players[0].pointsOfPlayer=100;
    // strcpy(players[0].playerName,"pedro");
    // players[1].pointsOfPlayer=300;
    // strcpy(players[1].playerName,"pasdo");
    // players[2].pointsOfPlayer=200;
    // strcpy(players[2].playerName,"qweo");
    // players[3].pointsOfPlayer=100;
    // strcpy(players[3].playerName,"pedro");
    // players[4].pointsOfPlayer=100;
    // strcpy(players[4].playerName,"pedro");
    int maxCharactersText=30;
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