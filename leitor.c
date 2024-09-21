#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Programa que realiza a leitura da string de eventos

/* AM_25
 *
 * E1_04_S_6,9,8 E2_11_D_5,8,9,3,2,T E3_20_D_5,8,T,9
 *
 * T5_P01_S_4_6 T8_P01_D_6_4 T9_P02_S_5_10 T10_P03_S_3_9 T10_P04_D_6_T
 * T10_P05_S_8_15 T15_P06_D_9_2 T15_P07_S_2_13 T18_P08_D_8_T T21_P01_D_16_3
 * T21_P10_S_T_13 T21_P11_S_T_12 T23_P12_S_T_15 T28_P13_S_2_13
 */

int tempoGeral = 0;


typedef struct pessoa1{
    int id;
    int andarD;
    int direcao;
    struct pessoa1 *prox;
    struct pessoa1 *ant;
}pessoa;

typedef struct andar1{
    int andar;
    struct andar1 *prox;
    struct andar1 *ant;
    pessoa *pessoaSubindo;
    pessoa *pessoaDescendo;
}andar;


typedef struct elevador1{
    int direcao;// -1 - Descendo, 1 - Subindo
    int id;
    pessoa *pessoasDentro;
    andar *andarAtual;
    andar *andaresDestino;
    andar *andaresChamado;
    struct elevador1 *prox;
}elevador;

typedef struct eventos1 {
    int tempo;
    int andar;
    pessoa *pessoa;
    struct eventos1 *prox;
} gerenciador;


void leitor(char *, andar **, elevador **, gerenciador **ge);
void createEventos(gerenciador **gerente, int tempo, pessoa *pessoaG, int andarG);
void gerenciarEventos(andar *andarGerenciar, gerenciador **gerencia);
pessoa *createPessoa(int id, char *a, int direcao);
void createAndar(andar **andares, int numAndar, int condicao);
void createElevador(elevador **elevadores, int id, int direcao, andar *andaresElevador);
int verificarDirecao(char *);
void moveElevadores(elevador *elevadorMove);
void inserirElevadorAndar(elevador *elevadorP, andar *andares, int andar, int id);
void inserirPessoasAndar(andar *andarInserir, pessoa *pessoaInserir);
void removerPessoasElevador(elevador *elevadorGerenciar);
void inserirPessoasElevador(elevador *elevadorAndar);
void printElevador(elevador *elevadorP);

void atualizarTempo();

