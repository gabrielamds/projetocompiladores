#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "arvore.h"

/* Contador de erros semanticos encontrados */
extern int errosSemanticos;

int analisarSemantica(NoArvore *raiz);

void inicializarFuncoesPredefinidas(void);

#endif
