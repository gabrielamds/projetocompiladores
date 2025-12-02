#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

static Simbolo *tabela = NULL; // lista ligada que representa a tabela

// Insere no início da lista
void inserirSimbolo(char *nome, char *tipo, char *escopo, int linha) {
    // Verifica duplicidade no mesmo escopo (pode adaptar)
    if (buscarSimbolo(nome, escopo) != NULL) {
        printf("Aviso: Simbolo '%s' ja declarado no escopo '%s'\n", nome, escopo);
        return;
    }

    Simbolo *novo = (Simbolo *) malloc(sizeof(Simbolo));
    if (!novo) {
        fprintf(stderr, "Erro de memória na inserção!\n");
        exit(1);
    }

    strcpy(novo->nome, nome);
    strcpy(novo->tipo, tipo);
    strcpy(novo->escopo, escopo);
    novo->linha = linha;

    novo->prox = tabela;
    tabela = novo;
}

// Busca lista ligada por nome e escopo
Simbolo* buscarSimbolo(char *nome, char *escopo) {
    Simbolo *atual = tabela;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0 && strcmp(atual->escopo, escopo) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Imprime todos os simbolos na tabela
void imprimirTabela(void) {
    printf("\n=== TABELA DE SIMBOLOS ===\n");
    printf("%-15s %-10s %-15s %-5s\n", "Nome", "Tipo", "Escopo", "Linha");
    printf("----------------------------------------\n");
    Simbolo *atual = tabela;
    while (atual != NULL) {
        printf("%-15s %-10s %-15s %-5d\n", atual->nome, atual->tipo, atual->escopo, atual->linha);
        atual = atual->prox;
    }
}

// Libera toda a memória alocada
void liberarTabela(void) {
    Simbolo *atual = tabela;
    while (atual != NULL) {
        Simbolo *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    tabela = NULL;
}
