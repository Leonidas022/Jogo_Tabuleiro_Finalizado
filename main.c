#include<stdio.h>
#include<stdlib.h>
#include"trilha.h"
#include<time.h>
#include"efeitos.h"

int jogar_dado(int n);
void verificarJogador(int njogadores, trilha p[]);
void print_efeito(jogador *j, int njogadores, int *atingido);
void verificarGanhador(int p, int *vencedor, int classificacao[], int *lugar,
int jogador, int *faltam);
int armadilhas[8] = {4, 10, 15, 18, 34, 42, 46, 54}; //enumeração de todas as armadilhas (os numeros representam as posições)
int ativaArmadilha[4] = {0}; //algumas armadilhas precisam ser ativadas antes de pegar algum jogador. esse vetor armazena seelas foram ativadas (1) ou não (0). A relação da ordem está assim: [0] é 4, [1] é 18, [2] é 34 e [3] é 42
int armadilhaFoiUsada[8] = {0}; // armazena o status sobre se a armadilha oi usada ou não. A relação da ordem é a mesma da indicada no vetor armadilhas
void troca(int *pfoi, int *pvai, int jogador, int proximo, trilha p[]);

int main(){
    jogador pl[4] = {0};
    int njogadores = 0, restantes, comando, posicao, dado[2], atingido = -1;
    int temp, vencedor = 0, classificacao[4] = {0}, lugar = 0;
    trilha p[4];
    
    //Definicao de jogadores. Deve ser entre 2 a 4 jogadores.
    while((njogadores < 2)||(njogadores > 4)){
        printf("Nº de jogadores:");
        scanf("%d", &njogadores);
        if(njogadores < 2){
            printf("Numero insuficiente. Deve haver pelo menos 2 jogadores\n");
        }else if(njogadores > 4){
            printf("Ha jogadores demais. Deve haver ate 4 jogadores\n");
        }
    }
    //Inicializacao dos vetores de cada jogador na biblioteca trilha.h
    for(int i = 0; i < njogadores; i++){
        inicio(&p[i]);
    }
    restantes = njogadores;
    /*Inicio do jogo. A partida sera realizada ate o penultimo jogador completar
    o tabuleiro*/
    while(restantes > 1){
        //Looping que intercala os jogadores
        for(int i = 0; i < njogadores; i++){
            /*Jogador 1. Verifica se ainda nao completou o jogo e se ainda ha mais
            um jogador na partida*/
            if((i==0)&&(restantes > 1)){
                if(valorUltimo(&p[i]) > 58){
                    continue;
                //Verifica se o jogador esta preso no buraco negro. Caso afirmativo, perde a rodada
                }else if(pl[i].semMover){
                    printf("Jogador %d esta no buraco negro e perdeu a rodada\n\n", i+1);
                    pl[i].semMover--;
                    continue;
                /*Verifica se o jogador perdeu a rodada apos ser atingido por um raio congelante*/
                }else if(i == atingido){
                    printf("Voce foi atingido por um raio congelate e perdeu a rodada\n");
                    atingido = -1;
                    continue;
                /*Verifica se o jogador esta preso. Caso afirmativo, ele podera perder
                ate tres rodadas. Pode sair da prisao antecipadamente se tirar dois
                dados com valores iguais*/
                }else if(pl[i].prisao){
                    printf("Jogador %d esta na prisao por %d rodadas. Pode tentar a sorte tirando os dados\n", i+1, pl[i].prisao);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar(); // Aguarda o usuário apertar ENTER
                    dado[0] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[0]);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar(); // Aguarda o usuário apertar ENTER
                    dado[1] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[1]);
                    if(dado[0] == dado[1]){
                        printf("Parabens! Voce tirou dois dados iguais e saiu da prisao!\n\n");
                        pl[i].prisao = 0;
                        i--;
                        continue;
                    }else{
                        printf("Que pena! Voce tirou dois dados diferentes e segue na prisao!\n");
                        pl[i].prisao--;
                    }
                    continue;
                }else{
                    comando = 0;
                    /*Inicio da jogada. O jogador recebe sua posicao atual e 
                    pode decidir se ira jogar os dados ou se ira verificar a posi-
                    cao de um jogador a sua escolha. A sua jogada termina apenas
                    quando os dados forem jogados*/
                    printf("\nVez do jogador %d\n", i+1);
                    pl[i].posicao = valorUltimo(&p[i]);
                    printf("Voce esta na casa %d\n\n", pl[i].posicao);
                    while(comando != 1){
                        printf("Digite que deseja fazer\n");
                        printf("1 - Jogar dado\n2 - Verificar posicao de um jogador\n");
                        scanf("%d", &comando);
                        switch(comando){
                            case 1 :
                                pl[i].posicao += jogar_dado(0);
                                /*Jogador ganha apenas se tirar o valor exato para
                                atingir a ultima casa. Caso contrario, o valor "ex-
                                cedente" sera subtraido da ultima posicao para de-
                                finir sua posicao atual*/
                                if(pl[i].posicao > 59){
                                    int sobra = pl[i].posicao - 59;
                                    pl[i].posicao = 59 - sobra;
                                }
                                //Recebe o efeito da casa no qual caiu
                                print_efeito(&pl[i], njogadores, &atingido);
                                //Insere a casa atual na sua trilha
                                insere(&p[i], pl[i].posicao);
                                printf("Sua nova posicao: %d\n", pl[i].posicao);
                                /*Jogador recem chegado nao pode compartilhar posi-
                                cao no tbuliro com nenhum outro jogador. Caso a- 
                                conteca, o jogador que anteriormente estava naque-
                                la posicao recua uma casa*/
                                for(int j = 0; j<njogadores; j++){
                                    if(i!=j && pl[i].posicao == pl[j].posicao){
                                    pl[j].posicao--;
                                    insere(&p[j], pl[j].posicao);
                                    printf("Alcancado pelo jogador %d, o jogador %d que estava na casa %d tera que recuar uma posicao\n", i+1, j+1, pl[i].posicao);
                                    }
                                }
                              
                                if(pl[i].posicao == 21 || pl[i].posicao == 48){
                                    troca(&pl[i].posicao, &pl[(i+1)%njogadores].posicao, i,(i+1)%njogadores, p);
                                }
                                //Recebe a nova posicao e encerra sua jogada
                                //Verifica se o jogador terminou o  jogo.
                                verificarGanhador(pl[i].posicao, &vencedor, classificacao,
                                &lugar, i+1, &restantes);
                                printf("Aperte ENTER para continuar\n");
                                getchar(); // Aguarda o usuário apertar ENTER
                                printf("\n\n\n\n\n\n\n\n");
                                break;
                            
                            case 2 :
                                verificarJogador(njogadores, p);
                                break;
                            
                            default :
                                printf("Comando invalido\n\n");
                                break;
                        }
                    }
                    
                }
            }
            /*Os outros "if"s presentes nesse looping seguem o mesmo procedimento
            explicado paa o jogador 1*/
            if((i==1)&&(restantes > 1)){
                if(valorUltimo(&p[i]) > 58){
                    continue;
                }else if(pl[i].semMover){
                    printf("Jogador %d esta no buraco negro e perdeu a rodada\n\n", i+1);
                    pl[i].semMover--;
                    continue;
                }else if(i == atingido){
                    printf("Voce foi atingido por um raio congelate e perdeu a rodada\n");
                    atingido = -1;
                    continue;
                }else if(pl[i].prisao){
                    printf("Jogador %d esta na prisao por %d rodadas. Pode tentar a sorte tirando os dados\n", i+1, pl[i].prisao);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar(); // Aguarda o usuário apertar ENTER
                    dado[0] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[0]);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar(); // Aguarda o usuário apertar ENTER
                    dado[1] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[1]);
                    if(dado[0] == dado[1]){
                        printf("Parabens! Voce tirou dois dados iguais e saiu da prisao!\n\n");
                        pl[i].prisao = 0;
                        i--;
                        continue;
                    }else{
                        printf("Que pena! Voce tirou dois dados diferentes e segue na prisao!\n");
                        pl[i].prisao--;
                    }
                    continue;
                }else{
                    comando = 0;
                    printf("\nVez do jogador %d\n", i+1);
                    pl[i].posicao = valorUltimo(&p[i]);
                    printf("Voce esta na casa %d\n\n", pl[i].posicao);
                    
                    while(comando != 1){
                        printf("Digite que deseja fazer\n");
                        printf("1 - Jogar dado\n2 - Verificar posicao de um jogador\n");
                        scanf("%d", &comando);
                        switch(comando){
                            case 1 :
                                pl[i].posicao += jogar_dado(0);
                                if(pl[i].posicao > 59){
                                    int sobra = pl[i].posicao - 59;
                                    pl[i].posicao = 59 - sobra;
                                }
                                print_efeito(&pl[i], njogadores, &atingido);
                                insere(&p[i], pl[i].posicao);
                                printf("Sua nova posicao: %d\n", pl[i].posicao);
                                for(int j = 0; j<njogadores; j++){
                                    if(i!=j && pl[i].posicao == pl[j].posicao){
                                    pl[j].posicao--;
                                    insere(&p[j], pl[j].posicao);
                                    printf("Alcancado pelo jogador %d, o jogador %d que estava na casa %d tera que recuar uma posicao\n", i+1, j+1, pl[i].posicao);
                                    }
                                }
                              
                                if(pl[i].posicao == 21 || pl[i].posicao == 48){
                                    troca(&pl[i].posicao, &pl[(i+1)%njogadores].posicao, i,(i+1)%njogadores, p);
                                }
                                verificarGanhador(pl[i].posicao, &vencedor, classificacao,
                                &lugar, i+1, &restantes);
                                printf("Aperte ENTER para continuar\n");
                                getchar();
                                printf("\n\n\n\n\n\n\n\n");                                
                                break;
                            
                            case 2 :
                                verificarJogador(njogadores, p);
                                break;
                            
                            default :
                                printf("Comando invalido\n\n");
                                break;
                        }
                    }
                    
                }
            }
            if((i==2)&&(restantes > 1)){
                if(valorUltimo(&p[i]) > 58){
                    continue;
                }else if(pl[i].semMover){
                    printf("Jogador %d esta no buraco negro e perdeu a rodada\n\n", i+1);
                    pl[i].semMover--;
                    continue;
                }else if(i == atingido){
                    printf("Voce foi atingido por um raio congelate e perdeu a rodada\n");
                    atingido = -1;
                    continue;
                }else if(pl[i].prisao){
                    printf("Jogador %d esta na prisao por %d rodadas. Pode tentar a sorte tirando os dados\n", i+1, pl[i].prisao);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar();
                    dado[0] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[0]);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar();
                    dado[1] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[1]);
                    if(dado[0] == dado[1]){
                        printf("Parabens! Voce tirou dois dados iguais e saiu da prisao!\n\n");
                        pl[i].prisao = 0;
                        i--;
                        continue;
                    }else{
                        printf("Que pena! Voce tirou dois dados diferentes e segue na prisao!\n");
                        pl[i].prisao--;
                    }
                    continue;
                }else{
                    comando = 0;
                    printf("\nVez do jogador %d\n", i+1);
                    pl[i].posicao = valorUltimo(&p[i]);
                    printf("Voce esta na casa %d\n\n", pl[i].posicao);
                    while(comando != 1){
                        printf("Digite que deseja fazer\n");
                        printf("1 - Jogar dado\n2 - Verificar posicao de um jogador\n");
                        scanf("%d", &comando);
                        switch(comando){
                            case 1 :
                                pl[i].posicao += jogar_dado(0);
                                if(pl[i].posicao > 59){
                                    int sobra = pl[i].posicao - 59;
                                    pl[i].posicao = 59 - sobra;
                                }
                                print_efeito(&pl[i], njogadores, &atingido);
                                insere(&p[i], pl[i].posicao);
                                printf("Sua nova posicao: %d\n", pl[i].posicao);
                                for(int j = 0; j<njogadores; j++){
                                    if(i!=j && pl[i].posicao == pl[j].posicao){
                                    pl[j].posicao--;
                                    insere(&p[j], pl[j].posicao);
                                    printf("Alcancado pelo jogador %d, o jogador %d que estava na casa %d tera que recuar uma posicao\n", i+1, j+1, pl[i].posicao);
                                    }
                                }
                              
                                if(pl[i].posicao == 21 || pl[i].posicao == 48){
                                    troca(&pl[i].posicao, &pl[(i+1)%njogadores].posicao, i,(i+1)%njogadores, p);
                                }
                                verificarGanhador(pl[i].posicao, &vencedor, classificacao,
                                &lugar, i+1, &restantes);
                                printf("Aperte ENTER para continuar\n");
                                getchar();
                                printf("\n\n\n\n\n\n\n\n");                                
                                break;
                            
                            case 2 :
                                verificarJogador(njogadores, p);
                                break;
                            
                            default :
                                printf("Comando invalido\n\n");
                                break;
                        }
                    }
                    
                }
            }
            if((i==3)&&(restantes > 1)){
                if(valorUltimo(&p[i]) > 58){
                    continue;
                }else if(pl[i].semMover){
                    printf("Jogador %d esta no buraco negro e perdeu a rodada\n\n", i+1);
                    pl[i].semMover--;
                    continue;
                }else if(i == atingido){
                    printf("Voce foi atingido por um raio congelate e perdeu a rodada\n");
                    atingido = -1;
                    continue;
                }else if(pl[i].prisao){
                    printf("Jogador %d esta na prisao por %d rodadas. Pode tentar a sorte tirando os dados\n", i+1, pl[i].prisao);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar();
                    dado[0] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[0]);
                    printf("Pressione enter para tirar o dado 1\n");
                    getchar();
                    dado[1] = (rand()%6 + 1);
                    printf("O valor obtido foi %d\n", dado[1]);
                    if(dado[0] == dado[1]){
                        printf("Parabens! Voce tirou dois dados iguais e saiu da prisao!\n\n");
                        pl[i].prisao = 0;
                        i--;
                        continue;
                    }else{
                        printf("Que pena! Voce tirou dois dados diferentes e segue na prisao!\n");
                        pl[i].prisao--;
                    }
                    continue;
                }else{
                    comando = 0;
                    printf("\nVez do jogador %d\n", i+1);
                    pl[i].posicao = valorUltimo(&p[i]);
                    printf("Voce esta na casa %d\n\n", pl[i].posicao);
                    while(comando != 1){
                        printf("Digite que deseja fazer\n");
                        printf("1 - Jogar dado\n2 - Verificar posicao de um jogador\n");
                        scanf("%d", &comando);
                        switch(comando){
                            case 1 :
                                pl[i].posicao += jogar_dado(0);
                                if(pl[i].posicao > 59){
                                    int sobra = pl[i].posicao - 59;
                                    pl[i].posicao = 59 - sobra;
                                }
                                print_efeito(&pl[i], njogadores, &atingido);
                                insere(&p[i], pl[i].posicao);
                                printf("Sua nova posicao: %d\n", pl[i].posicao);
                                for(int j = 0; j<njogadores; j++){
                                    if(i!=j && pl[i].posicao == pl[j].posicao){
                                    pl[j].posicao--;
                                    insere(&p[j], pl[j].posicao);
                                    printf("Alcancado pelo jogador %d, o jogador %d que estava na casa %d tera que recuar uma posicao\n", i+1, j+1, pl[i].posicao);
                                    }
                                }
                              
                                if(pl[i].posicao == 21 || pl[i].posicao == 48){
                                    troca(&pl[i].posicao, &pl[(i+1)%njogadores].posicao, i,(i+1)%njogadores, p);
                                }
                                verificarGanhador(pl[i].posicao, &vencedor, classificacao,
                                &lugar, i+1, &restantes);
                                printf("Aperte ENTER para continuar\n");
                                getchar();
                                printf("\n\n\n\n\n\n\n\n");                                
                                break;
                            
                            case 2 :
                                verificarJogador(njogadores, p);
                                break;
                            
                            default :
                                printf("Comando invalido\n\n");
                                break;
                        }
                    }
                    
                }
            }
        }
    }
    //Adiciona o jogador que na concluiu a jogatina a classificacao final.
    for(int i = 0; i < njogadores; i++){
        if((valorUltimo(&p[i])) < 59){
            classificacao[lugar] = i+1;
            lugar++;
        }
    }
    //Exibe a classificacao final e a trilha percorrida por cada jogador no tabuleiro 
    printf("Fim de jogo!\nO vencedor foi o jogador %d, Parabens!\n", vencedor);
    printf("classificacao final:\n");
    for(int i = 0; i < njogadores; i++){
        printf("%dº Lugar: %d\n", i+1, classificacao[i]);
    }
    for(int i = 0; i < njogadores; i++){
        printf("Caminho do jogador %d:\n 0 ", i+1);
        while(!vazio(&p[i])){
            printf("- %d ", valorInicio(&p[i]));
            int trash = retira(&p[i]);
        }
        fim(&p[i]);
        printf("\n");
    }
