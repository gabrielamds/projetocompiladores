/*
 * ============================================================================
 * GERADOR DE CODIGO INTERMEDIARIO - Implementacao
 * ============================================================================
 * 
 * Implementa a geracao de codigo intermediario em 3-address code.
 * Percorre a arvore sintatica em profundidade e gera instrucoes linearizadas.
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermediario.h"
#include "arvore.h"

/* Array para armazenar as instrucoes geradas */
static Instrucao3Endereco *instrucoes = NULL;
static int totalInstrucoes = 0;
static int capacidadeInstrucoes = 100;

/* Contador de variaveis temporarias */
static int contadorTemp = 0;

/* Funcoes auxiliares */
static void gerarInstrucao(const char *op, const char *op1, const char *op2, const char *res);
static char* gerarTemp(void);
static void analisarNoIntermediario(NoArvore *no, char *resultado);

/*
 * gerarTemp - Gera um novo nome de variavel temporaria
 */
static char* gerarTemp(void) {
    static char temp[20];
    sprintf(temp, "t%d", contadorTemp++);
    return temp;
}

/*
 * gerarInstrucao - Adiciona uma instrucao a lista
 */
static void gerarInstrucao(const char *op, const char *op1, const char *op2, const char *res) {
    if (totalInstrucoes >= capacidadeInstrucoes) {
        capacidadeInstrucoes *= 2;
        instrucoes = (Instrucao3Endereco*) realloc(instrucoes, 
                     sizeof(Instrucao3Endereco) * capacidadeInstrucoes);
    }
    
    instrucoes[totalInstrucoes].numero = totalInstrucoes + 1;
    strncpy(instrucoes[totalInstrucoes].operador, op, sizeof(instrucoes[totalInstrucoes].operador) - 1);
    strncpy(instrucoes[totalInstrucoes].operando1, op1, sizeof(instrucoes[totalInstrucoes].operando1) - 1);
    strncpy(instrucoes[totalInstrucoes].operando2, op2, sizeof(instrucoes[totalInstrucoes].operando2) - 1);
    strncpy(instrucoes[totalInstrucoes].resultado, res, sizeof(instrucoes[totalInstrucoes].resultado) - 1);
    
    totalInstrucoes++;
}

/*
 * analisarNoIntermediario - Percorre a arvore e gera codigo intermediario
 * 
 * A variavel 'resultado' armazena onde o resultado do no deve ser colocado.
 */
static void analisarNoIntermediario(NoArvore *no, char *resultado) {
    if (no == NULL) return;
    
    char temp1[50], temp2[50];
    
    /* EXPRESSOES BINARIAS (SOMA, SUB, MULT, DIV) */
    if (strcmp(no->tipo, "SOMA") == 0 || strcmp(no->tipo, "SUB") == 0 ||
        strcmp(no->tipo, "MULT") == 0 || strcmp(no->tipo, "DIV") == 0) {
        
        /* Gera codigo para operando 1 */
        analisarNoIntermediario(no->filhos[0], temp1);
        
        /* Gera codigo para operando 2 */
        analisarNoIntermediario(no->filhos[1], temp2);
        
        /* Gera a instrucao de operacao */
        char op[5];
        if (strcmp(no->tipo, "SOMA") == 0) strcpy(op, "+");
        else if (strcmp(no->tipo, "SUB") == 0) strcpy(op, "-");
        else if (strcmp(no->tipo, "MULT") == 0) strcpy(op, "*");
        else strcpy(op, "/");
        
        char *temp = gerarTemp();
        gerarInstrucao(op, temp1, temp2, temp);
        strcpy(resultado, temp);
    }
    
    /* ATRIBUICAO */
    else if (strcmp(no->tipo, "ATRIB") == 0) {
        char tempdireito[50];
        
        /* Processa lado direito */
        if (no->filhos[1] != NULL) {
            analisarNoIntermediario(no->filhos[1], tempdireito);
        }
        
        /* Atribui ao lado esquerdo */
        if (no->filhos[0] != NULL && strcmp(no->filhos[0]->tipo, "ID") == 0) {
            gerarInstrucao("=", tempdireito, "", no->filhos[0]->valor);
            strcpy(resultado, no->filhos[0]->valor);
        }
    }
    
    /* NUMERO */
    else if (strcmp(no->tipo, "NUM") == 0) {
        strcpy(resultado, no->valor);
    }
    
    /* IDENTIFICADOR */
    else if (strcmp(no->tipo, "ID") == 0) {
        strcpy(resultado, no->valor);
    }
    
    /* OUTROS NOS - Apenas percorre filhos */
    else {
        for (int i = 0; i < no->nFilhos; i++) {
            analisarNoIntermediario(no->filhos[i], resultado);
        }
    }
}

/*
 * gerarCodigoIntermediario - Ponto de entrada para geracao de codigo
 */
int gerarCodigoIntermediario(NoArvore *raiz) {
    contadorTemp = 0;
    totalInstrucoes = 0;
    
    /* Aloca memoria para instrucoes */
    instrucoes = (Instrucao3Endereco*) malloc(sizeof(Instrucao3Endereco) * capacidadeInstrucoes);
    
    if (raiz == NULL) return 0;
    
    char resultado[50];
    analisarNoIntermediario(raiz, resultado);
    
    return totalInstrucoes;
}

/*
 * imprimirCodigoIntermediario - Imprime o codigo intermediario gerado
 */
void imprimirCodigoIntermediario(void) {
    printf("\n");
    printf("================================================================================\n");
    printf("                        CODIGO INTERMEDIARIO (3-ADDRESS CODE)                   \n");
    printf("================================================================================\n");
    printf("%-8s %-10s %-15s %-15s %-15s\n", 
           "Num", "Op", "Operando1", "Operando2", "Resultado");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < totalInstrucoes; i++) {
        printf("%-8d %-10s %-15s %-15s %-15s\n",
               instrucoes[i].numero,
               instrucoes[i].operador,
               instrucoes[i].operando1,
               instrucoes[i].operando2,
               instrucoes[i].resultado);
    }
    
    printf("================================================================================\n");
}

/*
 * liberarCodigoIntermediario - Libera memoria alocada
 */
void liberarCodigoIntermediario(void) {
    if (instrucoes != NULL) {
        free(instrucoes);
        instrucoes = NULL;
    }
    totalInstrucoes = 0;
    contadorTemp = 0;
}
