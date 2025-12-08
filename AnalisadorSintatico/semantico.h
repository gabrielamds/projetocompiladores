/*
 * semantico.h - Analisador Semantico para o Compilador C-
 * 
 * O analisador semantico verifica regras que nao podem ser
 * expressas pela gramatica (analise sintatica), como:
 * 
 * 1. Variaveis e funcoes devem ser declaradas antes do uso
 * 2. Tipos devem ser compativeis em atribuicoes e operacoes
 * 3. Funcoes especiais (main, input, output) sao pre-definidas
 */

#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "arvore.h"

/* Contador de erros semanticos encontrados */
extern int errosSemanticos;

/*
 * analisarSemantica - Funcao principal do analisador semantico
 * 
 * Percorre a arvore sintatica e realiza as verificacoes semanticas.
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
 * - input(): retorna int, nao recebe parametros
 * - output(int x): retorna void, recebe um inteiro
 */
void inicializarFuncoesPredefinidas(void);

#endif
