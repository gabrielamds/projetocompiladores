/*
 * ============================================================================
 * ARVORE SINTATICA - Implementacao
 * ============================================================================
 * 
 * Valor: 1,5 pontos
 * 
 * Implementacao das funcoes para manipulacao da arvore sintatica.
 * 
 * A arvore e construida durante a analise sintatica (parsing) usando
 * as funcoes criarNo() e adicionarFilho() nas acoes semanticas do Bison.
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

/*
 * criarNo - Aloca e inicializa um novo no da arvore
 * 
 * Esta funcao e chamada nas acoes semanticas do parser para criar
 * os nos que formarao a arvore sintatica.
 */
NoArvore* criarNo(const char* tipo, const char* valor, int linha) {
    /* Aloca memoria para o no */
    NoArvore* no = (NoArvore*) malloc(sizeof(NoArvore));
    if (!no) {
        fprintf(stderr, "Erro de alocacao de memoria para NoArvore\n");
        exit(1);
    }
    
    /* Copia o tipo (usa strncpy para seguranca) */
    strncpy(no->tipo, tipo, sizeof(no->tipo) - 1);
    no->tipo[sizeof(no->tipo) - 1] = '\0';

    /* Copia o valor (se fornecido) */
    if (valor != NULL) {
        strncpy(no->valor, valor, sizeof(no->valor) - 1);
        no->valor[sizeof(no->valor) - 1] = '\0';
    } else {
        no->valor[0] = '\0';  /* String vazia */
    }

    /* Inicializa outros campos */
    no->linha = linha;
    no->nFilhos = 0;
    
    /* Inicializa todos os ponteiros de filhos como NULL */
    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filhos[i] = NULL;
    }
    
    return no;
}

/*
 * adicionarFilho - Adiciona um no como filho de outro
 * 
 * Esta funcao conecta os nos para formar a estrutura da arvore.
 * Cada no pode ter no maximo MAX_FILHOS filhos.
 */
void adicionarFilho(NoArvore* pai, NoArvore* filho) {
    /* Verifica se os ponteiros sao validos */
    if (pai == NULL || filho == NULL) {
        return;
    }
    
    /* Verifica se ainda ha espaco para mais filhos */
    if (pai->nFilhos < MAX_FILHOS) {
        pai->filhos[pai->nFilhos] = filho;
        pai->nFilhos++;
    } else {
        fprintf(stderr, "Aviso: numero maximo de filhos excedido\n");
    }
}

/*
 * imprimirArvore - Exibe a arvore de forma visual
 * 
 * Usa recursao para percorrer a arvore em profundidade (DFS).
 * A indentacao mostra a hierarquia dos nos.
 * 
 * Exemplo de saida:
 *   - PROGRAMA [linha 1]
 *     - DECL [linha 2]
 *       - VAR (x) [linha 2]
 *         - TIPO (int) [linha 2]
 */
void imprimirArvore(NoArvore* no, int nivel) {
    /* Caso base: no nulo */
    if (no == NULL) {
        return;
    }
    
    /* Imprime indentacao (2 espacos por nivel) */
    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }
    
    /* Imprime o no */
    printf("- %s", no->tipo);
    
    /* Se tem valor, imprime entre parenteses */
    if (no->valor[0] != '\0') {
        printf(" (%s)", no->valor);
    }
    
    /* Imprime o numero da linha */
    printf(" [linha %d]\n", no->linha);
    
    /* Recursivamente imprime os filhos */
    for (int i = 0; i < no->nFilhos; i++) {
        imprimirArvore(no->filhos[i], nivel + 1);
    }
}
