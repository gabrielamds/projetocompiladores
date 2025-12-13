/*
 * ============================================================================
 * ANALISADOR SEMANTICO - Implementacao
 * ============================================================================
 * 
 * Valor: 2,5 pontos
 * 
 * Este modulo percorre a arvore sintatica gerada pelo parser e realiza
 * as seguintes verificacoes semanticas:
 * 
 * 1. DECLARACAO: Insere variaveis e funcoes na tabela de simbolos
 * 2. USO: Verifica se identificadores foram declarados antes do uso
 * 3. REDECLARACAO: Detecta variaveis/funcoes declaradas mais de uma vez
 * 4. MAIN: Verifica se a funcao main() foi declarada
 * 
 * A analise usa um percurso em profundidade (DFS) na arvore.
 * 
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantico.h"
#include "simbolos.h"
#include "arvore.h"

/* Contador global de erros semanticos */
int errosSemanticos = 0;

/* Escopo atual durante a analise */
static char escopoAtual[50] = "global";

/* Prototipos das funcoes auxiliares */
static void analisarNo(NoArvore *no);
static void analisarDeclaracaoVariavel(NoArvore *no);
static void analisarDeclaracaoFuncao(NoArvore *no);
static void analisarParametros(NoArvore *no);

/*
 * inicializarFuncoesPredefinidas
 * 
 * Insere as funcoes da biblioteca padrao do C- na tabela:
 * - input(): le um inteiro da entrada
 * - output(int x): escreve um inteiro na saida
 */
void inicializarFuncoesPredefinidas(void) {
    /* input: funcao que retorna int, sem parametros */
    inserirSimbolo("input", "int", "global", 0, 1, 0);
    
    /* output: funcao void que recebe um int */
    inserirSimbolo("output", "void", "global", 0, 1, 1);
}

/*
 * analisarSemantica - Ponto de entrada da analise semantica
 * 
 * Esta funcao e chamada apos a analise sintatica ter sucesso.
 * Ela inicializa o ambiente e percorre toda a arvore.
 */
int analisarSemantica(NoArvore *raiz) {
    /* Reseta contadores */
    errosSemanticos = 0;
    strcpy(escopoAtual, "global");
    
    /* Insere funcoes pre-definidas na tabela */
    inicializarFuncoesPredefinidas();
    
    /* Percorre a arvore a partir da raiz */
    if (raiz != NULL) {
        analisarNo(raiz);
    }
    
    /* 
     * Verifica se main() foi declarada
     * Esta e uma regra semantica importante: todo programa C- deve
     * ter uma funcao main como ponto de entrada.
     */
    if (buscarSimbolo("main", "global") == NULL) {
        printf("ERRO SEMANTICO: funcao 'main' nao declarada LINHA: 0\n");
        errosSemanticos++;
    }
    
    return errosSemanticos;
}

/*
 * analisarNo - Analisa um no da arvore
 * 
 * Identifica o tipo do no e chama a funcao de analise apropriada.
 * Para nos que nao precisam de tratamento especial, apenas percorre
 * os filhos recursivamente.
 */
