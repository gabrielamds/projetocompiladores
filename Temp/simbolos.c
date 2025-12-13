#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

/* Ponteiro para o inicio da lista (cabeca da tabela) */
static Simbolo *tabela = NULL;

int inserirSimbolo(const char *nome, const char *tipo, const char *escopo, 
                   int linha, int ehFuncao, int numParams) {
    
    if (buscarSimboloNoEscopo(nome, escopo) != NULL) {
        return 0;  /* Simbolo ja existe - nao insere */
    }

    Simbolo *novo = (Simbolo*) malloc(sizeof(Simbolo));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para simbolo\n");
        exit(1);
    }

    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    
    strncpy(novo->tipo, tipo, sizeof(novo->tipo) - 1);
    novo->tipo[sizeof(novo->tipo) - 1] = '\0';
    
    strncpy(novo->escopo, escopo, sizeof(novo->escopo) - 1);
    novo->escopo[sizeof(novo->escopo) - 1] = '\0';
    
    novo->linha = linha;
    novo->ehFuncao = ehFuncao;
    novo->numParametros = numParams;

    novo->prox = tabela;
    tabela = novo;

    return 1;  /* Inserido com sucesso */
}

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

void liberarTabelaSimbolos(void) {
    Simbolo *atual = tabela;
    while (atual != NULL) {
        Simbolo *temp = atual;
        atual = atual->prox;
        free(temp);  /* Libera o no */
    }
    tabela = NULL;  /* Reseta o ponteiro da cabeca */
}