void leitor(char *_string, andar **andares, elevador **elevadores, gerenciador **ge) {
    char *auxExterno;
    char *auxInterno;
    char *saveExterno = NULL;
    char *saveInterno = NULL;
    int A, T, V, elevadorID, elevadorAndar;

    auxExterno = strtok_r(_string, " ", &saveExterno);

    while (auxExterno != NULL) {
        auxInterno = strtok_r(auxExterno, "_", &saveInterno);

        if (!strcmp(auxInterno, "AM")) {
            auxInterno = strtok_r(NULL, " ", &saveInterno);
            A = atoi(auxInterno);

            if (A > 25 || A < 5) {
                printf("Quantidade de andares deve ser no minimo 5 e no maximo 25!");
                return;
            }

            for (int i = 1; i <= A; i++) {
                createAndar(andares, i, 1);
            }
        } else if (!strncmp(auxInterno, "E", 1)) {
            elevadorID = atoi(auxInterno + 1);
            printf("Elevador: %d\n", elevadorID); // Printa qual elevador

            auxInterno = strtok_r(NULL, "_", &saveInterno);
            elevadorAndar = atoi(auxInterno);
            printf("Esta no andar: %d\n", elevadorAndar); // Printa andar onde o elevador esta

            auxInterno = strtok_r(NULL, "_", &saveInterno);

            V = verificarDirecao(auxInterno);
            if (V == 1) {
                printf("Subindo\n");
            } else if (V == -1) {
                printf("Descendo\n");
            } else {
                printf("Erro: Direcao do elevador invalida\n\n");
                return;
            }


            andar *andarElevador = NULL;
            auxInterno = strtok_r(NULL, ",", &saveInterno);

            while (auxInterno) {
                int numAndar;

                if (!strcmp(auxInterno, "T")) {
                    numAndar = 1;
                } else {
                    numAndar = atoi(auxInterno);
                }

                createAndar(&andarElevador, numAndar, V);
                auxInterno = strtok_r(NULL, ",", &saveInterno);
            }


            createElevador(elevadores, elevadorID, V, andarElevador);
            inserirElevadorAndar(*elevadores, *andares, elevadorAndar, elevadorID);
            printf("\n\n");

        } else if (!strncmp(auxInterno, "T", 1) && *(auxInterno + 1) + 1) {
            T = atoi(auxInterno + 1);
            printf("No tempo %d:\n", T);

            while (auxInterno) {
                auxInterno = strtok_r(NULL, "_", &saveInterno);
                if (!strncmp(auxInterno, "P", 1)) {
                    A = atoi(auxInterno + 1);
                    printf("Pessoa %d:\n", A);
                    auxInterno = strtok_r(NULL, "_", &saveInterno);
                }

                V = verificarDirecao(auxInterno);
                if (V == 1) {
                    printf("Subindo\n");
                } else if (V == -1) {
                    printf("Descendo\n");
                } else {
                    printf("Erro: Direcao invalida\n\n");
                    return;
                }
                auxInterno = strtok_r(NULL, "_", &saveInterno);

                int numAndar;

                if (!strcmp(auxInterno, "T")) {
                    numAndar = 1;
                } else {
                    numAndar = atoi(auxInterno);
                }

                createEventos(ge, T, createPessoa(A, auxInterno, V), numAndar);
                printf("\n\n");
                auxInterno = NULL;
            }
        }

        auxExterno = strtok_r(NULL, " ", &saveExterno);
    }
}

