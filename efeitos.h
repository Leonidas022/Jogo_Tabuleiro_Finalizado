#ifndef EFEITOS_H //Se ja esta definida essa biblioteca, pula para "endif".
#define EFEITOS_H

typedef struct jogador{
    int posicao;
    int semMover;
    int prisao;
    int parada[4];
}jogador;

//abaixo, todas as funcoes presentes na biblioteca.
void energiaEstelar(jogador *j);
void asteroides(jogador *j);
void buracoDeMinhoca(jogador *j);
void buracoNegro(jogador *j);
void prisao(jogador *j);
void pontoDeParada(jogador *j);

#endif