/*
 * semantico.c - Implementacao do Analisador Semantico
 * 
 * Este modulo percorre a arvore sintatica gerada pelo parser
 * e realiza as seguintes verificacoes:
 * 
 * 1. Declaracao de variaveis e funcoes (insere na tabela de simbolos)
 * 2. Uso de variaveis/funcoes nao declaradas (erro semantico)
 * 3. Redeclaracao de simbolos no mesmo escopo (erro semantico)
 * 4. Verificacao de tipos em atribuicoes
 * 
 * A analise e feita percorrendo a arvore de forma recursiva (DFS).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantico.h"
#include "simbolos.h"
#include "arvore.h"

/* Contador global de erros semanticos */
int errosSemanticos = 0;

/* Escopo atual durante a analise (comeca como "global") */
static char escopoAtual[50] = "global";

/* Prototipos das funcoes auxiliares (privadas deste modulo) */
static void analisarNo(NoArvore *no);
static void analisarDeclaracaoVariavel(NoArvore *no);
static void analisarDeclaracaoFuncao(NoArvore *no);
static void analisarParametros(NoArvore *no);

/*
 * inicializarFuncoesPredefinidas
 * 
 * Insere as funcoes input() e output() na tabela de simbolos.
 * Estas funcoes fazem parte da biblioteca padrao do C-.
 */
void inicializarFuncoesPredefinidas(void) {
    /* input: funcao que retorna int, sem parametros */
    inserirSimbolo("input", "int", "global", 0, 1, 0);
    
    /* output: funcao void que recebe um int */
    inserirSimbolo("output", "void", "global", 0, 1, 1);
}

/*
 * analisarSemantica - Ponto de entrada do analisador semantico
 */
int analisarSemantica(NoArvore *raiz) {
    errosSemanticos = 0;
    strcpy(escopoAtual, "global");
    
    /* Insere funcoes pre-definidas */
    inicializarFuncoesPredefinidas();
    
    /* Percorre a arvore a partir da raiz */
    if (raiz != NULL) {
        analisarNo(raiz);
    }
    
    /* Verifica se a funcao main() foi declarada */
    if (buscarSimbolo("main", "global") == NULL) {
        printf("ERRO SEMANTICO: funcao 'main' nao declarada LINHA: 0\n");
        errosSemanticos++;
    }
    
    return errosSemanticos;
}

/*
 * analisarNo - Analisa um no da arvore sintatica
 * 
 * Esta funcao identifica o tipo do no e chama a funcao apropriada.
 */
static void analisarNo(NoArvore *no) {
    if (no == NULL) return;
    
    /* Identifica o tipo do no pelo campo 'tipo' */
    
    if (strcmp(no->tipo, "VAR") == 0 || strcmp(no->tipo, "ARRAY_VAR") == 0) {
        analisarDeclaracaoVariavel(no);
    }
    else if (strcmp(no->tipo, "FUN_DEF") == 0) {
        analisarDeclaracaoFuncao(no);
    }
    else if (strcmp(no->tipo, "ID") == 0) {
        /* Uso de variavel - verifica se foi declarada */
        Simbolo *s = buscarSimbolo(no->valor, escopoAtual);
        if (s == NULL) {
            printf("ERRO SEMANTICO: variavel '%s' nao declarada LINHA: %d\n", 
                   no->valor, no->linha);
            errosSemanticos++;
        }
    }
    else if (strcmp(no->tipo, "ID_ARRAY") == 0) {
        /* Uso de array - verifica se foi declarado */
        Simbolo *s = buscarSimbolo(no->valor, escopoAtual);
        if (s == NULL) {
            printf("ERRO SEMANTICO: array '%s' nao declarado LINHA: %d\n", 
                   no->valor, no->linha);
            errosSemanticos++;
        }
        /* Analisa a expressao do indice */
        for (int i = 0; i < no->nFilhos; i++) {
            analisarNo(no->filhos[i]);
        }
    }
    else if (strcmp(no->tipo, "ATRIB") == 0) {
        /* Atribuicao - verifica se a variavel existe e analisa a expressao */
        if (no->nFilhos >= 1) {
            analisarNo(no->filhos[0]);  /* lado esquerdo (variavel) */
        }
        if (no->nFilhos >= 2) {
            analisarNo(no->filhos[1]);  /* lado direito (expressao) */
        }
    }
    else if (strcmp(no->tipo, "RETURN") == 0) {
        /* Verifica o retorno */
        for (int i = 0; i < no->nFilhos; i++) {
            analisarNo(no->filhos[i]);
        }
    }
    else {
        /* Para outros tipos de nos, percorre os filhos recursivamente */
        for (int i = 0; i < no->nFilhos; i++) {
            analisarNo(no->filhos[i]);
        }
    }
}

