/*
 * simbolos.h - Tabela de Simbolos para o Compilador C-
 * 
 * A tabela de simbolos armazena informacoes sobre identificadores
 * (variaveis e funcoes) encontrados durante a compilacao.
 * 
 * Campos obrigatorios conforme especificacao:
 * - nome: nome do identificador
 * - tipo: "int" ou "void"
 * - escopo: "global" ou nome da funcao onde foi declarado
 * - linha: numero da linha da declaracao
 */

#ifndef SIMBOLOS_H
#define SIMBOLOS_H

/* Estrutura que representa um simbolo na tabela */
typedef struct Simbolo {
    char nome[50];        /* nome do identificador */
    char tipo[10];        /* tipo: "int", "void", "int[]" */
    char escopo[50];      /* escopo: "global" ou nome da funcao */
    int linha;            /* linha onde foi declarado */
    int ehFuncao;         /* 1 se for funcao, 0 se for variavel */
    int numParametros;    /* quantidade de parametros (se for funcao) */
    struct Simbolo *prox; /* ponteiro para proximo simbolo (lista ligada) */
} Simbolo;

/* Insere um novo simbolo na tabela
 * Retorna 1 se inseriu com sucesso, 0 se ja existe no mesmo escopo */
int inserirSimbolo(const char *nome, const char *tipo, const char *escopo, 
                   int linha, int ehFuncao, int numParams);

/* Busca um simbolo pelo nome e escopo
 * Primeiro busca no escopo local, depois no global
 * Retorna ponteiro para o simbolo ou NULL se nao encontrado */
Simbolo* buscarSimbolo(const char *nome, const char *escopo);

/* Busca um simbolo apenas no escopo especificado (sem buscar no global) */
Simbolo* buscarSimboloNoEscopo(const char *nome, const char *escopo);

/* Imprime a tabela de simbolos completa */
void imprimirTabelaSimbolos(void);

/* Libera toda a memoria alocada pela tabela */
void liberarTabelaSimbolos(void);

#endif
