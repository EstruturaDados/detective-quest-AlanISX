#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 11 // Tamanho definido para a Tabela Hash

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

// REQUISITO: Estrutura para os nós da Tabela Hash (Tratamento de colisão por encadeamento)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

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

/* * Função: funcaoHash (Nova)
 * Objetivo: Calcula o índice correspondente a uma string (pista) na tabela hash.
 * Parâmetros: chave - a string da pista
 * Retorno: Índice calculado (0 a HASH_SIZE - 1)
 */
int funcaoHash(const char *chave) {
    unsigned long int valor = 0;
    int i = 0;
    while (chave[i] != '\0') {
        valor = valor * 37 + chave[i];
        i++;
    }
    return valor % HASH_SIZE;
}

/* * Função: inserirNaHash (Nova)
 * Objetivo: Insere uma associação pista/suspeito na tabela hash.
 * Parâmetros: tabela - vetor de ponteiros da tabela, pista - chave, suspeito - valor
 */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (novo == NULL) {
        printf("Erro Crítico: Falha na alocação de memória para a tabela hash.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

/* * Função: encontrarSuspeito (Nova)
 * Objetivo: Consulta o suspeito correspondente a uma pista na tabela hash.
 * Parâmetros: tabela - vetor de ponteiros da tabela, pista - chave para busca
 * Retorno: String contendo o nome do suspeito ou NULL se não encontrado
 */
const char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Correção: Linha duplicada errada removida
        }
        atual = atual->proximo;
    }
    return NULL;
}

/* * Função: liberarTabelaHash (Nova)
 * Objetivo: Libera toda a memória alocada dinamicamente para a tabela hash.
 * Parâmetros: tabela - vetor de ponteiros da tabela hash
 */
void liberarTabelaHash(HashNode *tabela[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *atual = tabela[i];
        while (atual != NULL) {
            HashNode *aux = atual;
            atual = atual->proximo;
            free(aux);
        }
        tabela[i] = NULL;
    }
}

/* * Função: contarPistasSuspeito (Nova)
 * Objetivo: Percorre recursivamente a BST de pistas coletadas e conta quantas apontam para o suspeito acusado.
 * Parâmetros: raiz - raiz da BST de pistas, tabela - tabela hash, acusado - nome do suspeito testado
 * Retorno: Quantidade de pistas que apontam para o acusado
 */
int contarPistasSuspeito(PistaNode *raiz, HashNode *tabela[], const char *acusado) {
    if (raiz == NULL) return 0;
    
    int contaAtual = 0;
    const char *suspeitoAssociado = encontrarSuspeito(tabela, raiz->texto);
    if (suspeitoAssociado != NULL && strcmp(suspeitoAssociado, acusado) == 0) {
        contaAtual = 1;
    }
    
    return contaAtual + contarPistasSuspeito(raiz->esquerda, tabela, acusado) 
                      + contarPistasSuspeito(raiz->direita, tabela, acusado);
}

/* * Função: verificarSuspeitoFinal (Nova)
 * Objetivo: Conduz à fase de julgamento final, avaliando se há pistas suficientes contra o acusado.
 * Parâmetros: raizPistas - raiz da BST de pistas coletadas, tabela - tabela hash de relações
 */
void verificarSuspeitoFinal(PistaNode *raizPistas, HashNode *tabela[]) {
    char acusado[50];
    printf("\n================ FASE DE JULGAMENTO ================\n");
    printf("Digite o nome do suspeito que você deseja acusar oficialmente: ");
    scanf(" %[^\n]", acusado); // Lê a string contendo espaços

    int pistasEncontradas = contarPistasSuspeito(raizPistas, tabela, acusado);

    printf("\nAnalisando as evidências contra %s...\n", acusado);
    printf("Total de pistas válidas coletadas que apontam para ele(a): %d\n", pistasEncontradas);

    if (pistasEncontradas >= 2) {
        printf("\n[SUCESSO] PARABÉNS! Você reuniu provas incontestáveis e solucionou o caso! O culpado está preso.\n");
    } else {
        printf("\n[FRACASSO] Acusação inválida! Provas insuficientes. O suspeito foi liberado por falta de evidências e o verdadeiro culpado escapou!\n");
    }
    printf("====================================================\n");
}

