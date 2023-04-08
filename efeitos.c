#include"efeitos.h"

//Avanca o jogador em duas casas
void energiaEstelar(jogador *j){
    j->posicao += 2;
    return;
}
//Recua o jogador em duas casas
void asteroides(jogador *j){
    j->posicao -= 2;
    return;
}
//Avanca o jogador em 10 casas ou recua em 10 casas, se cair no buraco da casa 49
void buracoDeMinhoca(jogador *j){
    if((j->posicao == 29) || (j->posicao == 39)){
        j->posicao += 10;
    }else if(j->posicao == 49){
        j->posicao -= 10;
    }
    return;
}
//Jogador perde uma rodada
void buracoNegro(jogador *j){
    j -> semMover = 1;
    return;
}
//Jogador pode perder at 3 rodadas, a depender da sorte dos dados
void prisao(jogador *j){
    j -> prisao = 3;
    return;
}
/*Parada obrigatoria para ajustes na nave. Jogador devera parar obrigatoriamente
nessa casa, independende de ter "caido" em uma casa maior*/
void pontoDeParada(jogador *j){
    if((j -> posicao > 12)&&(j -> parada[0] == 0)){
        j -> posicao = 13;
        j -> parada[0] = 1;
    }else if((j -> posicao > 27)&&(j -> parada[1] == 0)){
        j -> posicao = 28;
        j -> parada[1] = 1;
    }else if((j -> posicao > 39)&&(j -> parada[2] == 0)){
        j -> posicao = 40;
        j -> parada[2] = 1;
    }else if((j -> posicao > 49)&&(j -> parada[3] == 0)){
        j -> posicao = 50;
        j -> parada[3] = 1;
    }
    return;
}
