#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Programa que realiza a leitura da string de eventos

typedef struct no{
    int andar;
    struct no *prox;
    struct no *ant;
}no;

void leitor(char *, no **, no **);
void createNo(no **noC, char *valor);
int verificarDirecao(char *);
void list(no *noLT);
void finish(no **noF);

int main() {
    char teste[] = "AM_25 E1_04_S_6,9,8 T5_P01_S_4_6";
    no *no1 = NULL;
    no *no2 = NULL;

    leitor(teste, &no1, &no2);
    finish(&no1);
    finish(&no2);
    return 0;
}

void leitor(char *_string, no **noL, no **noL2) {
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
            printf("Maximo de andares: %d\n", A);
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

                while (auxInterno) {
                    createNo(noL2, auxInterno);
                    auxInterno = strtok_r(NULL, ",", &saveInterno);
                }
                list(*noL2);
                printf("\n\n");
            }
        }

        auxExterno = strtok_r(NULL, " ", &saveExterno);
    }
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