return 0;
}

int jogar_dado(int n) {
    printf("Aperte ENTER para jogar o dado\n");
    getchar(); // Aguarda o usuário apertar ENTER
    getchar(); // Lê o caractere ENTER que foi digitado pelo usuário
    srand(time(NULL));
    n = (rand()%6 + 1);
    printf("O valor tirado foi %d.\n", n);
    return n;
} 

void print_efeito(jogador *j, int njogadores, int *atingido){
	//Jogador recebe um efeito a depender da posicao em que "caiu" no tabuleiro.
	if(((j -> posicao > 12)&&(j -> parada[0] == 0))||((j -> posicao > 27)&&(j -> parada[1] == 0))||
	((j -> posicao > 39)&&(j -> parada[2] == 0))||((j -> posicao > 49)&&(j -> parada[3] == 0))){
	    if((j -> posicao > 12)&&(j -> parada[0] == 0)){
	        pontoDeParada(j);
            printf("Parada obrigatoria! Casa 13 para recarregar bateria\n");
            return;
	    }else if((j -> posicao > 27)&&(j -> parada[1] == 0)){
	        pontoDeParada(j);
	        printf("Parada obrigatoria! Casa 28 para redirecionar a nave\n");
	        return;
	    }else if((j -> posicao > 39)&&(j -> parada[2] == 0)){
	        pontoDeParada(j);
	        printf("Parada obrigatoria! Casa 40 para manutencao\n");
	        return;
	    }else if((j -> posicao > 49)&&(j -> parada[3] == 0)){
	        pontoDeParada(j);
	        printf("Parada obrigatoria! Casa 50 para abastecimento\n");
	        return;
	    }
	}else if ((j->posicao == 3) ||(j->posicao == 9) || (j->posicao == 14) ||
 (j->posicao == 25) || (j->posicao == 40) || (j->posicao == 44)){
        energiaEstelar(j);
		printf("Energia estelar! Voce avanca duas casas, sua nova posicao e %d\n", j -> posicao);
		return;
    }
	else if((j->posicao == 8) || (j->posicao == 20) || (j->posicao == 27) ||
(j->posicao == 36) || (j->posicao == 41) || (j->posicao == 55)){
        asteroides(j);
		printf("Asteroides! Voce recua duas casas e sua nova posicao e %d\n", j -> posicao);
		return;
	}
	else if((j->posicao == 29) || (j->posicao == 39)){
	    buracoDeMinhoca(j);
		printf("Que sorte! Voce achou um buraco de minhoca e se teletransportou para a casa %d\n", j->posicao);
		return;
	}else if(j->posicao == 49){
	    buracoDeMinhoca(j);
	    printf("Que azar! Voce achou um buraco de minhoca reverso e se teletransportou para a casa 39\n");
	    return;
	}
	else if((j->posicao == 24) ||(j->posicao == 33) || (j->posicao == 43) || (j->posicao == 58)){
	    buracoNegro(j);
		printf("Voce acabou caindo em um buraco negro e perdera uma rodada!\n");
		return;
	}
	else if((j->posicao == 17) || (j->posicao == 51)){
	    prisao(j);
	    printf("Voce foi preso e podera perder ate 3 rodadas\n");
	    return;
	
    //especiais
    /*Armadilhas: Primeiro jogador que cair nas casas 10,15, 46 e 54 caem na amadilha
    e podem voltar 1(casas 10 e 15),2(casa46), ou 3(casa54) casas.
    Ja se cair nas casas 4, 18, 34 e 42 plantam uma armadilha para, caso outro jogador
    "caia" naquela casa apos a bomba plantada perca 1(casas 4 e 34) ou duas(casas
    18 e 42) casas*/
	}else if((j->posicao == 4 && ativaArmadilha[0] && !armadilhaFoiUsada[0]) || (j->posicao == 10 && !armadilhaFoiUsada[1]) ||
	(j->posicao == 15 && !armadilhaFoiUsada[2]) || (j->posicao ==18 && ativaArmadilha[1] && !armadilhaFoiUsada[3]) ||
	(j->posicao == 34 && ativaArmadilha[2] && !armadilhaFoiUsada[4]) ||
	(j->posicao == 42 && ativaArmadilha[3] && !armadilhaFoiUsada[5]) ||
	(j->posicao == 46 && !armadilhaFoiUsada[6]) || (j->posicao == 54 && !armadilhaFoiUsada[7]) ){
      int v;
      if(j->posicao == 4 || j->posicao ==10 || j->posicao == 15 || j->posicao == 34){
        v = 1;
      }else if(j->posicao == 18 || j->posicao == 46 || j->posicao == 48){
        v = 2;
      }else{
        v = 3;
      }
    for(int i=0; i<8; i++){
      if(armadilhas[i] == j->posicao){
        armadilhaFoiUsada[i] = 1;
      }
    }
      printf("Opa, voce caiu na armadilha! Volte %d casas\n", v);
      j->posicao -= v;
  }else if(j->posicao == 4 && !ativaArmadilha[0]|| j->posicao == 18 && !ativaArmadilha[1]||
  j->posicao == 34 && !ativaArmadilha[2]|| j->posicao == 42 && !ativaArmadilha[3]){
    if(j->posicao == 4){
      ativaArmadilha[0] = 1;
      printf("Voce ativou a armadilha na casa 4\n");
    }else if(j->posicao == 18){
      ativaArmadilha[1] = 1;
      printf("Voce ativou a armadilha na casa 18\n");
    }
    else if(j->posicao==34){
      ativaArmadilha[2] = 1;
      printf("Voce ativou a armadilha na casa 34\n");
    }else if(j->posicao ==42){
      ativaArmadilha[3] = 1;
      printf("Voce ativou a armadilha na casa 42\n");
    }
  }
  /*Jogador ganha a possibilidade de tirar uma rodada de um adversario. Obs: Ele pode
  escolher si mesmo para perder a rodada*/
    else if((j -> posicao == 10)||(j -> posicao == 45)){
        while((*atingido < 1)||(*atingido > njogadores)){
            printf("Voce possui o raio congelante. Escolha um jogador para perder uma rodada:\n");
            int r;
            scanf("%d", &r);
            *atingido = r - 1;
            if((*atingido < 1)||(*atingido > njogadores)){
                printf("Valor invalido\n");
            }
        }
   }
	return;
}
void verificarJogador(int njogadores, trilha p[]){
    //Escolha de qual jogador se quer verifcar a posicao atual.
    int posicao = 0;
    while((posicao > njogadores)||(posicao < 1)){
        printf("Qual jogador eseja verificar?\n");
        scanf("%d", &posicao);
            if((posicao > njogadores)||(posicao < 1)){
                printf("Comando invalido\n\n");
                continue;
            }else{
                printf("O jogador %d se encontra na posicao %d\n\n", posicao, valorUltimo(&p[posicao - 1]));
            }
    }
    return;
}
void verificarGanhador(int p, int *vencedor, int classificacao[], int *lugar,
int jogador, int *faltam){
    /*Analisa se o jogador atingiu a casa final. Caso  positivo, verifica qual a 
    sua posicao final na partida. Caso negaativo, sai da funcao sem realizar qual-
    quer efeito*/
    if(p == 59){
        if(!(*vencedor)){
            printf("Parabens! Voce venceu o jogo!\n");
            *vencedor = jogador;
        }else{
            printf("Voce terminou o jogo!\n");}
        
        classificacao[*lugar] = jogador;
        *lugar += 1;
        *faltam -= 1;
    }
}

//Troca: qual o jogador cai na casa 21 ou 48, ele troca de posição com o jogador da próxima vez
void troca(int *pfoi, int *pvai, int jogador, int proximo, trilha p[]){
  //trocam os valores das variaveis referentes às posições a qual os seus endereços são carregos pelos ponteiro
  int aux = *pfoi;
  *pfoi = *pvai;
  *pvai = aux;
  //insere os valores das posições na trilha
  insere(&p[jogador], *pfoi);
  insere(&p[proximo], *pvai);
  //imprime a troca de posições que aconteceu
  printf("Os jogadores %d e %d trocaram de posicoes. Agora o jogador %d está em %d e o jogador %d pegou a %d posicao.\n", jogador+1, proximo+1, jogador+1, *pfoi, proximo+1, *pvai);
}