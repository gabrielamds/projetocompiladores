#ifndef INTERMEDIARIO_H
#define INTERMEDIARIO_H
#include "arvore.h"

typedef struct {
    int numero;              /* Numero sequencial da instrucao */
    char operador[10];       /* Operador: +, -, *, /, =, <, >, etc */
    char operando1[50];      /* Primeiro operando */
    char operando2[50];      /* Segundo operando (opcional) */
    char resultado[50];      /* Variavel resultado/temporaria */
} Instrucao3Endereco;

int gerarCodigoIntermediario(NoArvore *raiz);

void imprimirCodigoIntermediario(void);

void liberarCodigoIntermediario(void);

#endif
