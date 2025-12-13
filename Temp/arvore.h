#ifndef ARVORE_H
#define ARVORE_H

/* Numero maximo de filhos por no */
#define MAX_FILHOS 5

typedef struct NoArvore {
    char tipo[30];                    /* Tipo do no (ex: "IF", "WHILE", "VAR") */
    char valor[30];                   /* Valor associado (nome, operador, etc) */
    int linha;                        /* Linha no codigo fonte */
    struct NoArvore *filhos[MAX_FILHOS]; /* Ponteiros para filhos */
    int nFilhos;                      /* Numero de filhos */
} NoArvore;

NoArvore* criarNo(const char* tipo, const char* valor, int linha);

void adicionarFilho(NoArvore* pai, NoArvore* filho);

void imprimirArvore(NoArvore* no, int nivel);

#endif
