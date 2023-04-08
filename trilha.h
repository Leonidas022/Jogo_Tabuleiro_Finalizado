//construido com auxilio do exemplo em aula sobre "Encadeamento simples".
#ifndef TRILHA_H //Se ja esta definida essa biblioteca, pula para "endif".
#define TRILHA_H

//Struct insere e remove elementos da fila.
typedef struct mover{
    int valor;
    struct mover *posicao;
}mover;

//Struct salva as posicoes inicial e final da fila.
typedef struct trilha{
    mover *comeco, *final;
    unsigned posicao;
}trilha;

//abaixo, todas as funcoes presentes na biblioteca.
int vazio(trilha *t);
int tamanho(trilha *t);
void inicio(trilha *t);
int insere(trilha *t, int numero);
int retira(trilha *t);
int valorInicio(trilha *t);
int valorUltimo(trilha *t);
void fim(trilha *t);

#endif