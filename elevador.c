#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* To-Do List
- Criar/Receber pessoa (RELACIONADO COM LEITURA)
- Inicializar andares(n� de andares a ser determinado) 
- Posicionar elevador
- Adicinar/Retirar pessoa no elevador e andar
- Receber chamadas(demanda) -> Vai ser foda
- Definir destinos
- Definir sentidos
- Movimenta��o do elevador
*/
/*
elevador 1{
	andar 4
	chamadas 3,4,8
	subindo
}
elevador 2{
	andar 0
	chamadas 9,7
	subindo
}

Pessoa 1{
	andar 7
	andarDestino 20
	Tempo 4s
}

*/

typedef struct pessoa{
	int ID;
	int andarD;
}pessoa;

typedef struct listaPessoas{
	pessoa pessoa;
	listaPessoas *endProx;
	listaPessoas *endAnt;
}listaPessoas;

typedef struct andar{
	int nAndar;
	struct andar *prox;
	struct andar *ant;
	listaPessoas *pessoas;
}andar;

typedef struct listaAndares{
	int n;
	struct listaAndares *endProx;
	struct listaAndares *endAnt;
}listaAndares;

typedef struct elevador{
	int direcao;// -1 - Descendo, 0 - Parado, 1 - Subindo
	pessoa *pessoasDentro;
	andar *andarAtual;
	listaAndares *chamados;
	listaAndares *subindo;
	listaAndares *descendo;
	listaAndares **destino;//Vai apontar para o subindo ou descendo
}elevador;

typedef struct listaElevadores{
	elevador elevador;
	struct listaElevadores *endProx;
	struct listaElevadores *endAnt;
}listaElevadores;

//Funções:
andar* gerarAndar();

//
int main() {
	andar *predio = NULL;
	elevador elevador;
	
	return 0;
}


andar* gerarAndar(){
	andar *aux = malloc(sizeof(andar));
	return aux;
}

void inicializar(andar **inicializarPredio){
	int cont = 25;//Número de andares máximono Prédio
	while(cont >= 0){
		if(*inicializarPredio == NULL){
			andar *aux = gerarAndar();
			aux->nAndar = cont;
			aux->prox = NULL;
			aux->ant = NULL;
			*inicializarPredio = aux;
			aux->pessoas = NULL;
		}else{
			andar *aux = gerarAndar();
			aux->nAndar = cont;
			andar *aux2 = *inicializarPredio;
			aux2->ant = aux;
			aux->prox = *inicializarPredio;
			*inicializarPredio = aux;
			aux->ant = NULL;
			aux->pessoas = NULL;
		}
		cont--;
	}
	return;
}