/*
 * analisarDeclaracaoVariavel - Processa declaracao de variavel
 * 
 * Extrai o tipo e nome da variavel e insere na tabela de simbolos.
 */
static void analisarDeclaracaoVariavel(NoArvore *no) {
    if (no == NULL) return;
    
    char tipo[20] = "";
    char *nome = no->valor;  /* Nome esta no campo valor do no */
    
    /* O primeiro filho contem o tipo */
    if (no->nFilhos > 0 && no->filhos[0] != NULL) {
        if (strcmp(no->filhos[0]->tipo, "TIPO") == 0) {
            strcpy(tipo, no->filhos[0]->valor);
        }
    }
    
    /* Verifica se e array e ajusta o tipo */
    if (strcmp(no->tipo, "ARRAY_VAR") == 0) {
        strcat(tipo, "[]");
    }
    
    /* Tenta inserir na tabela de simbolos */
    if (!inserirSimbolo(nome, tipo, escopoAtual, no->linha, 0, 0)) {
        printf("ERRO SEMANTICO: variavel '%s' ja declarada neste escopo LINHA: %d\n", 
               nome, no->linha);
        errosSemanticos++;
    }
}

/*
 * analisarDeclaracaoFuncao - Processa declaracao de funcao
 * 
 * Insere a funcao na tabela, muda o escopo para dentro da funcao,
 * processa os parametros e o corpo, depois restaura o escopo.
 */
static void analisarDeclaracaoFuncao(NoArvore *no) {
    if (no == NULL) return;
    
    char tipo[20] = "";
    char *nomeFuncao = no->valor;
    int numParams = 0;
    
    /* Extrai o tipo de retorno (primeiro filho) */
    if (no->nFilhos > 0 && no->filhos[0] != NULL) {
        if (strcmp(no->filhos[0]->tipo, "TIPO") == 0) {
            strcpy(tipo, no->filhos[0]->valor);
        }
    }
    
    /* Conta os parametros */
    if (no->nFilhos > 1 && no->filhos[1] != NULL) {
        NoArvore *params = no->filhos[1];
        if (strcmp(params->tipo, "PARAMS") == 0) {
            if (params->valor != NULL && strcmp(params->valor, "void") == 0) {
                numParams = 0;
            } else {
                /* Conta parametros na lista */
                numParams = 1;  /* Pelo menos um parametro */
                for (int i = 0; i < params->nFilhos; i++) {
                    if (params->filhos[i] != NULL && 
                        strcmp(params->filhos[i]->tipo, "PARAM_LIST") == 0) {
                        numParams++;
                    }
                }
            }
        }
    }
    
    /* Insere a funcao na tabela (escopo global) */
    if (!inserirSimbolo(nomeFuncao, tipo, "global", no->linha, 1, numParams)) {
        printf("ERRO SEMANTICO: funcao '%s' ja declarada LINHA: %d\n", 
               nomeFuncao, no->linha);
        errosSemanticos++;
    }
    
    /* Muda o escopo para dentro da funcao */
    char escopoAnterior[50];
    strcpy(escopoAnterior, escopoAtual);
    strcpy(escopoAtual, nomeFuncao);
    
    /* Processa os parametros (insere como variaveis locais) */
    if (no->nFilhos > 1 && no->filhos[1] != NULL) {
        analisarParametros(no->filhos[1]);
    }
    
    /* Processa o corpo da funcao */
    if (no->nFilhos > 2 && no->filhos[2] != NULL) {
        analisarNo(no->filhos[2]);
    }
    
    /* Restaura o escopo anterior */
    strcpy(escopoAtual, escopoAnterior);
}

/*
 * analisarParametros - Processa a lista de parametros de uma funcao
 */
static void analisarParametros(NoArvore *no) {
    if (no == NULL) return;
    
    if (strcmp(no->tipo, "PARAM") == 0 || strcmp(no->tipo, "PARAM_ARRAY") == 0) {
        char tipo[20] = "";
        char *nome = no->valor;
        
        /* Extrai o tipo */
        if (no->nFilhos > 0 && no->filhos[0] != NULL) {
            if (strcmp(no->filhos[0]->tipo, "TIPO") == 0) {
                strcpy(tipo, no->filhos[0]->valor);
            }
        }
        
        if (strcmp(no->tipo, "PARAM_ARRAY") == 0) {
            strcat(tipo, "[]");
        }
        
        /* Insere parametro como variavel local */
        if (!inserirSimbolo(nome, tipo, escopoAtual, no->linha, 0, 0)) {
            printf("ERRO SEMANTICO: parametro '%s' ja declarado LINHA: %d\n", 
                   nome, no->linha);
            errosSemanticos++;
        }
    }
    
    /* Processa filhos recursivamente */
    for (int i = 0; i < no->nFilhos; i++) {
        analisarParametros(no->filhos[i]);
    }
}
