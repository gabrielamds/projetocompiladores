#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

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
