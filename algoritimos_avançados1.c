#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para cada cômodo da mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* * Função: criarSala
 * Objetivo: Cria, de forma dinâmica usando malloc, uma sala com o nome fornecido.
 * Parâmetros: nome - string com o nome do cômodo
 * Retorno: Ponteiro para a nova Sala criada
 */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro Crítico: Falha na alocação de memória.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/* * Função: explorarSalas
 * Objetivo: Permite a navegação interativa do jogador pela árvore até alcançar uma folha ou sair.
 * Parâmetros: raiz - ponteiro para o nó inicial (Hall de entrada)
 */
void explorarSalas(Sala *raiz) {
    if (raiz == NULL) {
        printf("A mansão está vazia!\n");
        return;
    }

    Sala *atual = raiz;
    char escolha;

    printf("\n--- Iniciando a Investigacao em Detective Quest ---\n");

    while (atual != NULL) {
        // Exibe o cômodo atual visitado
        printf("\nVoce esta na sala: %s\n", atual->nome);

        // Verifica se alcançou um nó-folha (fim dos caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce alcancou um ponto sem saida! Fim da exploracao.\n");
            break;
        }

        // Mostra as opções disponíveis para o jogador
        printf("Caminhos disponiveis:\n");
        if (atual->esquerda != NULL) printf(" [e] Ir para a Esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf(" [d] Ir para a Direita (%s)\n", atual->direita->nome);
        printf(" [s] Sair do jogo\n");
        
        printf("Escolha sua direcao: ");
        scanf(" %c", &escolha); // O espaço antes de %c limpa o buffer do teclado

        if (escolha == 's' || escolha == 'S') {
            printf("Saindo da investigacao...\n");
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Escolha invalida ou caminho bloqueado! Tente novamente.\n");
        }
    }
    
    printf("\nObrigado por jogar Detective Quest!\n");
}

/* * Função: liberarMansao (Auxiliar)
 * Objetivo: Desalocar recursivamente a memória da árvore para evitar vazamento de memória.
 */
void liberarMansao(Sala *raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}

/* * Função: main
 * Objetivo: Monta o mapa inicial da mansão automaticamente e dá início à exploração.
 */
int main() {
    // 1. Montagem automática da árvore (Mapa da Mansão)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim Oculto");
    Sala *laboratorio = criarSala("Laboratorio Secreto");
    Sala *porao = criarSala("Porao Escuro");

    // Conectando os nós para formar a árvore binária
    hall->esquerda = biblioteca;
    hall->direita = cozinha;

    biblioteca->esquerda = jardim;
    biblioteca->direita = laboratorio;

    cozinha->esquerda = porao; // Cozinha só tem caminho para o porão, por exemplo

    // 2. Início do fluxo de exploração interativa
    explorarSalas(hall);

    // 3. Limpeza de memória ao fechar o programa
    liberarMansao(hall);

    return 0;
}