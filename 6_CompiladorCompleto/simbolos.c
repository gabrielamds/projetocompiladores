/*
 * ============================================================================
 * TABELA DE SIMBOLOS - Implementacao
 * ============================================================================
 * 
 * Valor: 1,5 pontos
 * 
 * Implementacao usando Lista Ligada Simples.
 * 
 * A lista ligada foi escolhida por ser uma estrutura classica ensinada
 * em Estruturas de Dados e adequada para tabelas de simbolos de
 * compiladores didaticos.
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

/* Ponteiro para o inicio da lista (cabeca da tabela) */
static Simbolo *tabela = NULL;

/*
 * inserirSimbolo - Insere um novo simbolo na tabela
 * 
 * Complexidade: O(n) devido a verificacao de duplicidade
 * Insercao propriamente dita: O(1) - no inicio da lista
 */
int inserirSimbolo(const char *nome, const char *tipo, const char *escopo, 
                   int linha, int ehFuncao, int numParams) {
    
    /* 
     * PASSO 1: Verifica se ja existe no mesmo escopo
     * Isso evita redeclaracao de variaveis/funcoes
     */
    if (buscarSimboloNoEscopo(nome, escopo) != NULL) {
        return 0;  /* Simbolo ja existe - nao insere */
    }

    /*
     * PASSO 2: Aloca memoria para o novo no
     */
    Simbolo *novo = (Simbolo*) malloc(sizeof(Simbolo));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para simbolo\n");
        exit(1);
    }

    /*
     * PASSO 3: Copia os dados para o novo simbolo
     * Usa strncpy para evitar buffer overflow
     */
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    
    strncpy(novo->tipo, tipo, sizeof(novo->tipo) - 1);
    novo->tipo[sizeof(novo->tipo) - 1] = '\0';
    
    strncpy(novo->escopo, escopo, sizeof(novo->escopo) - 1);
    novo->escopo[sizeof(novo->escopo) - 1] = '\0';
    
    novo->linha = linha;
    novo->ehFuncao = ehFuncao;
    novo->numParametros = numParams;

    /*
     * PASSO 4: Insere no inicio da lista
     * Tecnica padrao de insercao em lista ligada
     */
    novo->prox = tabela;
    tabela = novo;

    return 1;  /* Inserido com sucesso */
}

/*
 * buscarSimboloNoEscopo - Busca linear na lista
 * 
 * Complexidade: O(n)
 */
Simbolo* buscarSimboloNoEscopo(const char *nome, const char *escopo) {
    Simbolo *atual = tabela;
    
    /* Percorre toda a lista */
    while (atual != NULL) {
        /* Compara nome E escopo */
        if (strcmp(atual->nome, nome) == 0 && 
            strcmp(atual->escopo, escopo) == 0) {
            return atual;  /* Encontrou! */
        }
        atual = atual->prox;
    }
    
    return NULL;  /* Nao encontrou */
}

/*
 * buscarSimbolo - Busca com regra de escopo
 * 
 * Primeiro busca no escopo local, depois no global.
 * Isso implementa a semantica de que variaveis locais
 * "escondem" variaveis globais de mesmo nome.
 */
Simbolo* buscarSimbolo(const char *nome, const char *escopo) {
    /* Tenta encontrar no escopo local primeiro */
    Simbolo *s = buscarSimboloNoEscopo(nome, escopo);
    if (s != NULL) {
        return s;
    }
    
    /* Se nao encontrou e nao esta no global, busca no global */
    if (strcmp(escopo, "global") != 0) {
        s = buscarSimboloNoEscopo(nome, "global");
    }
    
    return s;
}

/*
 * imprimirTabelaSimbolos - Exibe a tabela formatada
 * 
 * Formato de saida conforme esperado na disciplina.
 */
void imprimirTabelaSimbolos(void) {
    printf("\n");
    printf("================================================================================\n");
    printf("                           TABELA DE SIMBOLOS                                   \n");
    printf("================================================================================\n");
    printf("%-20s %-10s %-15s %-8s %-10s\n", 
           "Nome", "Tipo", "Escopo", "Linha", "Categoria");
    printf("--------------------------------------------------------------------------------\n");
    
    Simbolo *atual = tabela;
    while (atual != NULL) {
        printf("%-20s %-10s %-15s %-8d %-10s\n",
               atual->nome,
               atual->tipo,
               atual->escopo,
               atual->linha,
               atual->ehFuncao ? "funcao" : "variavel");
        atual = atual->prox;
    }
    
    printf("================================================================================\n");
}

/*
 * liberarTabelaSimbolos - Desaloca toda a memoria
 * 
 * Importante para evitar memory leaks.
 * Percorre a lista liberando cada no.
 */
void liberarTabelaSimbolos(void) {
    Simbolo *atual = tabela;
    while (atual != NULL) {
        Simbolo *temp = atual;
        atual = atual->prox;
        free(temp);  /* Libera o no */
    }
    tabela = NULL;  /* Reseta o ponteiro da cabeca */
}
