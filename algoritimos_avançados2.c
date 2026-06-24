#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para cada cômodo da mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    char pista[100]; // REQUISITO: Nova propriedade para armazenar a pista do cômodo
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// REQUISITO: Definição da estrutura da árvore BST para armazenar as pistas coletadas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* * Função: criarSala
 * Objetivo: Cria, de forma dinâmica usando malloc, uma sala com o nome fornecido.
 * Parâmetros: nome - string com o nome do cômodo
 * Retorno: Ponteiro para a nova Sala criada
 * OBS: Foi adicionado o parâmetro opcional 'pista' conforme novos requisitos.
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro Crítico: Falha na alocação de memória.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    
    // Inicializa a pista se ela existir, caso contrário deixa string vazia
    if (pista != NULL) {
        strcpy(novaSala->pista, pista);
    } else {
        strcpy(novaSala->pista, "");
    }
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/* * Função: inserirPista (Nova)
 * Objetivo: Insere recursivamente uma nova pista na árvore BST em ordem alfabética.
 * Parâmetros: raiz - ponteiro para o nó atual da BST, texto - a pista a ser inserida
 * Retorno: Ponteiro para o nó atualizado da BST
 */
PistaNode* inserirPista(PistaNode *raiz, const char *texto) {
    if (raiz == NULL) {
        PistaNode *novoNode = (PistaNode*) malloc(sizeof(PistaNode));
        if (novoNode == NULL) {
            printf("Erro Crítico: Falha na alocação de memória para pista.\n");
            exit(1);
        }
        strcpy(novoNode->texto, texto);
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;
        return novoNode;
    }

    // Comparação alfabética para inserção na BST
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    // Se forem iguais, não faz nada (evita duplicatas)

    return raiz;
}

/* * Função: exibirPistas (Nova)
 * Objetivo: Imprime todas as pistas inseridas na BST em ordem alfabética (In-order traversal).
 * Parâmetros: raiz - ponteiro para a raiz da BST de pistas
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

/* * Função: liberarPistas (Nova)
 * Objetivo: Desaloca recursivamente a memória da árvore BST de pistas.
 * Parâmetros: raiz - ponteiro para a raiz da árvore BST
 */
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

/* * Função: explorarSalasComPistas (Nova)
 * Objetivo: Controla a navegação entre salas e gerencia a coleta automática de pistas em uma BST.
 * Parâmetros: raiz - ponteiro para o nó inicial (Hall de entrada)
 */
void explorarSalasComPistas(Sala *raiz) {
    if (raiz == NULL) {
        printf("A mansão está vazia!\n");
        return;
    }

    Sala *atual = raiz;
    PistaNode *bstPistas = NULL; // Inicializa a árvore de pistas vazia
    char escolha;

    printf("\n--- Iniciando a Investigacao em Detective Quest ---\n");

    while (atual != NULL) {
        // Exibe o cômodo atual visitado
        printf("\nVoce esta na sala: %s\n", atual->nome);

        // Coleta automática de pista se houver uma associada ao cômodo
        if (strlen(atual->pista) > 0) {
            printf("[PISTA ENCONTRADA]: %s\n", atual->pista);
            bstPistas = inserirPista(bstPistas, atual->pista);
        }

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
    
    // Exibição do relatório final das pistas coletadas em ordem alfabética
    printf("\n--- INVENTARIO DE PISTAS COLETADAS (Ordem Alfabetica) ---\n");
    if (bstPistas == NULL) {
        printf("Nenhuma pista foi coletada durante a jornada.\n");
    } else {
        exibirPistas(bstPistas);
    }
    printf("-------------------------------------------------------\n");

    // Limpeza da árvore de pistas
    liberarPistas(bstPistas);

    printf("\nObrigado por jogar Detective Quest!\n");
}

/* * Função: explorarSalas (Original mantida por compatibilidade / preservação)
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
    // 1. Montagem automática da árvore (Mapa da Mansão atualizado com pistas)
    Sala *hall = criarSala("Hall de Entrada", "Pegadas de lama perto da porta");
    Sala *biblioteca = criarSala("Biblioteca", "Um bilhete rasgado dentro de um livro antigo");
    Sala *cozinha = criarSala("Cozinha", "Uma xicara ainda morna sobre o balcao");
    Sala *jardim = criarSala("Jardim Oculto", "Terra mexida recentemente sob a roseira");
    Sala *laboratorio = criarSala("Laboratorio Secreto", "Frasco quebrado com liquido azul fluorescente");
    Sala *porao = criarSala("Porao Escuro", "Uma chave dourada enferrujada no chao");

    // Conectando os nós para formar a árvore binária
    hall->esquerda = biblioteca;
    hall->direita = cozinha;

    biblioteca->esquerda = jardim;
    biblioteca->direita = laboratorio;

    cozinha->esquerda = porao; // Cozinha só tem caminho para o porão, por exemplo

    // 2. Início do fluxo de exploração interativa (Alterado para a nova função com controle de pistas)
    explorarSalasComPistas(hall);

    // 3. Limpeza de memória ao fechar o programa
    liberarMansao(hall);

    return 0;
}