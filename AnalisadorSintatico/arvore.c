#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

NoArvore* criarNo(const char* tipo, const char* valor, int linha) {
    NoArvore* no = (NoArvore*) malloc(sizeof(NoArvore));
    if (!no) {
        fprintf(stderr, "Erro de alocacao de memoria para NoArvore\n");
        exit(1);
    }
    strncpy(no->tipo, tipo, sizeof(no->tipo)-1);
    no->tipo[sizeof(no->tipo)-1] = '\0';

    if (valor) {
        strncpy(no->valor, valor, sizeof(no->valor)-1);
        no->valor[sizeof(no->valor)-1] = '\0';
    } else {
        no->valor[0] = '\0';
    }

    no->linha = linha;
    no->nFilhos = 0;
    for (int i = 0; i < MAX_FILHOS; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

void adicionarFilho(NoArvore* pai, NoArvore* filho) {
    if (!pai || !filho) return;
    if (pai->nFilhos < MAX_FILHOS) {
        pai->filhos[pai->nFilhos++] = filho;
    }
}

void imprimirArvore(NoArvore* no, int nivel) {
    if (!no) return;
    for (int i = 0; i < nivel; i++) printf("  ");
    printf("- %s", no->tipo);
    if (no->valor[0] != '\0')
        printf(" (%s)", no->valor);
    printf(" [linha %d]\n", no->linha);
    for (int i = 0; i < no->nFilhos; i++)
        imprimirArvore(no->filhos[i], nivel+1);
}
