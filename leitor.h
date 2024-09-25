#ifndef LEITOR_H
#define LEITOR_H

typedef struct pessoa1{
    int id;
    int andarD;
    int direcao;
    int chamou; // 1 - Sim, 0 - Nao
    struct pessoa1 *prox;
    struct pessoa1 *ant;
}pessoa;

typedef struct andar{
    int andar;
    struct andar *prox;
    struct andar *ant;
    pessoa *pessoaSubindo;
    pessoa *pessoaDescendo;
}andar;


typedef struct elevador{
    int direcao;// -1 - Descendo, 0 - Parado, 1 - Subindo
    int id;
    int moves;
    pessoa *pessoasDentro;
    andar *andarAtual;
    andar *andaresDestino;
    andar *andaresChamado;
    andar *andarReserva;
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
void moveElevadores(elevador *elevadorMove);
void createElevador(elevador **elevadores, int id, int direcao, andar *andaresElevador);
void inserirPessoasAndar(andar *andarInserir, pessoa *pessoaInserir);
int verificarDirecao(char *);
void printElevador(elevador *elevadorP);
void gerenciarEventos(andar *andarGerenciar, gerenciador **gerencia);
void removerPessoasElevador(elevador *elevadorGerenciar);
void inserirPessoasElevador(elevador *elevadorAndar);

void atualizarTempo();
void ativar(andar *andares, elevador *elevadores, gerenciador *gerente);

#endif
