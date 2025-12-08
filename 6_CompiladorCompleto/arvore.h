/*
 * ============================================================================
 * ARVORE SINTATICA - Compilador C-
 * ============================================================================
 * 
 * Valor: 1,5 pontos
 * 
 * A arvore sintatica (Abstract Syntax Tree - AST) representa a estrutura
 * hierarquica do programa fonte. Cada no da arvore representa uma
 * construcao sintatica da linguagem.
 * 
 * Estrutura do No:
 * - tipo:    indica o tipo de construcao (ex: "VAR", "IF", "WHILE")
 * - valor:   valor associado (ex: nome do identificador, operador)
 * - linha:   linha do codigo fonte (para mensagens de erro)
 * - filhos:  ponteiros para os nos filhos
 * 
 * ============================================================================
 */

#ifndef ARVORE_H
#define ARVORE_H

/* Numero maximo de filhos por no */
#define MAX_FILHOS 5

/*
 * Estrutura que representa um no da arvore sintatica
 */
typedef struct NoArvore {
    char tipo[30];                    /* Tipo do no (ex: "IF", "WHILE", "VAR") */
    char valor[30];                   /* Valor associado (nome, operador, etc) */
    int linha;                        /* Linha no codigo fonte */
    struct NoArvore *filhos[MAX_FILHOS]; /* Ponteiros para filhos */
    int nFilhos;                      /* Numero de filhos */
} NoArvore;

/*
 * criarNo - Cria um novo no da arvore
 * 
 * Parametros:
 *   tipo  - tipo do no (string identificadora)
 *   valor - valor associado (pode ser NULL)
 *   linha - numero da linha no codigo fonte
 * 
 * Retorna: ponteiro para o novo no
 */
NoArvore* criarNo(const char* tipo, const char* valor, int linha);

/*
 * adicionarFilho - Adiciona um filho a um no
 * 
 * Parametros:
 *   pai   - no pai
 *   filho - no a ser adicionado como filho
 */
void adicionarFilho(NoArvore* pai, NoArvore* filho);

/*
 * imprimirArvore - Imprime a arvore de forma indentada
 * 
 * Parametros:
 *   no    - raiz da subarvore a imprimir
 *   nivel - nivel de indentacao atual
 */
void imprimirArvore(NoArvore* no, int nivel);

#endif