void createEventos(gerenciador **gerente, int tempo, pessoa *pessoaG, int andarG) {
    gerenciador *novo = malloc(sizeof(gerenciador));
    novo->tempo = tempo;
    novo->pessoa = pessoaG;
    novo->andar = andarG;
    novo->prox = NULL;

    if (!*gerente) {
        *gerente = novo;
    } else {
        gerenciador *aux = *gerente;
        while (aux->prox) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

pessoa *createPessoa(int id, char *a, int direcao) {
    pessoa *novo = malloc(sizeof(pessoa));
    novo->ant = NULL;
    novo->prox = NULL;
    novo->direcao = direcao;

    int numAndar;

    if (!strcmp(a + 2, "T")) {
        numAndar = 1;
    } else {
        numAndar = atoi(a + 2);
    }

    novo->andarD = numAndar;
    novo->id= id;

    return novo;
}

void createElevador(elevador **elevadores, int id, int direcao, andar *andaresElevador) {
    elevador *novo = malloc(sizeof(elevador));
    novo->id = id;
    novo->direcao = direcao;
    novo->andaresDestino = andaresElevador;
    novo->andaresChamado = NULL;
    novo->pessoasDentro = NULL;
    novo->andarAtual = NULL;
    novo->prox = NULL;


    if (!*elevadores) {
        *elevadores = novo;
        return;
    } else {
        elevador *aux = *elevadores;
        while (aux->prox) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}



void createAndar(andar **andares, int numAndar, int condicao) {
    andar *novo = malloc(sizeof(andar));
    novo->prox = NULL;
    novo->ant = NULL;
    novo->pessoaSubindo = NULL;
    novo->pessoaDescendo = NULL;
    novo->andar = numAndar;

    if (!(*andares)) {
        *andares = novo;
        return;
    } else if (condicao == 1) {
        andar *aux2 = *andares;
        if (aux2->andar > novo->andar) {
            novo->prox = aux2;
            aux2->ant = novo;
            *andares = novo;
        } else {
            while (aux2->prox && novo->andar > (aux2->prox)->andar) {
                aux2 = aux2->prox;
            }
            if (!aux2->prox) {
                novo->ant = aux2;
                aux2->prox = novo;
            } else {
                novo->prox = aux2->prox;
                novo->ant = aux2;
                (aux2->prox)->ant = novo;
                aux2->prox = novo;
            }
        }
    } else {
        andar *aux2 = *andares;
        if (aux2->andar < novo->andar) {
            novo->prox = aux2;
            aux2->ant = novo;
            *andares = novo;
        } else {
            while (aux2->prox && novo->andar < (aux2->prox)->andar) {
                aux2 = aux2->prox;
            }
            if (!aux2->prox) {
                novo->ant = aux2;
                aux2->prox = novo;
            } else {
                novo->prox = aux2->prox;
                novo->ant = aux2;
                (aux2->prox)->ant = novo;
                aux2->prox = novo;
            }
        }
    }
}


int verificarDirecao(char *verif) {
    if (!strcmp(verif, "S")) {
        return 1;
    } else if (!strcmp(verif, "D")) {
        return -1;
    } else {
        return -2;
    }
}

void inserirElevadorAndar(elevador *elevadorP, andar *andares, int andar, int id) {
    while (elevadorP->id != id) {
        elevadorP = elevadorP->prox;
    }

    while (andares->andar != andar) {
        andares = andares->prox;
    }

    elevadorP->andarAtual = andares;
}


// Conrado da muito e o rabo
// Move o elevadaor, inverte o sentido se necessario e troca as listas

void moveElevadores(elevador *elevadorMove) {
    while (elevadorMove) {
        if (elevadorMove->andaresChamado || elevadorMove->andaresDestino) {
            if (elevadorMove->direcao == 1 && elevadorMove->andarAtual->prox) {
                elevadorMove->andarAtual = elevadorMove->andarAtual->prox;
            }
            if (elevadorMove->direcao == -1 && elevadorMove->andarAtual->ant) {
                elevadorMove->andarAtual = elevadorMove->andarAtual->ant;
            }
        }
        if (elevadorMove->andaresChamado && !elevadorMove->andaresDestino) {
            if (elevadorMove->direcao == 1) {
                elevadorMove->direcao = -1;
                elevadorMove->andaresDestino = elevadorMove->andaresChamado;
                elevadorMove->andaresChamado = NULL;
            } else {
                elevadorMove->direcao = 1;
                elevadorMove->andaresDestino = elevadorMove->andaresChamado;
                elevadorMove->andaresChamado = NULL;
            }
        }
        elevadorMove = elevadorMove->prox;
    }
}

void gerenciarEventos(andar *andarGerenciar, gerenciador **gerencia) {
    gerenciador *aux = *gerencia;
    while (aux && tempoGeral == aux->tempo) {
        while (andarGerenciar->andar != aux->andar) {
            if (aux->andar > andarGerenciar->andar) {
                andarGerenciar = andarGerenciar->prox;
            } else {
                andarGerenciar = andarGerenciar->ant;
            }
        }
        inserirPessoasAndar(andarGerenciar, aux->pessoa);
        *gerencia = aux->prox;
        free(aux);
        aux = *gerencia;
    }
}

void inserirPessoasAndar(andar *andarInserir, pessoa *pessoaInserir) { // Já inseri ordenado as pessoas nos andares
    if (!andarInserir->pessoaSubindo && pessoaInserir->direcao == 1) {
        andarInserir->pessoaSubindo = pessoaInserir;
        pessoaInserir->prox = NULL;
        pessoaInserir->ant = NULL;
        return;
    } else if (!andarInserir->pessoaDescendo && pessoaInserir->direcao == -1) {
        andarInserir->pessoaDescendo = pessoaInserir;
        pessoaInserir->prox = NULL;
        pessoaInserir->ant = NULL;
        return;
    } else if (andarInserir->pessoaSubindo && pessoaInserir->direcao == 1) {
        pessoa *aux = andarInserir->pessoaSubindo;
        if (pessoaInserir->andarD < aux->andarD) {
            pessoaInserir->prox = aux;
            pessoaInserir->ant = NULL;
            aux->ant = pessoaInserir;
        }
        while (aux->prox && aux->prox->andarD < pessoaInserir->andarD) {
            aux = aux->prox;
        }
        if (aux->prox) {
            pessoaInserir->prox = aux->prox;
            pessoaInserir->ant = aux;
            aux->prox->ant = pessoaInserir;
            aux->prox = pessoaInserir;
        } else {
            pessoaInserir->prox = NULL;
            pessoaInserir->ant = aux;
            aux->prox = pessoaInserir;
        }
    } else if (andarInserir->pessoaDescendo && pessoaInserir->andarD == -1) {
        pessoa *aux = andarInserir->pessoaDescendo;
        if (pessoaInserir->andarD > aux->andarD) {
            pessoaInserir->prox = aux;
            pessoaInserir->ant = NULL;
            aux->ant = pessoaInserir;
        }
        while (aux->prox && aux->prox->andarD > pessoaInserir->andarD) {
            aux = aux->prox;
        }
        if (aux->prox) {
            pessoaInserir->prox = aux->prox;
            pessoaInserir->ant = aux;
            aux->prox->ant = pessoaInserir;
            aux->prox = pessoaInserir;
        } else {
            pessoaInserir->prox = NULL;
            pessoaInserir->ant = aux;
            aux->prox = pessoaInserir;
        }
    }
}


// Fé em Deus q a casa vai
void inserirPessoasElevador(elevador *elevadorAndar) {

    if (elevadorAndar->direcao == 1) { // Crescente
        while (elevadorAndar->andarAtual->pessoaSubindo) {
            andar *andarAtualElevador = elevadorAndar->andarAtual;
            pessoa *pessoasElevador = elevadorAndar->pessoasDentro;

            if (!pessoasElevador) {
                elevadorAndar->pessoasDentro = andarAtualElevador->pessoaSubindo;
                andarAtualElevador->pessoaSubindo = andarAtualElevador->pessoaSubindo->prox;
                elevadorAndar->pessoasDentro->prox = NULL;
            } else {
                if (pessoasElevador->andarD > andarAtualElevador->pessoaSubindo->andarD) {
                    andarAtualElevador->pessoaSubindo->prox = pessoasElevador;
                    andarAtualElevador->pessoaSubindo->ant = NULL;
                    pessoasElevador->ant = andarAtualElevador->pessoaSubindo;
                    andarAtualElevador->pessoaSubindo = andarAtualElevador->pessoaSubindo->prox;
                } else {
                    while (pessoasElevador->prox && andarAtualElevador->pessoaSubindo->andarD > pessoasElevador->prox->andarD) {
                        pessoasElevador = pessoasElevador->prox;
                    }
                    if (pessoasElevador->prox) {
                        andarAtualElevador->pessoaSubindo->ant = pessoasElevador;
                        pessoasElevador->prox->ant = andarAtualElevador->pessoaSubindo;
                        andarAtualElevador->pessoaSubindo = andarAtualElevador->pessoaSubindo->prox;
                        pessoasElevador->prox->ant->prox = pessoasElevador->prox;
                        pessoasElevador->prox = pessoasElevador->prox->ant;
                    } else {
                        andarAtualElevador->pessoaSubindo->ant = pessoasElevador;
                        pessoasElevador->prox = andarAtualElevador->pessoaSubindo;
                        andarAtualElevador->pessoaSubindo = andarAtualElevador->pessoaSubindo->prox;
                        pessoasElevador->prox->prox = NULL;
                    }
                }
            }
        }

    } else { // Decrescente
        while (elevadorAndar->andarAtual->pessoaDescendo) {
            andar *andarAtualElevador = elevadorAndar->andarAtual;
            pessoa *pessoasElevador = elevadorAndar->pessoasDentro;
            if (!pessoasElevador) {
                elevadorAndar->pessoasDentro = andarAtualElevador->pessoaDescendo;
                andarAtualElevador->pessoaDescendo = andarAtualElevador->pessoaDescendo->prox;
                elevadorAndar->pessoasDentro->prox = NULL;
            } else {
                if (pessoasElevador->andarD < andarAtualElevador->pessoaDescendo->andarD) {
                    andarAtualElevador->pessoaDescendo->prox = pessoasElevador;
                    andarAtualElevador->pessoaDescendo->ant = NULL;
                    pessoasElevador->ant = andarAtualElevador->pessoaDescendo;
                    andarAtualElevador->pessoaDescendo = andarAtualElevador->pessoaDescendo->prox;
                } else {
                    while (pessoasElevador->prox && andarAtualElevador->pessoaDescendo->andarD < pessoasElevador->prox->andarD) {
                        pessoasElevador = pessoasElevador->prox;
                    }
                    if (pessoasElevador->prox) {
                        andarAtualElevador->pessoaDescendo->ant = pessoasElevador;
                        pessoasElevador->prox->ant = andarAtualElevador->pessoaDescendo;
                        andarAtualElevador->pessoaDescendo = andarAtualElevador->pessoaDescendo->prox;
                        pessoasElevador->prox->ant->prox = pessoasElevador->prox;
                        pessoasElevador->prox = pessoasElevador->prox->ant;
                    } else {
                        andarAtualElevador->pessoaDescendo->ant = pessoasElevador;
                        pessoasElevador->prox = andarAtualElevador->pessoaDescendo;
                        andarAtualElevador->pessoaDescendo = andarAtualElevador->pessoaDescendo->prox;
                        pessoasElevador->prox->prox = NULL;
                    }
                }
            }
        }
    }
}



void removerPessoasElevador(elevador *elevadorGerenciar) {

    while (elevadorGerenciar) {
        andar *auxAndar = elevadorGerenciar->andaresDestino;
        pessoa *aux = elevadorGerenciar->pessoasDentro;
        if (elevadorGerenciar->andaresDestino && elevadorGerenciar->andaresDestino->andar == elevadorGerenciar->andarAtual->andar) {
            while (aux) { // Remocao de destinos
                if (aux->andarD == elevadorGerenciar->andarAtual->andar) {
                    elevadorGerenciar->pessoasDentro = elevadorGerenciar->pessoasDentro->prox;
                    free(aux);
                    aux = elevadorGerenciar->pessoasDentro;
                } else {
                    aux = aux->prox;
                }
            }
            elevadorGerenciar->andaresDestino = elevadorGerenciar->andaresDestino->prox;
            free(auxAndar);
        }
        elevadorGerenciar = elevadorGerenciar->prox;
    }
}

void verificarChamadas(andar *andaresChamados, elevador *elevadoresChamados) {
    while (andaresChamados) {
        if (andaresChamados->pessoaSubindo || andaresChamados->pessoaDescendo) {

        }
        andaresChamados = andaresChamados->prox;
    }
}


void chamar(andar *andarChamador, elevador *elevadores) {
    elevador *chamado = NULL;

    while (elevadores) {
        if (andarChamador->pessoaSubindo && elevadores->direcao == 1) {

        }
    }
}









void printElevador(elevador *elevadorP) {
    printf("////////////////////////////////////////////////////////////\n");
    printf("Tempo atual: %d\n\n", tempoGeral);
    while (elevadorP) {
        printf("Elevador ID: %d\n", elevadorP->id);
        printf("Andar Atual: %d\n", elevadorP->andarAtual->andar);
        printf("Rota: ");

        andar *auxAndar = elevadorP->andaresDestino;
        while (auxAndar) {
            printf("%d ", auxAndar->andar);
            if (auxAndar->prox) {
                printf("-> ");
            }
            auxAndar = auxAndar->prox;
        }
        printf("\n\n");
        elevadorP = elevadorP->prox;

    }
    printf("////////////////////////////////////////////////////////////\n");
}





// AREA DE TESTES
void atualizarTempo() {
    tempoGeral++;
}






//void finish(no **noF) {
//    no *aux = *noF;
//    while (*noF) {
//        aux = aux->prox;
//        free(*noF);
//        *noF = aux;
//    }
//}

//void list(no *noLT) {
//    printf("[ ");
//    while (noLT) {
//        printf("%d ", noLT->andar);
//        if (noLT->prox) {
//            printf("-> ");
//        }
//        noLT = noLT->prox;
//    }
//    printf("]");
//}