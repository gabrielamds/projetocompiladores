#ifndef SIMBOLOS_H
#define SIMBOLOS_H

typedef struct Simbolo {
    char nome[50];        // nome do identificador
    char tipo[10];        // tipo (ex: "int", "void")
    char escopo[50];      // escopo do identificador (ex: "global", "funcao1")
    int linha;            // linha da declaração
    struct Simbolo *prox; // próximo simbolo na lista
} Simbolo;

void inserirSimbolo(char *nome, char *tipo, char *escopo, int linha);
Simbolo* buscarSimbolo(char *nome, char *escopo);
void imprimirTabela(void);
void liberarTabela(void);

#endif
