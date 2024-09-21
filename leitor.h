#ifndef LEITOR_H
#define LEITOR_H

typedef struct no{
    int andar;
    struct no *prox;
    struct no *ant;
}no;

typedef struct pessoa{
    int id;
    int andarD;
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
    int id;
    pessoa *pessoasDentro;
    andar *andarAtual;
    andar *andaresDestino;
    andar *andaresChamado;
    struct elevador *prox;
}elevador;

typedef struct eventos {
    int tempo;
    int andar;
    pessoa *pessoa;
    struct eventos *prox;
} gerenciador;


void leitor(char *_string, andar **andares, elevador **elevadores, gerenciador **ge);
void createEventos(gerenciador **gerente, int tempo, pessoa *pessoaG, int andarG);
void createAndar(andar **andares, int numAndar, int condicao);
pessoa *createPessoa(int id, char *a, int direcao);
void createElevador(elevador **elevadores, int id, int direcao, andar *andaresElevador);
void createNo(no **noC, char *valor);
int verificarDirecao(char *);
void list(no *noLT);
void finish(no **noF);

#endif
