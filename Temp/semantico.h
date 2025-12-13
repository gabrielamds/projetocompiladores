/*
 * ============================================================================
 * ANALISADOR SEMANTICO - Compilador C-
 * ============================================================================
 * 
 * Valor: 2,5 pontos
 * 
 * O analisador semantico verifica regras que NAO podem ser expressas
 * pela gramatica livre de contexto, como:
 * 
 * 1. Variaveis e funcoes devem ser declaradas antes do uso
 * 2. Nao pode haver redeclaracao no mesmo escopo
 * 3. A funcao main() deve existir
 * 4. Funcoes input() e output() sao pre-definidas
 * 
 * O analisador percorre a arvore sintatica (AST) e usa a tabela de
 * simbolos para verificar as restricoes semanticas.
 * 
 * ============================================================================
 */

#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "arvore.h"

/* Contador de erros semanticos encontrados */
extern int errosSemanticos;

/*
 * analisarSemantica - Funcao principal do analisador semantico
 * 
 * Percorre a arvore sintatica e verifica as regras semanticas.
 * Preenche a tabela de simbolos durante a analise.
 * 
 * Parametros:
 *   raiz - ponteiro para a raiz da arvore sintatica
 * 
 * Retorna: numero de erros semanticos encontrados
 */
int analisarSemantica(NoArvore *raiz);

/*
 * inicializarFuncoesPredefinidas - Insere input() e output() na tabela
 * 
 * Conforme especificacao, estas funcoes sao pre-definidas:
 * - input(): retorna int, sem parametros
 * - output(int x): retorna void, recebe um inteiro
 */
void inicializarFuncoesPredefinidas(void);

#endif
