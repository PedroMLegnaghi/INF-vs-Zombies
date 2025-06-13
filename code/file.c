//teremos um arquivo binario que contem o nome do jogador e a sua pontuacao. 
//teremos um arquivo de texto que recebe a quantidade de zombies que deve vir por horda, apos a ultima horda, o jogo acaba
#include "stdio.h"
#include "Player.h"
FILE *leaderBoardFile;

//checksIfInTop5:
//given the points of a player and the points of the top5 players, check if it's reached the top five of the leaderboard(true) or not(false)
int checksIfInTop5(int pointsOfPlayer, int pointsOfCurrTop5[5]){
    if(pointsOfPlayer>pointsOfCurrTop5[4]){
        return 1;
    } else{
        return 0;
    }
}

//reorderTop5:
//given the points of a player and the points of the top5 players, checks if he's reached the top five and updates the top five
void reorderTop5(int pointsOfPlayer, int pointsOfCurrTop5[5]){
    
}

//recordPlayerToLeaderboard:
//given the name of the player and it's points, record in the leaderboard(if he's reached the top 5 biggest points)
void recordPlayerToLeaderboard(char nameOfPlayer[MAX_SIZE_OF_NAME], int points){

}

//a cada jogo resetado (acabado) eu recordo o player e atualizo
//o array dos top 5 jogadores (array virtual).
//uso esse array virtual para tudo, pois me baseio nele quando vou
//fazer um novo arquivo para o leaderboard a cada vez
//SO FECHO O ARQUIVO QUANDO O PROGRAMA ACABA (NA ABA DE DE-INITIALIZATION)