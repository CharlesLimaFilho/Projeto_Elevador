#include <stdio.h>
#include "leitor.h"
#include <unistd.h>

int main() {
    char teste[] = "AM_25 E1_25_D_6,9,4 T01_P01_D_24_20 T01_P04_D_24_14 T01_P05_D_24_T T01_P06_S_24_25 T02_P02_D_21_13";
    andar *andares = NULL;
    elevador *elevadores = NULL;
    gerenciador *gerente = NULL;

    leitor(teste, &andares, &elevadores, &gerente);

    for (int i = 0; i < 30; i++) {
        gerenciarEventos(andares, &gerente);
        inserirPessoasElevador(elevadores);
        removerPessoasElevador(elevadores);
        printElevador(elevadores);
        moveElevadores(elevadores);
        atualizarTempo();
        //sleep(1);
    }


    return 0;
}