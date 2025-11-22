#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da struct para representar uma peça do Tetris
struct Peca {
    char nome;  // Tipo da peça: I, O, T, L
    int id;     // Identificador único
};

// Definição da fila circular
#define TAMANHO_FILA 5

struct Fila {
    struct Peca itens[TAMANHO_FILA];
    int frente;
    int tras;
    int tamanho;
    int contador_id; // Contador para gerar IDs únicos
};

// Protótipos das funções
void inicializarFila(struct Fila *fila);
int filaVazia(struct Fila *fila);
int filaCheia(struct Fila *fila);
void enqueue(struct Fila *fila, struct Peca peca);
struct Peca dequeue(struct Fila *fila);
void exibirFila(struct Fila *fila);
struct Peca gerarPeca(struct Fila *fila);
void preencherFilaInicial(struct Fila *fila);

int main() {
    struct Fila fila;
    int opcao;
    
    // Inicialização da semente para números aleatórios
    srand(time(NULL));
    
    // Inicializar a fila com peças
    inicializarFila(&fila);
    preencherFilaInicial(&fila);
    
    printf("=== TETRIS STACK - SISTEMA DE FILA DE PEÇAS ===\n");
    
    do {
        // Exibir estado atual da fila
        printf("\n--- ESTADO ATUAL DA FILA ---\n");
        exibirFila(&fila);
        
        // Menu de opções
        printf("\n=== OPÇÕES DE AÇÃO ===\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 0:
                printf("\nSaindo do sistema...\n");
                printf("Obrigado por jogar Tetris Stack!\n");
                break;
                
            case 1:
                // Jogar peça - remover da frente
                if (!filaVazia(&fila)) {
                    struct Peca pecaRemovida = dequeue(&fila);
                    printf("\n✅ Peça jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
                } else {
                    printf("\n❌ Erro: Fila vazia! Não há peças para jogar.\n");
                }
                break;
                
            case 2:
                // Inserir nova peça
                if (!filaCheia(&fila)) {
                    struct Peca novaPeca = gerarPeca(&fila);
                    enqueue(&fila, novaPeca);
                    printf("\n✅ Nova peça adicionada: [%c %d]\n", novaPeca.nome, novaPeca.id);
                } else {
                    printf("\n❌ Erro: Fila cheia! Capacidade máxima: %d peças.\n", TAMANHO_FILA);
                }
                break;
                
            default:
                printf("\n❌ Opção inválida! Digite 0, 1 ou 2.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}

// Função para inicializar a fila
void inicializarFila(struct Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    fila->contador_id = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(struct Fila *fila) {
    return (fila->tamanho == 0);
}

// Função para verificar se a fila está cheia
int filaCheia(struct Fila *fila) {
    return (fila->tamanho == TAMANHO_FILA);
}

// Função para adicionar uma peça ao final da fila (enqueue)
void enqueue(struct Fila *fila, struct Peca peca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    // Atualiza a posição traseira (circular)
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->itens[fila->tras] = peca;
    fila->tamanho++;
}

// Função para remover uma peça do início da fila (dequeue)
struct Peca dequeue(struct Fila *fila) {
    struct Peca pecaVazia = {' ', -1}; // Peça vazia para erro
    
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia!\n");
        return pecaVazia;
    }
    
    // Obtém a peça da frente
    struct Peca peca = fila->itens[fila->frente];
    
    // Atualiza a posição frontal (circular)
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    return peca;
}

// Função para exibir o estado atual da fila
void exibirFila(struct Fila *fila) {
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    
    printf("Fila de peças (%d/%d): ", fila->tamanho, TAMANHO_FILA);
    
    // Percorre a fila a partir da frente
    int posicao = fila->frente;
    for (int i = 0; i < fila->tamanho; i++) {
        printf("[%c %d] ", fila->itens[posicao].nome, fila->itens[posicao].id);
        posicao = (posicao + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

// Função para gerar uma nova peça aleatória
struct Peca gerarPeca(struct Fila *fila) {
    struct Peca novaPeca;
    
    // Tipos de peças disponíveis
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = 4;
    
    // Gera tipo aleatório
    novaPeca.nome = tipos[rand() % numTipos];
    
    // Atribui ID único e incrementa contador
    novaPeca.id = fila->contador_id;
    fila->contador_id++;
    
    return novaPeca;
}

// Função para preencher a fila com peças iniciais
void preencherFilaInicial(struct Fila *fila) {
    // Gera 5 peças iniciais para a fila
    for (int i = 0; i < TAMANHO_FILA; i++) {
        struct Peca peca = gerarPeca(fila);
        enqueue(fila, peca);
    }
}