/* * Função: explorarSalasComPistasNova (Nova)
 * Objetivo: Controla a navegação entre salas e gerencia a coleta automática de pistas em uma BST e julgamento.
 * Parâmetros: raiz - ponteiro para o nó inicial (Hall de entrada), tabela - tabela hash de suspeitos
 */
void explorarSalasComPistasNova(Sala *raiz, HashNode *tabela[]) {
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

    // REQUISITO: Fase de julgamento acionada se houver jogo executado
    verificarSuspeitoFinal(bstPistas, tabela);

    // Limpeza da árvore de pistas
    liberarPistas(bstPistas);

    printf("\nObrigado por jogar Detective Quest!\n");
}

/* * Função original mantida intacta por compatibilidade */
void explorarSalasComPistas(Sala *raiz) {
    if (raiz == NULL) {
        printf("A mansão está vazia!\n");
        return;
    }

    Sala *atual = raiz;
    PistaNode *bstPistas = NULL; 
    char escolha;

    printf("\n--- Iniciando a Investigacao em Detective Quest ---\n");

    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("[PISTA ENCONTRADA]: %s\n", atual->pista);
            bstPistas = inserirPista(bstPistas, atual->pista);
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce alcancou um ponto sem saida! Fim da exploracao.\n");
            break;
        }

        printf("Caminhos disponiveis:\n");
        if (atual->esquerda != NULL) printf(" [e] Ir para a Esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf(" [d] Ir para a Direita (%s)\n", atual->direita->nome);
        printf(" [s] Sair do jogo\n");
        
        printf("Escolha sua direcao: ");
        scanf(" %c", &escolha); 

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
    
    printf("\n--- INVENTARIO DE PISTAS COLETADAS (Ordem Alfabetica) ---\n");
    if (bstPistas == NULL) {
        printf("Nenhuma pista foi coletada durante a jornada.\n");
    } else {
        exibirPistas(bstPistas);
    }
    printf("-------------------------------------------------------\n");
    liberarPistas(bstPistas);
    printf("\nObrigado por jogar Detective Quest!\n");
}

/* * Função: explorarSalas (Original mantida por compatibilidade / preservação) */
void explorarSalas(Sala *raiz) {
    if (raiz == NULL) {
        printf("A mansão está vazia!\n");
        return;
    }

    Sala *atual = raiz;
    char escolha;

    printf("\n--- Iniciando a Investigacao em Detective Quest ---\n");

    while (atual != NULL) {
        printf("\nVoce esta na sala: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce alcancou um ponto sem saida! Fim da exploracao.\n");
            break;
        }

        printf("Caminhos disponiveis:\n");
        if (atual->esquerda != NULL) printf(" [e] Ir para a Esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf(" [d] Ir para a Direita (%s)\n", atual->direita->nome);
        printf(" [s] Sair do jogo\n");
        
        printf("Escolha sua direcao: ");
        scanf(" %c", &escolha); 

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

/* * Função: liberarMansao (Auxiliar) */
void liberarMansao(Sala *raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}

/* * Função: main */
int main() {
    // Inicialização da Tabela Hash vazia
    HashNode *tabelaHash[HASH_SIZE] = {NULL};

    // Populando a Tabela Hash com os suspeitos de forma estática
    inserirNaHash(tabelaHash, "Pegadas de lama perto da porta", "Mordomo");
    inserirNaHash(tabelaHash, "Um bilhete rasgado dentro de um livro antigo", "Coveiro");
    inserirNaHash(tabelaHash, "Uma xicara ainda morna sobre o balcao", "Mordomo");
    inserirNaHash(tabelaHash, "Terra mexida recentemente sob a roseira", "Coveiro");
    inserirNaHash(tabelaHash, "Frasco quebrado com liquido azul fluorescente", "Cientista");
    inserirNaHash(tabelaHash, "Uma chave dourada enferrujada no chao", "Cientista");

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

    cozinha->esquerda = porao; 

    // 2. Início do fluxo de exploração interativa (Acionando a função com suporte à Hash)
    explorarSalasComPistasNova(hall, tabelaHash);

    // 3. Limpeza de memória ao fechar o programa
    liberarMansao(hall);
    liberarTabelaHash(tabelaHash);

    return 0;
}