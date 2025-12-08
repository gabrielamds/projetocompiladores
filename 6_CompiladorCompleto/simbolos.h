/*
 * ============================================================================
 * TABELA DE SIMBOLOS - Compilador C-
 * ============================================================================
 * 
 * Valor: 1,5 pontos
 * 
 * A tabela de simbolos armazena informacoes sobre todos os identificadores
 * (variaveis e funcoes) encontrados durante a compilacao.
 * 
 * Campos obrigatorios (conforme especificacao):
 * - nome:   nome do identificador
 * - tipo:   tipo do identificador ("int", "void", "int[]")
 * - escopo: escopo onde foi declarado ("global" ou nome da funcao)
 * - linha:  numero da linha da declaracao
 * 
 * Estrutura de dados: Lista Ligada Simples
 * - Insercao: O(1) - no inicio da lista
 * - Busca: O(n) - percorre a lista
 * 
 * ============================================================================
 */

#ifndef SIMBOLOS_H
#define SIMBOLOS_H

/*
 * Estrutura que representa um simbolo na tabela
 */
typedef struct Simbolo {
    char nome[50];        /* Nome do identificador */
    char tipo[10];        /* Tipo: "int", "void", "int[]" */
    char escopo[50];      /* Escopo: "global" ou nome da funcao */
    int linha;            /* Linha onde foi declarado */
    int ehFuncao;         /* 1 = funcao, 0 = variavel */
    int numParametros;    /* Numero de parametros (se for funcao) */
    struct Simbolo *prox; /* Ponteiro para proximo simbolo */
} Simbolo;

/*
 * inserirSimbolo - Insere um novo simbolo na tabela
 * 
 * Parametros:
 *   nome     - nome do identificador
 *   tipo     - tipo do identificador
 *   escopo   - escopo onde foi declarado
 *   linha    - numero da linha
 *   ehFuncao - 1 se for funcao, 0 se for variavel
 *   numParams- numero de parametros (para funcoes)
 * 
 * Retorna: 1 se inseriu com sucesso, 0 se ja existe no escopo
 */
int inserirSimbolo(const char *nome, const char *tipo, const char *escopo, 
                   int linha, int ehFuncao, int numParams);

/*
 * buscarSimbolo - Busca simbolo no escopo local e depois no global
 * 
 * Implementa a regra de escopo: variaveis locais tem precedencia
 * sobre variaveis globais com o mesmo nome.
 * 
 * Retorna: ponteiro para o simbolo ou NULL se nao encontrado
 */
Simbolo* buscarSimbolo(const char *nome, const char *escopo);

/*
 * buscarSimboloNoEscopo - Busca apenas no escopo especificado
 */
Simbolo* buscarSimboloNoEscopo(const char *nome, const char *escopo);

/*
 * imprimirTabelaSimbolos - Imprime a tabela completa
 */
void imprimirTabelaSimbolos(void);

/*
 * liberarTabelaSimbolos - Libera memoria alocada
 */
void liberarTabelaSimbolos(void);

#endif
