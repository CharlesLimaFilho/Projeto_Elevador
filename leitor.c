#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Programa que realiza a leitura da string de eventos

typedef struct {
    int andar;
    struct no *prox;
}no;

void leitor(char *, no **);


int main() {
    char teste[] = "AM_25 E1_04_S_6,9,8";
    no *no = NULL;

    leitor(teste, &no);


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

            }
        }

        auxExterno = strtok_r(NULL, " ", &saveExterno);
    }
}