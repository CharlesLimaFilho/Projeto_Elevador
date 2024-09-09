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



typedef struct no{
    int andar;
    struct no *prox;
    struct no *ant;
} no;

typedef struct pessoa{
    int id;
    int andarD;
    int direcao;
    struct pessoa *prox;
    struct pessoa *ant;
}pessoa;

typedef struct andar{
    int andar;
    struct andar *prox;
    struct andar *ant;
    pessoa *pessoa;
}andar;


typedef struct elevador{
    int direcao;// -1 - Descendo, 0 - Parado, 1 - Subindo
    pessoa *pessoasDentro;
    andar *andarAtual;
    andar *andaresDestino;
    andar *andaresChamado;
}elevador;

typedef struct eventos {
    int tempo;
    int andar;
    pessoa *pessoa;
    struct eventos *prox;
} gerenciador;



void leitor(char *, andar **, elevador **, gerenciador **ge);
void createEventos(gerenciador **gerente, int tempo, pessoa *pessoaG, int andarG);
void gerenciadorEventos();
void createNo(no **noC, char *valor);
pessoa *createPessoa(int id, char *a, int direcao);
void createAndar(andar **andares, int numAndar);
elevador *createElevador();
int verificarDirecao(char *);
void list(no *noLT);
void finish(no **noF);

//int main() {
//    char teste[] = "AM_25 E1_04_S_6,9,8 T5_P01_S_4_6";
//    no *no1 = NULL;
//    no *no2 = NULL;
//
//    leitor(teste, &no1, &no2);
//    finish(&no1);
//    finish(&no2);
//    return 0;
//}

void leitor(char *_string, andar **andares, elevador **elevadores, gerenciador **ge) {
    char *auxExterno;
    char *auxInterno;
    char *saveExterno = NULL;
    char *saveInterno = NULL;
    int A, T, V;

    auxExterno = strtok_r(_string, " ", &saveExterno);

    while (auxExterno != NULL) {
        auxInterno = strtok_r(auxExterno, "_", &saveInterno);

        if (!strcmp(auxInterno, "AM")) {
            auxInterno = strtok_r(NULL, " ", &saveInterno);
            A = atoi(auxInterno);
            for (int i = 1; i <= A; i++) {
                createAndar(andares, i);
            }
        } else if (!strncmp(auxInterno, "E", 1)) {
            A = atoi(auxInterno + 1);
            printf("Elevador: %d\n", A); // Printa qual elevador

            auxInterno = strtok_r(NULL, "_", &saveInterno);
            A = atoi(auxInterno);
            printf("Esta no andar: %d\n", A); // Printa andar onde o elevador esta

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

            auxInterno = strtok_r(NULL, ",", &saveInterno);

            while (auxInterno) {
                createNo(noL, auxInterno);
                auxInterno = strtok_r(NULL, ",", &saveInterno);
            }
            list(*noL);
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

                while (auxInterno) {
                    auxInterno = strtok_r(NULL, "_", &saveInterno);
                }
                printf("\n\n");
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


void createNo(no **noC, char *valor) {
    no *novo = malloc(sizeof(no));
    novo->prox = NULL;
    novo->ant = NULL;
    novo->andar = atoi(valor);

    if (!*noC) {
        *noC = novo;
        return;
    } else {
        no *aux2 = *noC;
        if (aux2->andar > novo->andar) {
            novo->prox = aux2;
            aux2->ant = novo;
            *noC = novo;
        } else {
            while (aux2->prox && novo->andar > (aux2->prox)->andar && aux2->prox->andar != novo->andar) {
                aux2 = aux2->prox;
            }
            if (aux2->andar == novo->andar || (aux2->prox && aux2->prox->andar == novo->andar)) {
                free(novo);
                return;
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



void createAndar(andar **andares, int numAndar) {
    andar *novo = malloc(sizeof(andar));
    novo->prox = NULL;
    novo->ant = NULL;
    novo->pessoa = NULL;
    novo->andar = numAndar;

    if (!*andares) {
        *andares = novo;
        return;
    } else {
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
    }
}

void list(no *noLT) {
    printf("[ ");
    while (noLT) {
        printf("%d ", noLT->andar);
        if (noLT->prox) {
            printf("-> ");
        }
        noLT = noLT->prox;
    }
    printf("]");
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

void finish(no **noF) {
    no *aux = *noF;
    while (*noF) {
        aux = aux->prox;
        free(*noF);
        *noF = aux;
    }
}