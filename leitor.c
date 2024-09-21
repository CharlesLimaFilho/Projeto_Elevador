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
    pessoa *pessoa;
}andar;


typedef struct elevador1{
    int direcao;// -1 - Descendo, 0 - Parado, 1 - Subindo
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
void gerenciadorEventos();
pessoa *createPessoa(int id, char *a, int direcao);
void createAndar(andar **andares, int numAndar, int condicao);
void createElevador(elevador **elevadores, int id, int direcao, andar *andaresElevador);
int verificarDirecao(char *);
void inserirElevadorAndar(elevador *elevadorP, andar *andares, int andar, int id);


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

                createEventos(ge, T, createPessoa(A, auxInterno, V), atoi(auxInterno));
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
    return;
}

pessoa *createPessoa(int id, char *a, int direcao) {
    pessoa *novo = malloc(sizeof(pessoa));
    novo->ant = NULL;
    novo->prox = NULL;
    novo->direcao = direcao;
    novo->andarD = atoi(a + 2);
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
    novo->pessoa = NULL;
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