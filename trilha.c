//construido com auxilio do exemplo em aula sobre "encadeamento simples"
#include "trilha.h"
#include <stdlib.h>
//Verifica se a  esta vazia atraves da quantidade de elementos (posicao)
int vazio(trilha *t){
    return (t->posicao == 0);
}
//Informa a quantidade de elementos da trilha.
int tamanho(trilha *t){
    return (t->posicao);
}
//Inicializa a trilha.
void inicio(trilha *t){
    t->comeco = NULL;
    t->final = NULL;
    t->posicao = 0;
}
//Insere um elemento da trilha. Retorna 1 se a insercao foi sucedida e 0 se fracassada
int insere(trilha *t, int numero){
    //Cria um auxiliar a trilha, que ira armazenar o valor inserido no final da trilha
    mover *m = (mover*)malloc(sizeof(mover));
    if(m == NULL){
        return 0;
    }
    m->valor = numero;
    m->posicao = NULL;
    /*Verifica se ja havia ou nao elementos na trilha. Caso afirmativo, atualiza
    a posicao do ultimo elemento da trilha. Caso negativo, indica o inicio da trilha
    e insere o elemento*/
    if(t->final){
        t->final->posicao = m;
    }else{t->comeco = m;}
    t -> final = m;
    t -> posicao++;
    return 1;
}
/*Remove um elemento no inicio trilha. Retorna 0 se a trilha ja estava vazia e 1 se a 
remocao foi sucedida */
int retira(trilha *t){
    if(vazio(t)){
       return 0; 
    }
    /*Atualiza a posicao inicial da trilha, caso a trilha ja esteja vazia, atualiza
    tanto o inicio quanto o final com a posicao nula*/
    mover *m = t->comeco;
    t->comeco = m->posicao;
    free(m);
    if(t->comeco == NULL)
        t->final = NULL;
    
    t->posicao--;
    return 1;
}
/*Retorna o valor do primeiro elemento da trilha sem remove-lo. Retorna 0 se a trilha
estiver vazia. */
int valorInicio(trilha *t){
    if(t -> comeco == NULL){
            return 0;
        }
    return (t -> comeco -> valor);
}
/*Retorna o valor do ultimo elemento da trilha sem remove-lo. Retorna 0 se a trilha
estiver vazia.*/
int valorUltimo(trilha *t){
        if(t -> final == NULL){
            return 0;
        }
    return (t -> final -> valor);
}
//Esvazia a trilha atraves das funcoes "retira" e "vazio".
void fim(trilha *t){
    while(!vazio(t)){
        retira(t);
    }
}
