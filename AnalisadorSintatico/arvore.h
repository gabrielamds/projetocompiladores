#ifndef ARVORE_H
#define ARVORE_H

#define MAX_FILHOS 5

typedef struct NoArvore {
    char tipo[30];
    char valor[30];
    int linha;
    struct NoArvore *filhos[MAX_FILHOS];
    int nFilhos;
} NoArvore;

NoArvore* criarNo(const char* tipo, const char* valor, int linha);
void adicionarFilho(NoArvore* pai, NoArvore* filho);
void imprimirArvore(NoArvore* no, int nivel);

#endif
