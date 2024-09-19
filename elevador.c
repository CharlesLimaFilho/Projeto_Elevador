#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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


//Pessoas:
typedef struct pessoa{
	int ID;
	int andarD;
	//int andarInicial;//Ideia
}pessoa;

typedef struct listaPessoas{
	pessoa pessoa;
	struct listaPessoas *endProx;
	struct listaPessoas *endAnt;
}listaPessoas;

//Andares:
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

typedef struct rota{
	int tempo;
	int nAndar;
	struct rota *endprox;
}rota;

//Cronograma(Roteiro):
typedef struct cronograma{
	int tempo;
	listaAndares *andaresIniciais;
	listaPessoas *pessoas;
	struct cronograma *endProx;
}cronograma;

//Elevadores:
typedef struct elevador{
	int ID;
	int direcao;// -1 - Descendo, 0 - Parado, 1 - Subindo
	int andaresPercorridos;
	listaPessoas *pessoasDentro;
	andar *andarAtual;
	listaAndares *chamados;
	listaAndares *reserva;
	listaAndares *subindo;
	listaAndares *descendo;
	listaAndares *destino;//Vai apontar para os andares Destinos das pessoas dentro do elevador
	rota *andaresPassados;
}elevador;

typedef struct listaElevadores{
	elevador elevador;
	struct listaElevadores *endProx;
	struct listaElevadores *endAnt;
}listaElevadores;


//Funções:
andar* gerarAndar();
void inicializar(andar **inicializarPredio);
void inserirPessoasAndares(cronograma **roteiro, andar **predio, int tempo, elevador *elevador);
void chamarElevador(int andarAtual, int andarDestino, elevador *elevador);
void separarChamadas(elevador *elevador, int andarDestino);
void inicializarElevador(elevador *elevador, andar **predio, int direcao, listaAndares **chamados, int andarAtual);
void inserirAndaresPassados(elevador *elev, int tempo);
void inserirPessoasElevador(elevador *insElevador);
void organizarDestino (elevador *elev);
void ordemPessoasElevador(elevador *elev);
void moverElevador(elevador *elev);
void tirarPessoa(elevador *elev, int tempo);
void definirSentido(elevador *elev);
void atualizar(elevador *elev, int tempo);
void imprimirElevador(elevador *elev);
//
int main() {
	andar *predio = NULL;
	inicializar(&predio);
	elevador elevador;
	int tempo = 0;
	int dtempo = 1;
	int tAtual;
	//teste:
	
	//Essas Pessoas seriam criadas na hora da leitura
	pessoa p1;
	p1.andarD = 2;
	p1.ID = 001;

	pessoa p2;
	p2.andarD = 7;
	p2.ID = 002;
	
	pessoa p3;
	p3.andarD = 3;
	p3.ID = 003;
	
	pessoa p4;
	p4.andarD = 10;
	p4.ID = 004;
	
	pessoa p5;
	p5.andarD = 1;
	p5.ID = 005;

	//Essas Listas de Pessoas seriam criadas na hora da leitura e acumulam Pessoas em determinado Tempo
	listaPessoas *l1 = NULL;
	l1 = malloc(sizeof(listaPessoas));
	l1->pessoa = p1;
	l1->endAnt = NULL;
	l1->endProx = malloc(sizeof(listaPessoas));
	l1->endProx->pessoa = p2;
	l1->endProx->endAnt = l1;
	l1->endProx->endProx = NULL;

	listaPessoas *l2 = NULL;
	l2 = malloc(sizeof(listaPessoas));
	l2->pessoa = p3;
	l2->endAnt = NULL;
	l2->endProx = NULL;
	
	listaPessoas *l3 = NULL;
	l3 = malloc(sizeof(listaPessoas));
	l3->pessoa = p4;
	l3->endAnt = NULL;
	l3->endProx = malloc(sizeof(listaPessoas));
	l3->endProx->pessoa = p5;
	l3->endProx->endProx = NULL;
	l3->endProx->endAnt = l3;

	//São as Listas de andares onde cada Pessoa estará inicialmente
	listaAndares *a1 = NULL;
	a1 = malloc(sizeof(listaAndares));
	a1->n = 9;
	a1->endAnt = NULL;
	a1->endProx = malloc(sizeof(listaAndares));
	a1->endProx->n = 5;
	a1->endProx->endAnt = a1;
	a1->endProx->endProx = NULL;

	listaAndares *a2 = NULL;
	a2 = malloc(sizeof(listaAndares));
	a2->n = 14;
	a2->endAnt = NULL;
	a2->endProx= NULL;

	listaAndares *a3 = NULL;
	a3 = malloc(sizeof(listaAndares));
	a3->n = 2;
	a3->endAnt = NULL;
	a3->endProx = malloc(sizeof(listaAndares));
	a3->endProx->n = 4;
	a3->endProx->endAnt = a3;
	a3->endProx->endProx = NULL;

	//Exemplo de Lista de Andares que o Elevador l vai ter como Destino inicialmente
	listaAndares *l = malloc(sizeof(listaAndares));
	l->endAnt = NULL;
	l->n = 3;
	l->endProx = malloc(sizeof(listaAndares));
	l->endProx->n = 8;
	l->endProx->endAnt = l;
	l->endProx->endProx = malloc(sizeof(listaAndares));
	l->endProx->endProx->n = 14;
	l->endProx->endProx->endAnt = l->endProx;
	l->endProx->endProx->endProx = NULL;
	
	
	//O Cronograma junta as Listas de Pessoas em determinado tempo T e seu andares iniciais
	cronograma *roteiro = malloc(sizeof(cronograma));
	roteiro->tempo = 4;
	roteiro->pessoas = l1;
	roteiro->andaresIniciais = a1;
	roteiro->endProx = malloc(sizeof(cronograma));
	roteiro->endProx->tempo = 8;
	roteiro->endProx->pessoas = l2;
	roteiro->endProx->andaresIniciais = a2;
	roteiro->endProx->endProx = malloc(sizeof(cronograma));
	roteiro->endProx->endProx->tempo = 9;
	roteiro->endProx->endProx->pessoas = l3;
	roteiro->endProx->endProx->andaresIniciais = a3;
	roteiro->endProx->endProx->endProx = NULL;

	inicializarElevador(&elevador,&predio,1,&l,1);
	
	while(roteiro != NULL || elevador.direcao != 0 ){
		tAtual = clock()/CLOCKS_PER_SEC;
		if(tAtual - tempo >= dtempo){
			inserirPessoasAndares(&roteiro,&predio,tAtual,&elevador);
			atualizar(&elevador, tAtual);
			tempo = tempo + 1;
		}
	}
	
	/*
	
	while(tempo <= 50){
		inserirPessoasAndares(&roteiro,&predio,tempo,&elevador);
		atualizar(&elevador,tempo);
		tempo++;
	}
	*/

	
	
	return 0;
}