static void analisarNo(NoArvore *no) {
    if (no == NULL) return;
    
    /* ----------------------------------------------------------------
     * DECLARACAO DE VARIAVEL
     * Quando encontra VAR ou ARRAY_VAR, insere na tabela de simbolos
     * ---------------------------------------------------------------- */
    if (strcmp(no->tipo, "VAR") == 0 || strcmp(no->tipo, "ARRAY_VAR") == 0) {
        analisarDeclaracaoVariavel(no);
    }
    
    /* ----------------------------------------------------------------
     * DECLARACAO DE FUNCAO
     * Quando encontra FUN_DEF, insere funcao e muda escopo
     * ---------------------------------------------------------------- */
    else if (strcmp(no->tipo, "FUN_DEF") == 0) {
        analisarDeclaracaoFuncao(no);
    }
    
    /* ----------------------------------------------------------------
     * USO DE VARIAVEL
     * Quando encontra ID, verifica se foi declarada
     * ---------------------------------------------------------------- */
    else if (strcmp(no->tipo, "ID") == 0) {
        Simbolo *s = buscarSimbolo(no->valor, escopoAtual);
        if (s == NULL) {
            printf("ERRO SEMANTICO: variavel '%s' nao declarada LINHA: %d\n", 
                   no->valor, no->linha);
            errosSemanticos++;
        }
    }
    
    /* ----------------------------------------------------------------
     * USO DE ARRAY
     * Similar a variavel, mas tambem analisa o indice
     * ---------------------------------------------------------------- */
    else if (strcmp(no->tipo, "ID_ARRAY") == 0) {
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
    
    /* ----------------------------------------------------------------
     * ATRIBUICAO
     * Verifica o lado esquerdo (variavel) e analisa a expressao
     * ---------------------------------------------------------------- */
    else if (strcmp(no->tipo, "ATRIB") == 0) {
        // Verifica tipos do lado esquerdo e direito
        char tipo_esq[20] = "", tipo_dir[20] = "";
        // Lado esquerdo (variável)
        if (no->nFilhos >= 1 && no->filhos[0] != NULL) {
            analisarNo(no->filhos[0]);
            NoArvore *esq = no->filhos[0];
            if (strcmp(esq->tipo, "ID") == 0 || strcmp(esq->tipo, "ID_ARRAY") == 0) {
                Simbolo *s = buscarSimbolo(esq->valor, escopoAtual);
                if (!s && strcmp(esq->tipo, "ID") == 0) s = buscarSimbolo(esq->valor, "global");
                if (s) {
                    strcpy(tipo_esq, s->tipo);
                    // Se for acesso a array, remove []
                    if (strcmp(esq->tipo, "ID_ARRAY") == 0 && strlen(tipo_esq) > 2) {
                        tipo_esq[strlen(tipo_esq) - 2] = '\0';
                    }
                }
            }
        }
        // Lado direito (expressão)
        if (no->nFilhos >= 2 && no->filhos[1] != NULL) {
            analisarNo(no->filhos[1]);
            NoArvore *dir = no->filhos[1];
            if (strcmp(dir->tipo, "ID") == 0 || strcmp(dir->tipo, "ID_ARRAY") == 0) {
                Simbolo *s = buscarSimbolo(dir->valor, escopoAtual);
                if (!s && strcmp(dir->tipo, "ID") == 0) s = buscarSimbolo(dir->valor, "global");
                if (s) {
                    strcpy(tipo_dir, s->tipo);
                    // Se for acesso a array, remove []
                    if (strcmp(dir->tipo, "ID_ARRAY") == 0 && strlen(tipo_dir) > 2) {
                        tipo_dir[strlen(tipo_dir) - 2] = '\0';
                    }
                }
            } else if (strcmp(dir->tipo, "NUM") == 0) {
                strcpy(tipo_dir, "int");
            }
        }
        // Verifica compatibilidade de tipos (simples: ambos int)
        if (tipo_esq[0] && tipo_dir[0] && strcmp(tipo_esq, tipo_dir) != 0) {
            printf("ERRO SEMANTICO: atribuicao de tipo incompatível ('%s' = '%s') LINHA: %d\n", tipo_esq, tipo_dir, no->linha);
            errosSemanticos++;
        }
    }
    /* Verificação de tipos em operações aritméticas */
    else if (strcmp(no->tipo, "SOMA") == 0 || strcmp(no->tipo, "SUB") == 0 || strcmp(no->tipo, "MULT") == 0 || strcmp(no->tipo, "DIV") == 0) {
        char tipo_esq[20] = "", tipo_dir[20] = "";
        if (no->nFilhos >= 1 && no->filhos[0] != NULL) {
            analisarNo(no->filhos[0]);
            NoArvore *esq = no->filhos[0];
            if (strcmp(esq->tipo, "ID") == 0 || strcmp(esq->tipo, "ID_ARRAY") == 0) {
                Simbolo *s = buscarSimbolo(esq->valor, escopoAtual);
                if (!s && strcmp(esq->tipo, "ID") == 0) s = buscarSimbolo(esq->valor, "global");
                if (s) {
                    strcpy(tipo_esq, s->tipo);
                    // Se for acesso a array, remove []
                    if (strcmp(esq->tipo, "ID_ARRAY") == 0 && strlen(tipo_esq) > 2) {
                        tipo_esq[strlen(tipo_esq) - 2] = '\0';
                    }
                }
            } else if (strcmp(esq->tipo, "NUM") == 0) {
                strcpy(tipo_esq, "int");
            }
        }
        if (no->nFilhos >= 2 && no->filhos[1] != NULL) {
            analisarNo(no->filhos[1]);
            NoArvore *dir = no->filhos[1];
            if (strcmp(dir->tipo, "ID") == 0 || strcmp(dir->tipo, "ID_ARRAY") == 0) {
                Simbolo *s = buscarSimbolo(dir->valor, escopoAtual);
                if (!s && strcmp(dir->tipo, "ID") == 0) s = buscarSimbolo(dir->valor, "global");
                if (s) {
                    strcpy(tipo_dir, s->tipo);
                    // Se for acesso a array, remove []
                    if (strcmp(dir->tipo, "ID_ARRAY") == 0 && strlen(tipo_dir) > 2) {
                        tipo_dir[strlen(tipo_dir) - 2] = '\0';
                    }
                }
            } else if (strcmp(dir->tipo, "NUM") == 0) {
                strcpy(tipo_dir, "int");
            }
        }
        if ((tipo_esq[0] && strcmp(tipo_esq, "int") != 0) || (tipo_dir[0] && strcmp(tipo_dir, "int") != 0)) {
            printf("ERRO SEMANTICO: operacao aritmetica com tipo nao inteiro ('%s' e '%s') LINHA: %d\n", tipo_esq, tipo_dir, no->linha);
            errosSemanticos++;
        }
    }
    
    /* ----------------------------------------------------------------
     * OUTROS NOS
     * Apenas percorre os filhos recursivamente
     * ---------------------------------------------------------------- */
    else {
        for (int i = 0; i < no->nFilhos; i++) {
            analisarNo(no->filhos[i]);
        }
    }
}

/*
 * analisarDeclaracaoVariavel - Processa declaracao de variavel
 * 
 * Extrai tipo e nome, e insere na tabela de simbolos.
 * Se ja existir no mesmo escopo, reporta erro de redeclaracao.
 */
static void analisarDeclaracaoVariavel(NoArvore *no) {
    if (no == NULL) return;
    
    char tipo[20] = "";
    char *nome = no->valor;
    
    /* Extrai o tipo do primeiro filho */
    if (no->nFilhos > 0 && no->filhos[0] != NULL) {
        if (strcmp(no->filhos[0]->tipo, "TIPO") == 0) {
            strcpy(tipo, no->filhos[0]->valor);
        }
    }
    
    /* Se for array, ajusta o tipo */
    if (strcmp(no->tipo, "ARRAY_VAR") == 0) {
        strcat(tipo, "[]");
    }
    
    /* Tenta inserir na tabela */
    if (!inserirSimbolo(nome, tipo, escopoAtual, no->linha, 0, 0)) {
        printf("ERRO SEMANTICO: variavel '%s' ja declarada neste escopo LINHA: %d\n", 
               nome, no->linha);
        errosSemanticos++;
    }
}

/*
 * analisarDeclaracaoFuncao - Processa declaracao de funcao
 * 
 * 1. Insere a funcao na tabela (escopo global)
 * 2. Muda o escopo atual para dentro da funcao
 * 3. Processa os parametros como variaveis locais
 * 4. Analisa o corpo da funcao
 * 5. Restaura o escopo anterior
 */
static void analisarDeclaracaoFuncao(NoArvore *no) {
    if (no == NULL) return;
    
    char tipo[20] = "";
    char *nomeFuncao = no->valor;
    int numParams = 0;
    
    /* Extrai tipo de retorno */
    if (no->nFilhos > 0 && no->filhos[0] != NULL) {
        if (strcmp(no->filhos[0]->tipo, "TIPO") == 0) {
            strcpy(tipo, no->filhos[0]->valor);
        }
    }
    
    /* Conta parametros */
    if (no->nFilhos > 1 && no->filhos[1] != NULL) {
        NoArvore *params = no->filhos[1];
        if (strcmp(params->tipo, "PARAMS") == 0) {
            if (params->valor[0] != '\0' && strcmp(params->valor, "void") == 0) {
                numParams = 0;
            } else if (params->nFilhos > 0) {
                numParams = 1;
            }
        }
    }
    
    /* Insere a funcao na tabela (sempre no escopo global) */
    if (!inserirSimbolo(nomeFuncao, tipo, "global", no->linha, 1, numParams)) {
        printf("ERRO SEMANTICO: funcao '%s' ja declarada LINHA: %d\n", 
               nomeFuncao, no->linha);
        errosSemanticos++;
    }
    
    /* Salva escopo atual e muda para dentro da funcao */
    char escopoAnterior[50];
    strcpy(escopoAnterior, escopoAtual);
    strcpy(escopoAtual, nomeFuncao);
    
    /* Processa parametros */
    if (no->nFilhos > 1 && no->filhos[1] != NULL) {
        analisarParametros(no->filhos[1]);
    }
    
    /* Analisa o corpo da funcao */
    if (no->nFilhos > 2 && no->filhos[2] != NULL) {
        analisarNo(no->filhos[2]);
    }
    
    /* Restaura escopo anterior */
    strcpy(escopoAtual, escopoAnterior);
}

/*
 * analisarParametros - Processa lista de parametros
 * 
 * Cada parametro e inserido como variavel local da funcao.
 */
static void analisarParametros(NoArvore *no) {
    if (no == NULL) return;
    
    /* Se for um parametro, insere na tabela */
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
        
        /* Insere como variavel local */
        if (!inserirSimbolo(nome, tipo, escopoAtual, no->linha, 0, 0)) {
            printf("ERRO SEMANTICO: parametro '%s' ja declarado LINHA: %d\n", 
                   nome, no->linha);
            errosSemanticos++;
        }
    }
    
    /* Percorre filhos recursivamente */
    for (int i = 0; i < no->nFilhos; i++) {
        analisarParametros(no->filhos[i]);
    }
}
