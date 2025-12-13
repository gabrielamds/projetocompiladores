/*
 * ============================================================================
 * GERADOR DE CODIGO INTERMEDIARIO - Compilador C-
 * ============================================================================
 * 
 * Este modulo implementa a geracao de codigo intermediario em formato
 * de 3-address code (codigo de tres enderecos).
 * 
 * Formato: resultado = operando1 operador operando2
 * 
 * Exemplo:
 *   t1 = a + b
 *   t2 = t1 * c
 *   x = t2
 * 
 * ============================================================================
 */

#ifndef INTERMEDIARIO_H
#define INTERMEDIARIO_H

#include "arvore.h"

/*
 * Estrutura para representar uma instrucao de 3 enderecos
 */
typedef struct {
    int numero;              /* Numero sequencial da instrucao */
    char operador[10];       /* Operador: +, -, *, /, =, <, >, etc */
    char operando1[50];      /* Primeiro operando */
    char operando2[50];      /* Segundo operando (opcional) */
    char resultado[50];      /* Variavel resultado/temporaria */
} Instrucao3Endereco;

/*
 * gerarCodigoIntermediario - Gera codigo de 3 enderecos a partir da arvore
 * 
 * Parametros:
 *   raiz - raiz da arvore sintatica
 * 
 * Retorna: numero de instrucoes geradas
 */
int gerarCodigoIntermediario(NoArvore *raiz);

/*
 * imprimirCodigoIntermediario - Imprime o codigo intermediario gerado
 */
void imprimirCodigoIntermediario(void);

/*
 * liberarCodigoIntermediario - Libera memoria alocada
 */
void liberarCodigoIntermediario(void);

#endif