andar* gerarAndar(){
	andar *aux = malloc(sizeof(andar));
	return aux;
}

void inicializar(andar **inicializarPredio){
	int cont = 15;//Número de andares máximono Prédio
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

void inserirPessoasAndares(cronograma **roteiro, andar **predio, int tempo, elevador *elevador){
	if(*roteiro != NULL){
		cronograma *auxCronograma = *roteiro;
		if(tempo >= auxCronograma->tempo){//Pode não ser exatamente o mesmo tempo sei lá, vai que
			while(auxCronograma->pessoas != NULL){
				//Achar os andares de cada pessoa da lista naquele tempo:
				andar *aux = *predio;
				while(aux->nAndar != auxCronograma->andaresIniciais->n){
					aux = aux->prox;
				}
				//Ver se está vazio ou não para saber onde adicionar na lista de pessoas do andar:
				if(aux->pessoas == NULL){
					listaPessoas *auxPessoas = malloc(sizeof(listaPessoas));
					auxPessoas->pessoa = auxCronograma->pessoas->pessoa;
					auxPessoas->endProx = NULL;
					auxPessoas->endAnt = NULL;
					aux->pessoas = auxPessoas;
					chamarElevador(aux->nAndar,auxPessoas->pessoa.andarD,elevador);
				}else{
					listaPessoas *auxPessoas = malloc(sizeof(listaPessoas));
					auxPessoas->pessoa = auxCronograma->pessoas->pessoa;
					auxPessoas->endProx = aux->pessoas;
					auxPessoas->endAnt = NULL;
					aux->pessoas->endAnt = auxPessoas;
					aux->pessoas = auxPessoas;
					chamarElevador(aux->nAndar,auxPessoas->pessoa.andarD,elevador);
				}
				//Ir para a próxima pessoa, se tiver, nesse mesmo instante:
				listaPessoas *aux2 = auxCronograma->pessoas;
				auxCronograma->pessoas = auxCronograma->pessoas->endProx;
				free(aux2);
				//Ir para o andar da próxima pessoa, se tiver, no mesmo instante:
				listaAndares *aux3 = auxCronograma->andaresIniciais;
				auxCronograma->andaresIniciais = auxCronograma->andaresIniciais->endProx;	
				free(aux3);
			}
		//Não faço menor ideia do porquê não funcionar, se conseguir ajeitar, vá em frente:
		auxCronograma->tempo = 0;
		*roteiro = auxCronograma->endProx;
		}	
	}
	return;
}

void chamarElevador(int andarAtual, int andarDestino, elevador *elevador){
	if(elevador->direcao == 0){
		if(elevador->andarAtual->nAndar >= andarAtual){
			elevador->direcao = -1;
		}else{
			elevador->direcao = 1;
		}
	}
	listaAndares *aux = malloc(sizeof(listaAndares));
	aux->n = andarAtual;
	aux->endProx = NULL;
	aux->endAnt = NULL;
	elevador->chamados = aux;
	separarChamadas(elevador,andarDestino);
	return;
}

void separarChamadas(elevador *elevador, int andarDestino){
	//Adicionar mecânica da lista RESERVA e DESCENDO:
	//Subindo:
	if(andarDestino > elevador->chamados->n){
		//Entra na Fila Subindo Padrão
		if(elevador->chamados->n >= elevador->andarAtual->nAndar || elevador->direcao == -1){// Se o andar chamado estiver acima do andar atual OU o elevador estiver DESCENDO
			if(elevador->subindo == NULL){
				listaAndares *aux;
				aux = elevador->chamados;
				elevador->chamados = NULL;
				aux->endProx = NULL;
				elevador->subindo = aux;
			}else{
				listaAndares *aux;
				aux = elevador->subindo;
				while(elevador->chamados->n >= aux->n && aux->endProx != NULL){
					aux = aux->endProx;
					if(aux->endProx == NULL){
						break;
					}
				}
				//inserir no final:
				if(aux->endProx == NULL && elevador->chamados->n >= aux->n){
					aux->endProx = elevador->chamados;
					elevador->chamados = NULL;
					aux->endProx->endAnt = aux;
					aux->endProx->endProx = NULL;
				//inserir no início:
				}else if(aux == elevador->subindo){
					elevador->subindo = elevador->chamados;
					elevador->chamados = NULL;
					elevador->subindo->endProx = aux;
					aux->endAnt = elevador->subindo;
				}
				//inserir no meio:
				else{
					listaAndares *aux2 = aux->endAnt;
					aux2->endProx = elevador->chamados;
					aux->endAnt = elevador->chamados;
					elevador->chamados = NULL;
					aux2->endProx->endAnt = aux2;
					aux2->endProx->endProx = aux;
				}
			}
		}//Entra na Fila Reserva(Subida):
		else{
			if(elevador->reserva == NULL){
				listaAndares *aux;
				aux = elevador->chamados;
				elevador->chamados = NULL;
				aux->endProx = NULL;
				elevador->reserva = aux;
			}else{
				listaAndares *aux;
				aux = elevador->reserva;
				while(elevador->chamados->n >= aux->n && aux->endProx != NULL){
					aux = aux->endProx;
					if(aux->endProx == NULL){
						break;
					}
				}
				//inserir no Final:
				if(aux->endProx == NULL && elevador->chamados->n >= aux->n){
					aux->endProx = elevador->chamados;
					elevador->chamados = NULL;
					aux->endProx->endAnt = aux;
					aux->endProx->endProx = NULL;
				}
				//inserir no Inicio:
				else if(aux == elevador->reserva){
					elevador->reserva = elevador->chamados;
					elevador->chamados = NULL;
					elevador->reserva->endProx = aux;
					aux->endAnt = elevador->reserva;
				}
				//inserir no meio:
				else{
					listaAndares *aux2 = aux->endAnt;
					aux2->endProx = elevador->chamados;
					aux->endAnt = elevador->chamados;
					elevador->chamados = NULL;
					aux2->endProx->endAnt = aux2;
					aux2->endProx->endProx = aux;
				}
			}
		}
		
	}//Descendo:
	else{
		//Entrar na Fila Descendo Padrão:
		if(elevador->chamados->n <= elevador->andarAtual->nAndar || elevador->direcao == 1){//Se estiver abaixo do elevador ou quando elevador estiver DESCENDO
			if(elevador->descendo == NULL){
				listaAndares *aux;
				aux = elevador->chamados;
				elevador->chamados = NULL;
				aux->endProx = NULL;
				elevador->descendo = aux;
			}else{
				listaAndares *aux;
				aux = elevador->descendo;
				while(elevador->chamados->n <= aux->n && aux->endProx != NULL){
					aux = aux->endProx;
					if(aux->endProx == NULL){
						break;
					}
				}
				//inserir no início:
				if(aux == elevador->descendo && elevador->chamados->n >= aux->n){
					elevador->descendo = elevador->chamados;
					elevador->chamados = NULL;
					elevador->descendo->endProx = aux;
					aux->endAnt = elevador->descendo;
				}//inserir no final:
				else if(aux->endProx == NULL && elevador->chamados->n <= aux->n){
					aux->endProx = elevador->chamados;
					elevador->chamados = NULL;
					aux->endProx->endAnt = aux;
					aux->endProx->endProx = NULL;
				}//inserir no meio:
				else{
					listaAndares *aux2 = aux->endAnt;
					aux2->endProx = elevador->chamados;
					aux->endAnt = elevador->chamados;
					elevador->chamados = NULL;
					aux2->endProx->endAnt = aux2;
					aux2->endProx->endProx = aux;
				}
			}
		}//Entrar na Fila Reserva(Descida):
		else{
			if(elevador->reserva == NULL){
				listaAndares *aux;
				aux = elevador->chamados;
				elevador->chamados = NULL;
				aux->endProx = NULL;
				elevador->reserva = aux;
			}else{
				listaAndares *aux;
				aux = elevador->reserva;
				while(elevador->chamados->n <= aux->n && aux->endProx != NULL){
					aux = aux->endProx;
					if(aux->endProx == NULL){
						break;
					}
				}
				
				//inserir no final:
				if(aux->endProx == NULL && elevador->chamados->n <= aux->n){
					aux->endProx = elevador->chamados;
					elevador->chamados = NULL;
					aux->endProx->endAnt = aux;
					aux->endProx->endProx = NULL;
				}//inserir no início:
				else if(aux == elevador->reserva && elevador->chamados->n >= elevador->reserva->n){
					elevador->reserva = elevador->chamados;
					elevador->chamados = NULL;
					elevador->reserva->endProx = aux;
					aux->endAnt = elevador->reserva;
				}//inserir no meio:
				else{
					listaAndares *aux2 = aux->endAnt;
					aux2->endProx = elevador->chamados;
					aux->endAnt = elevador->chamados;
					elevador->chamados = NULL;
					aux2->endProx->endAnt = aux2;
					aux2->endProx->endProx = aux;
				}
			}
		}
	}
	return;	
}

void inicializarElevador(elevador *elevador, andar **predio, int direcao, listaAndares **apertados, int andarAtual){
	andar *aux = *predio;
	while(aux->nAndar != andarAtual){
		aux = aux->prox;
	}
	elevador->andarAtual = aux;
	elevador->direcao = direcao;
	elevador->subindo = NULL;
	elevador->descendo = NULL;
	elevador->destino = *apertados;	
	elevador->chamados = NULL;
	elevador->reserva = NULL;
	elevador->pessoasDentro = NULL;
	elevador->andaresPassados = NULL;
	elevador->andaresPercorridos = 0;
	elevador->ID = 1;
	return;
}

void inserirAndaresPassados(elevador *elev, int tempo){
	elevador *auxElevador = elev;
	rota *aux = elev->andaresPassados;
	if(aux == NULL){
		rota *aux2 = malloc(sizeof(rota));
		aux2->tempo = tempo;
		aux2->nAndar = auxElevador->andarAtual->nAndar;
		aux2->endprox = NULL;
		elev->andaresPassados = aux2;
	}else{
		rota *aux3 = aux;
		while(aux3->endprox != NULL){
			aux3 = aux3->endprox;
		}
		rota *aux2 = malloc(sizeof(rota));
		aux2->tempo = tempo;
		aux2->endprox = NULL;
		aux2->nAndar = auxElevador->andarAtual->nAndar;
		aux3->endprox = aux2;
	}
	return;
}

void inserirPessoasElevador(elevador *insElevador){
	//Subindo
	if(insElevador->direcao == 1){
		listaAndares *aux = insElevador->subindo;
		listaAndares *listaSubindo = insElevador->subindo;
		
		while(aux != NULL && aux->n == insElevador->andarAtual->nAndar){
			while(aux->n != insElevador->andarAtual->nAndar && aux != NULL){
				aux = aux->endProx;
			}if(aux != NULL){
				//Inserir o andar atual na rota dos andares passados do elevador

				//Retirar esse andar na lista de subida, adicionar essa pessoa na Lista das Pessoas Dentro, adicionar o Andar Desino na Lista Destino e retirar essa Pessoa do andar:
				//(1) Retirar esse andar:
				//a)Primeiro elemento
				if(aux->endAnt == NULL && aux->endProx != NULL){
					insElevador->subindo = aux->endProx;
					aux->endProx->endAnt = NULL;
					free(aux);
					listaSubindo = insElevador->subindo;
				}
				//b)Único elemento
				else if(aux->endAnt == NULL && aux == insElevador->subindo){
					insElevador->subindo = NULL;
					free(aux);
					listaSubindo = insElevador->subindo;
				}//c)Último elemento
				else if(aux->endProx == NULL && aux->endAnt != NULL){
					aux->endAnt = NULL;
					free(aux);
					listaSubindo = insElevador->subindo;
				}//d)Elemento no meio
				else{
					listaAndares *aux2 = aux->endProx;
					aux2->endAnt = aux->endAnt;
					aux->endAnt->endProx = aux2;
					free(aux);
					listaSubindo = insElevador->subindo;
				}
				//(2) Adicionar essa Pessoa na Lista das Pessoas Dentro
				//a) Olhar pras Pessoas do Andar e ver quem quer subir e depois adicionar na lista de Pessoas Dentro
				andar *andarAtual = insElevador->andarAtual;
				listaPessoas *pessoasInicial = insElevador->andarAtual->pessoas;//Salvar o endereço inicial da lista 
				while(andarAtual->pessoas != NULL && andarAtual->pessoas->pessoa.andarD < insElevador->andarAtual->nAndar){
					andarAtual->pessoas = andarAtual->pessoas->endProx;
				}
				if(andarAtual != NULL){//Achou alguém
					//b) Vamos retirar essa Pessoa da lista do andar e Adicionar ela na Lista de Dentro
					//Verificar se a lista de Pessoas Dentro está vazia:
					
					//Único elemento
					if(andarAtual->pessoas->endAnt == NULL && andarAtual->pessoas->endProx == NULL && andarAtual->pessoas == insElevador->andarAtual->pessoas){
						if(insElevador->pessoasDentro == NULL){
							insElevador->pessoasDentro = andarAtual->pessoas;
							insElevador->andarAtual->pessoas = NULL;
						}else{
							insElevador->pessoasDentro->endAnt = andarAtual->pessoas;
							andarAtual->pessoas->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro = andarAtual->pessoas;
							insElevador->andarAtual->pessoas = NULL;
							insElevador->pessoasDentro->endAnt = NULL;
						}
					}//Primeiro elemento
					else if(andarAtual->pessoas->endAnt == NULL){
						if(insElevador->pessoasDentro == NULL){
							listaPessoas *pessoa = andarAtual->pessoas;//Copiado para destino
							andarAtual->pessoas->endProx->endAnt = NULL;
							andarAtual->pessoas = andarAtual->pessoas->endProx;
							insElevador->pessoasDentro = pessoa;
							pessoa->endProx = NULL;
							pessoa->endProx = NULL;
						}else{
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endProx->endAnt = NULL;
							andarAtual->pessoas = andarAtual->pessoas->endProx;
							pessoa->endProx = insElevador->pessoasDentro;
							pessoa->endAnt = NULL;
							insElevador->pessoasDentro->endAnt = pessoa;
							insElevador->pessoasDentro = pessoa;
						}
						
					}//Último elemento
					else if(andarAtual->pessoas->endProx == NULL){
						if(insElevador->pessoasDentro == NULL){
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							andarAtual->pessoas->endProx = NULL;
							pessoa->endAnt = NULL;
							pessoa->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro->endAnt = pessoa;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}else{
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							andarAtual->pessoas->endProx = NULL;
							pessoa->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro->endAnt = pessoa;
							pessoa->endAnt = NULL;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}
					}//No meio
					else{
						if(insElevador->pessoasDentro == NULL){
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = andarAtual->pessoas->endProx;
							andarAtual->pessoas->endProx->endAnt = andarAtual->pessoas->endAnt;
							andarAtual->pessoas->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}
						else{
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = andarAtual->pessoas->endProx;
							andarAtual->pessoas->endProx->endAnt = andarAtual->pessoas->endAnt;
							andarAtual->pessoas->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							pessoa->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro->endAnt = pessoa;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}
						
					}
				}
			}
			aux = listaSubindo;
			ordemPessoasElevador(insElevador);
			organizarDestino(insElevador);
		}
	}//Descendo
	else if(insElevador->direcao == -1){
		listaAndares *aux = insElevador->descendo;
		listaAndares *listaDescendo = insElevador->descendo;
		
		
		while(aux != NULL && aux->n == insElevador->andarAtual->nAndar){
			while(aux->n != insElevador->andarAtual->nAndar && aux != NULL){
				aux = aux->endProx;
			}if(aux != NULL){
				//Inserir na rota de Andares Passados do elevador
				
				
				//Retirar esse andar na lista de Descida, adicionar essa pessoa na Lista das Pessoas Dentro, adicionar o Andar Desino na Lista Destino e retirar essa Pessoa do andar:
				//(1) Retirar esse andar:
				//a)Primeiro elemento
				if(aux->endAnt == NULL && aux->endProx != NULL){
					insElevador->descendo = aux->endProx;
					aux->endProx->endAnt = NULL;
					free(aux);
					listaDescendo = insElevador->descendo;
				}
				//b)Único elemento
				else if(aux->endAnt == NULL && aux == insElevador->descendo){
					insElevador->descendo = NULL;
					free(aux);
					listaDescendo = insElevador->descendo;
				}//c)Último elemento
				else if(aux->endProx == NULL && aux->endAnt != NULL){
					aux->endAnt = NULL;
					free(aux);
					listaDescendo = insElevador->descendo;
				}//d)Elemento no meio
				else{
					listaAndares *aux2 = aux->endProx;
					aux2->endAnt = aux->endAnt;
					aux->endAnt->endProx = aux2;
					free(aux);
					listaDescendo = insElevador->descendo;
				}
				//(2) Adicionar essa Pessoa na Lista das Pessoas Dentro
				//a) Olhar pras Pessoas do Andar e ver quem quer descer e depois adicionar na lista de Pessoas Dentro
				andar *andarAtual = insElevador->andarAtual;
				listaPessoas *pessoasInicial = insElevador->andarAtual->pessoas;//Salvar o endereço inicial da lista 
				while(andarAtual->pessoas != NULL && andarAtual->pessoas->pessoa.andarD > insElevador->andarAtual->nAndar){
					andarAtual->pessoas = andarAtual->pessoas->endProx;
				}
				if(andarAtual != NULL){//Achou alguém
					//b) Vamos retirar essa Pessoa da lista do andar e Adicionar ela na Lista de Dentro
					//Verificar se a lista de Pessoas Dentro está vazia:
					
					//Único elemento
					if(andarAtual->pessoas->endAnt == NULL && andarAtual->pessoas->endProx == NULL && andarAtual->pessoas == insElevador->andarAtual->pessoas){
						if(insElevador->pessoasDentro == NULL){
							insElevador->pessoasDentro = andarAtual->pessoas;
							insElevador->andarAtual->pessoas = NULL;
						}else{
							insElevador->pessoasDentro->endAnt = andarAtual->pessoas;
							andarAtual->pessoas->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro = andarAtual->pessoas;
							insElevador->andarAtual->pessoas = NULL;
							insElevador->pessoasDentro->endAnt = NULL;
						}
					}//Primeiro elemento
					else if(andarAtual->pessoas->endAnt == NULL){
						if(insElevador->pessoasDentro == NULL){
							listaPessoas *pessoa = andarAtual->pessoas;//Copiado para destino
							andarAtual->pessoas->endProx->endAnt = NULL;
							andarAtual->pessoas = andarAtual->pessoas->endProx;
							insElevador->pessoasDentro = pessoa;
							pessoa->endProx = NULL;
						}else{
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endProx->endAnt = NULL;
							andarAtual->pessoas = andarAtual->pessoas->endProx;
							pessoa->endProx = insElevador->pessoasDentro;
							pessoa->endAnt = NULL;
							insElevador->pessoasDentro->endAnt = pessoa;
							insElevador->pessoasDentro = pessoa;
						}
						
					}//Último elemento
					else if(andarAtual->pessoas->endProx == NULL){
						if(insElevador->pessoasDentro == NULL){
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							andarAtual->pessoas->endProx = NULL;
							pessoa->endAnt = NULL;
							pessoa->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro->endAnt = pessoa;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}else{
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							andarAtual->pessoas->endProx = NULL;
							pessoa->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro->endAnt = pessoa;
							pessoa->endAnt = NULL;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}
					}//No meio
					else{
						if(insElevador->pessoasDentro == NULL){
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = andarAtual->pessoas->endProx;
							andarAtual->pessoas->endProx->endAnt = andarAtual->pessoas->endAnt;
							andarAtual->pessoas->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}
						else{
							listaPessoas *pessoa = andarAtual->pessoas;
							andarAtual->pessoas->endAnt->endProx = andarAtual->pessoas->endProx;
							andarAtual->pessoas->endProx->endAnt = andarAtual->pessoas->endAnt;
							andarAtual->pessoas->endProx = NULL;
							andarAtual->pessoas->endAnt = NULL;
							pessoa->endProx = insElevador->pessoasDentro;
							insElevador->pessoasDentro->endAnt = pessoa;
							insElevador->pessoasDentro = pessoa;
							insElevador->andarAtual->pessoas = pessoasInicial;
						}
						
					}
				}
			}
			aux = listaDescendo;
			organizarDestino(insElevador);
			ordemPessoasElevador(insElevador);
		}
	}
	return;
}

//Criar função para ordenar as pessoas dentro do elevador enquanto é colocado
void ordemPessoasElevador(elevador *elev){
	elevador *aux = elev;
	if(aux->pessoasDentro != NULL && aux->pessoasDentro->endProx != NULL){
		if(aux->direcao == 1){//Subindo
			listaPessoas *aux1 = aux->pessoasDentro;
			listaPessoas *aux2 = aux1->endProx;
			while(aux2 != NULL){
				if(aux1->pessoa.andarD > aux2->pessoa.andarD){
					//Se for Início + Próximo e só tiver 2
					if(aux1->endProx == aux2 && aux2->endProx == NULL){
						aux2->endProx = aux1;
						aux1->endAnt = aux2;
						aux1->endProx = NULL;
						aux2->endAnt = NULL;
						aux->pessoasDentro = aux2;
						break;
					}//Se for Início + Próximo e tiver mais de 2
					else if(aux1->endProx == aux2 && aux2->endProx != NULL && aux1->pessoa.andarD < aux2->endProx->pessoa.andarD){
						aux1->endProx = aux2->endProx;
						aux2->endProx->endAnt = aux1;
						aux2->endProx = aux1;
						aux2->endAnt = NULL;
						aux1->endAnt = aux2;
						aux->pessoasDentro = aux2;
					}
					// Se for Início + Meio
					else if(aux2->endProx != NULL && aux1->pessoa.andarD < aux2->endProx->pessoa.andarD){
						aux1->endProx->endAnt = NULL;
						aux->pessoasDentro = aux1->endProx;
						aux1->endProx = aux2->endProx;
						aux1->endAnt = aux2;
						aux2->endProx->endAnt = aux1;
						aux2->endProx = aux1;
						break;
					}// Se for Início + Final
					else if(aux1->endAnt == NULL && aux2->endProx == NULL){
						aux1->endProx->endAnt = NULL;
						aux->pessoasDentro = aux1->endProx;
						aux1->endProx = NULL;
						aux1->endAnt = aux2;
						aux2->endProx = aux1;
						break;
					}
				}
				aux2 = aux2->endProx;
			}
			return;
		}//Descendo
		else{
			listaPessoas *aux1 = aux->pessoasDentro;
			listaPessoas *aux2 = aux1->endProx;
			while(aux2 != NULL){
				if(aux1->pessoa.andarD < aux2->pessoa.andarD){
					//Se for Início + Próximo e só tiver 2
					if(aux1->endProx == aux2 && aux2->endProx == NULL){
						aux2->endProx = aux1;
						aux1->endAnt = aux2;
						aux1->endProx = NULL;
						aux2->endAnt = NULL;
						aux->pessoasDentro = aux2;
						break;
					}//Se for Início + Próximo e tiver mais de 2
					else if(aux1->endProx == aux2 && aux2->endProx != NULL && aux1->pessoa.andarD > aux2->endProx->pessoa.andarD){
						aux1->endProx = aux2->endProx;
						aux2->endProx->endAnt = aux1;
						aux2->endProx = aux1;
						aux2->endAnt = NULL;
						aux1->endAnt = aux2;
						aux->pessoasDentro = aux2;
						break;
					}
					// Se for Início + Meio
					else if(aux2->endProx != NULL && aux1->pessoa.andarD > aux2->endProx->pessoa.andarD){
						aux1->endProx->endAnt = NULL;
						aux->pessoasDentro = aux1->endProx;
						aux1->endProx = aux2->endProx;
						aux1->endAnt = aux2;
						aux2->endProx->endAnt = aux1;
						aux2->endProx = aux1;
						break;
					}// Se for Início + Final
					else if(aux1->endAnt == NULL && aux2->endProx == NULL){
						aux1->endProx->endAnt = NULL;
						aux->pessoasDentro = aux1->endProx;
						aux1->endProx = NULL;
						aux1->endAnt = aux2;
						aux2->endProx = aux1;
						break;
					}
				}
				aux2 = aux2->endProx;
			}
			return;
		}
		return;
	}
}

//Inserir Ordenado
void organizarDestino (elevador *elev){
	elevador *aux = elev;
	//Subindo
	if(aux->direcao == 1){
		if(aux->destino == NULL){
			listaAndares *aux2 = malloc(sizeof(listaAndares));
			aux2->endProx = NULL;
			aux2->endAnt = NULL;
			aux2->n = aux->pessoasDentro->pessoa.andarD;
			aux->destino = aux2;
		}else{
			listaAndares *aux2 = malloc(sizeof(listaAndares));
			aux2->n = aux->pessoasDentro->pessoa.andarD;
			
			listaAndares *aux3 = aux->destino;
			while(aux3 != NULL){
				if(aux2->n >= aux3->n){
					//Colocar no Final
					if(aux3->endProx == NULL){
						aux2->endAnt = aux3;
						aux2->endProx = NULL;
						aux3->endProx = aux2;
						break;
					}//Colocar no meio
					else if(aux2->n < aux3->endProx->n){
						aux2->endProx = aux3->endProx;
						aux2->endAnt = aux3;
						aux3->endProx = aux2;
						aux2->endProx->endAnt = aux2;
						break;
					}
					}else{
						aux3->endAnt = aux2;
						aux2->endProx = aux3;
						aux2->endAnt = NULL;
						aux->destino = aux2;
						break;
					}
				aux3 = aux3->endProx;
			}	
		}
	}//Descendo
	else{
		if(aux->destino == NULL){
			listaAndares *aux2 = malloc(sizeof(listaAndares));
			aux2->endProx = NULL;
			aux2->endAnt = NULL;
			aux2->n = aux->pessoasDentro->pessoa.andarD;
			aux->destino = aux2;
		}else{
			listaAndares *aux2 = malloc(sizeof(listaAndares));
			aux2->n = aux->pessoasDentro->pessoa.andarD;
			
			listaAndares *aux3 = aux->destino;
			while(aux3 != NULL){
				if(aux2->n <= aux3->n){
					//Colocar no Final
					if(aux3->endProx == NULL){
						aux2->endAnt = aux3;
						aux2->endProx = NULL;
						aux3->endProx = aux2;
						break;
					}//Colocar no meio
					else if(aux2->n > aux3->endProx->n){
						aux2->endProx = aux3->endProx;
						aux2->endAnt = aux3;
						aux3->endProx = aux2;
						aux2->endProx->endAnt = aux2;
						break;
					}
					}else{
						aux3->endAnt = aux2;
						aux2->endProx = aux3;
						aux2->endAnt = NULL;
						aux->destino = aux2;
						break;
					}
				aux3 = aux3->endProx;
			}	
		}
	}
	return;
}

void moverElevador(elevador *elev){
	// Subir
	if(elev->direcao == 1 && elev->andarAtual->prox != NULL){
		elev->andarAtual = elev->andarAtual->prox;
		elev->andaresPercorridos++;
	}// Descer
	else if(elev->direcao == -1 && elev->andarAtual->ant != NULL){
		elev->andarAtual = elev->andarAtual->ant;
		elev->andaresPercorridos++;
	}
	return;
}

void tirarPessoa(elevador *elev, int tempo){
	elevador *aux = elev;
	if(aux->destino != NULL && aux->andarAtual->nAndar == aux->destino->n){

		listaAndares *auxAndar = aux->destino;
		listaPessoas *auxP = aux->pessoasDentro;
		while(aux->destino != NULL && aux->andarAtual->nAndar == aux->destino->n){
			inserirAndaresPassados(elev, tempo);
			auxAndar = aux->destino;
			auxP = aux->pessoasDentro;
			aux->destino = auxAndar->endProx;
			if(auxAndar->endProx != NULL){
				auxAndar->endProx->endAnt = NULL;	
			}
			free(auxAndar);
			if(auxP != NULL && auxP->pessoa.andarD == aux->andarAtual->nAndar){
				aux->pessoasDentro = auxP->endProx;
				if(auxP->endProx != NULL){
					auxP->endProx->endAnt = NULL;
				}
				free(auxP);
			}
		}
	}
	return;
}

void definirSentido(elevador *elev){
	elevador *aux = elev;
	if(aux->destino == NULL){
		//Se o elevador estiver subindo, mas tiver que mudar de sentido
		if((aux->direcao == 1 && aux->subindo == NULL && aux->descendo != NULL && aux->andarAtual->nAndar == aux->descendo->n) || (aux->direcao == 1 && aux->subindo != NULL && aux->descendo != NULL && aux->descendo->n > aux->subindo->n && aux->andarAtual->nAndar == aux->descendo->n)){
			aux->direcao = -1;
			if(aux->reserva != NULL){
				aux->subindo = aux->reserva;
				aux->reserva = NULL;
			}
		}
		//Se o elevador estiver descendo, mas tiver que mudar de sentido
		else if((aux->direcao == -1 && aux->descendo == NULL && aux->subindo != NULL && aux->andarAtual->nAndar == aux->subindo->n) || (aux->direcao == -1 && aux->descendo != NULL && aux->subindo != NULL && aux->descendo->n > aux->subindo->n && aux->andarAtual->nAndar == aux->subindo->n)){
			aux->direcao = 1;
			if(aux->reserva != NULL){
				aux->descendo = aux->reserva;
				aux->reserva = NULL;
			}
		}
		
		//Se já estivesse subindo
		if(aux->direcao == 1 && aux->subindo == NULL){
			if(aux->descendo != NULL){
				if(aux->descendo->n <= aux->andarAtual->nAndar || aux->andarAtual->nAndar == aux->descendo->n){
					aux->direcao = -1;
				}else{
					aux->direcao = 1;
				}
				aux->subindo = aux->reserva;
				aux->reserva = NULL;
				return;
			}else if(aux->reserva != NULL){
				aux->direcao = -1;
				aux->subindo = aux->reserva;
				aux->reserva = NULL;
				return;
			}
			aux->direcao = 0;
		}//Se estivesse descendo
		else if(aux->direcao == -1 && aux->descendo == NULL){
			if(aux->subindo != NULL){
				if(aux->subindo->n >= aux->andarAtual->nAndar){
					aux->direcao = 1;
				}else{
					aux->direcao = -1;
				}
				aux->descendo = aux->reserva;
				aux->reserva = NULL;
				return;
			}else if(aux->reserva != NULL){
				aux->direcao = -1;
				aux->descendo = aux->reserva;
				aux->reserva = NULL;
				return;
			}
			aux->direcao = 0;
		}
	}
	return;
}

void atualizar(elevador *elev, int tempo){
	tirarPessoa(elev,tempo);
	definirSentido(elev);
	inserirPessoasElevador(elev);
	imprimirElevador(elev);
	moverElevador(elev);
	return;
}

void imprimirElevador(elevador *elev){
	elevador *aux = elev;
	if(aux->andaresPassados != NULL){
		printf("\nElevador %d :\n %d(%d)",aux->ID,aux->andaresPassados->nAndar,aux->andaresPassados->tempo);
		rota *aux2 = aux->andaresPassados->endprox;
		while(aux2 != NULL){
			printf(" -> %d(%d)",aux2->nAndar,aux2->tempo);
			aux2 = aux2->endprox;
		}
		printf("\nO elevador percorreu : %d andares.\n",aux->andaresPercorridos);
	}
	return;
}
/*COMO SERIA O LOOP DO PROGRAMA:
int tempo = 0;
	int dtempo = 2;
	int tAtual;
	while(tempo < 10){
		tAtual = clock()/CLOCKS_PER_SEC;
		if(tAtual - tempo >= dtempo){
			printf("k\n");
			tempo = tempo + 2;
		}
	}
*/