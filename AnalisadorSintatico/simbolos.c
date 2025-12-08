/*
 * simbolos.c - Implementacao da Tabela de Simbolos
 * 
 * Utiliza uma lista ligada simples para armazenar os simbolos.
 * Esta e uma estrutura classica ensinada em Estruturas de Dados.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

/* Ponteiro para o inicio da lista (tabela de simbolos) */
static Simbolo *tabela = NULL;

/*
 * inserirSimbolo - Insere um novo simbolo na tabela
 * 
 * Antes de inserir, verifica se ja existe um simbolo com o mesmo
 * nome no mesmo escopo (isso seria uma redeclaracao - erro semantico).
 * 
 * Parametros:
 *   nome     - nome do identificador
 *   tipo     - tipo do identificador ("int", "void")
 *   escopo   - escopo onde foi declarado
 *   linha    - numero da linha da declaracao
 *   ehFuncao - 1 se for funcao, 0 se for variavel
 *   numParams- numero de parametros (apenas para funcoes)
 * 
 * Retorna: 1 se inseriu, 0 se ja existia
 */
int inserirSimbolo(const char *nome, const char *tipo, const char *escopo, 
                   int linha, int ehFuncao, int numParams) {
    
    /* Verifica se ja existe no mesmo escopo */
    if (buscarSimboloNoEscopo(nome, escopo) != NULL) {
        return 0;  /* Simbolo ja existe - nao insere */
    }

    /* Aloca memoria para o novo simbolo */
    Simbolo *novo = (Simbolo*) malloc(sizeof(Simbolo));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para simbolo\n");
        exit(1);
    }

    /* Copia os dados para o novo simbolo */
    strncpy(novo->nome, nome, sizeof(novo->nome) - 1);
    novo->nome[sizeof(novo->nome) - 1] = '\0';
    
    strncpy(novo->tipo, tipo, sizeof(novo->tipo) - 1);
    novo->tipo[sizeof(novo->tipo) - 1] = '\0';
    
    strncpy(novo->escopo, escopo, sizeof(novo->escopo) - 1);
    novo->escopo[sizeof(novo->escopo) - 1] = '\0';
    
    novo->linha = linha;
    novo->ehFuncao = ehFuncao;
    novo->numParametros = numParams;

    /* Insere no inicio da lista (mais eficiente) */
    novo->prox = tabela;
    tabela = novo;

    return 1;  /* Inserido com sucesso */
}

/*
 * buscarSimboloNoEscopo - Busca simbolo apenas no escopo especificado
 */
Simbolo* buscarSimboloNoEscopo(const char *nome, const char *escopo) {
    Simbolo *atual = tabela;
    
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0 && 
            strcmp(atual->escopo, escopo) == 0) {
            return atual;  /* Encontrou */
        }
        atual = atual->prox;
    }
    
    return NULL;  /* Nao encontrou */
}

/*
 * buscarSimbolo - Busca simbolo primeiro no escopo local, depois no global
 * 
 * Esta funcao implementa a regra de escopo: variaveis locais
 * tem precedencia sobre variaveis globais com o mesmo nome.
 */
Simbolo* buscarSimbolo(const char *nome, const char *escopo) {
    /* Primeiro busca no escopo local */
    Simbolo *s = buscarSimboloNoEscopo(nome, escopo);
    if (s != NULL) {
        return s;
    }
    
    /* Se nao encontrou, busca no escopo global */
    if (strcmp(escopo, "global") != 0) {
        s = buscarSimboloNoEscopo(nome, "global");
    }
    
    return s;  /* Retorna NULL se nao encontrou em nenhum escopo */
}

/*
 * imprimirTabelaSimbolos - Imprime toda a tabela de simbolos
 * 
 * Formato de saida conforme esperado na disciplina.
 */
void imprimirTabelaSimbolos(void) {
    printf("\n");
    printf("===============================================================================\n");
    printf("                          TABELA DE SIMBOLOS                                   \n");
    printf("===============================================================================\n");
    printf("%-20s %-10s %-15s %-8s %-10s\n", 
           "Nome", "Tipo", "Escopo", "Linha", "Categoria");
    printf("-------------------------------------------------------------------------------\n");
    
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
    
    printf("===============================================================================\n");
}

/*
 * liberarTabelaSimbolos - Libera toda a memoria alocada
 * 
 * Importante para evitar vazamentos de memoria (memory leaks).
 */
void liberarTabelaSimbolos(void) {
    Simbolo *atual = tabela;
    while (atual != NULL) {
        Simbolo *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    tabela = NULL;
}
