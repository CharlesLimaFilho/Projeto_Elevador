#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Programa que realiza a leitura da string de eventos

typedef struct {
    int andar;
    struct no *prox;
    struct no *ant;
}no;

void leitor(char *, no **);
void createNo(no **noC, char *valor);
void finish(no **noF);

int main() {
    char teste[] = "AM_25 E1_04_S_6,9,8 E2_11_D_5,8,9,3,2,T E3_20_D_5,8,T,9";
    no *no1 = NULL;
    no *no2 = NULL;

    leitor(teste, &no1);

    finish(&no1);
    return 0;
}

void leitor(char *_string, no **noL) {
    char *auxExterno;
    char *auxInterno;
    char *saveExterno = NULL;
    char *saveInterno = NULL;
    int A;

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

            if (!strcmp(auxInterno, "S")) {
                printf("Subindo\n");
            } else if (!strcmp(auxInterno, "D")) {
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
        }

        auxExterno = strtok_r(NULL, " ", &saveExterno);
    }
}

void createNo(no **noC, char *valor) {
    no *aux = malloc(sizeof(no));
    aux->prox = NULL;
    aux->ant = NULL;
    aux->andar = atoi(valor);

    if (!*noC) {
        *noC = aux;
        return;
    } else {
        no *aux2 = *noC;
        while (aux2->prox) {
            aux2 = aux2->prox;
        }
        aux2->prox = aux;
        aux->ant = aux2;
    }
}

void list(no *noLT) {
    while (noLT) {
        printf("%d", noLT->andar);
        noLT = noLT->prox;
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