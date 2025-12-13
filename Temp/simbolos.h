#ifndef SIMBOLOS_H
#define SIMBOLOS_H

typedef struct Simbolo {
    char nome[50];        /* Nome do identificador */
    char tipo[10];        /* Tipo: "int", "void", "int[]" */
    char escopo[50];      /* Escopo: "global" ou nome da funcao */
    int linha;            /* Linha onde foi declarado */
    int ehFuncao;         /* 1 = funcao, 0 = variavel */
    int numParametros;    /* Numero de parametros (se for funcao) */
    struct Simbolo *prox; /* Ponteiro para proximo simbolo */
} Simbolo;

int inserirSimbolo(const char *nome, const char *tipo, const char *escopo, 
                   int linha, int ehFuncao, int numParams);

Simbolo* buscarSimbolo(const char *nome, const char *escopo);

Simbolo* buscarSimboloNoEscopo(const char *nome, const char *escopo);

void imprimirTabelaSimbolos(void);

void liberarTabelaSimbolos(void);

#endif
