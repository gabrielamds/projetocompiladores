%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "simbolos.h"
#include "semantico.h"
#include "intermediario.h"

/* Funcoes externas do scanner */
extern int yylex();
extern int linha;

/* Raiz da arvore sintatica - sera preenchida durante o parsing */
NoArvore* raiz = NULL;

/* Flag de debug do Bison */
int yydebug;

/* Variavel externa do Flex com o token atual */
extern char *yytext;

void yyerror(char *s) {
    if (yytext && yytext[0] != '\0') {
        fprintf(stderr, "ERRO SINTATICO: '%s' (esperado: %s) LINHA: %d\n", yytext, s, linha);
    } else {
        fprintf(stderr, "ERRO SINTATICO: %s LINHA: %d\n", s, linha);
    }
}
%}

%union {
    void* no;   /* Ponteiro para NoArvore */
    char* str;  /* String (identificadores) */
    int num;    /* Numero inteiro */
}

/* --- Tokens (terminais) vindos do scanner --- */
%token <str> ID
%token <num> NUM
%token IF ELSE WHILE RETURN INT VOID
%token MAIS MENOS VEZES DIVIDIDO
%token MENOR MAIOR MENORIGUAL MAIORIGUAL IGUAL DIFERENTE
%token ATRIBUICAO
%token ABREPARENTESES FECHAPARENTESES
%token ABRECOLCHETE FECHACOLCHETE
%token ABRECHAVE FECHACHAVE
%token PONTOVIRGULA VIRGULA

/* --- Tipos dos nao-terminais --- */
%type <no> programa declaracao_lista declaracao var_declaracao tipo_especificador
%type <no> fun_declaracao parametros parametro_lista parametro corpo_funcao
%type <no> comando_lista comando expressao_comando selecao_comando bloco_comando
%type <no> iteracao_comando retorno_comando expressao simples_expressao relacional
%type <no> soma_expressao termo fator var args_funcao

%%

/* Programa e uma lista de declaracoes */
programa:
    declaracao_lista   { raiz = $1; }
;

/* Lista de declaracoes (pelo menos uma) */
declaracao_lista:
    declaracao_lista declaracao {
        $$ = criarNo("DECL_LIST", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $2);
    }
  | declaracao { $$ = $1; }
;

/* Uma declaracao pode ser de variavel ou de funcao */
declaracao:
    var_declaracao {
        $$ = criarNo("DECL", NULL, linha);
        adicionarFilho($$, $1);
    }
  | fun_declaracao {
        $$ = criarNo("DECL", NULL, linha);
        adicionarFilho($$, $1);
    }
;

/* Declaracao de variavel simples ou array */
var_declaracao:
    tipo_especificador ID PONTOVIRGULA {
        $$ = criarNo("VAR", $2, linha);
        adicionarFilho($$, $1);
    }
  | tipo_especificador ID ABRECOLCHETE NUM FECHACOLCHETE PONTOVIRGULA {
        char numstr[16]; 
        sprintf(numstr, "%d", $4);
        $$ = criarNo("ARRAY_VAR", $2, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, criarNo("TAM", numstr, linha));
    }
;

/* Especificador de tipo: int ou void */
tipo_especificador:
    INT  { $$ = criarNo("TIPO", "int", linha); }
  | VOID { $$ = criarNo("TIPO", "void", linha); }
;

/* Declaracao de funcao */
fun_declaracao:
    tipo_especificador ID ABREPARENTESES parametros FECHAPARENTESES corpo_funcao {
        $$ = criarNo("FUN_DEF", $2, linha);
        adicionarFilho($$, $1);   /* tipo de retorno */
        adicionarFilho($$, $4);   /* parametros */
        adicionarFilho($$, $6);   /* corpo */
    }
;

/* Parametros: void ou lista de parametros */
parametros:
    VOID { $$ = criarNo("PARAMS", "void", linha); }
  | parametro_lista {
        $$ = criarNo("PARAMS", NULL, linha);
        adicionarFilho($$, $1);
    }
;

/* Lista de parametros separados por virgula */
parametro_lista:
    parametro_lista VIRGULA parametro {
        $$ = criarNo("PARAM_LIST", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | parametro { $$ = $1; }
;

/* Parametro simples ou array */
parametro:
    tipo_especificador ID {
        $$ = criarNo("PARAM", $2, linha);
        adicionarFilho($$, $1);
    }
  | tipo_especificador ID ABRECOLCHETE FECHACOLCHETE {
        $$ = criarNo("PARAM_ARRAY", $2, linha);
        adicionarFilho($$, $1);
    }
;

/* Corpo da funcao: bloco com declaracoes e comandos */
corpo_funcao:
    ABRECHAVE declaracao_lista comando_lista FECHACHAVE {
        $$ = criarNo("CORPO", NULL, linha);
        adicionarFilho($$, $2);
        adicionarFilho($$, $3);
    }
  | ABRECHAVE comando_lista FECHACHAVE {
        $$ = criarNo("CORPO", NULL, linha);
        adicionarFilho($$, $2);
    }
;

/* Lista de comandos (pode ser vazia) */
comando_lista:
    comando_lista comando {
        $$ = criarNo("CMD_LIST", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $2);
    }
  | comando { $$ = $1; }
  | /* vazio */ {
        $$ = criarNo("CMD_LIST_VAZIA", NULL, linha);
    }
;

/* Tipos de comandos */
comando:
    expressao_comando {
        $$ = criarNo("CMD", NULL, linha);
        adicionarFilho($$, $1);
    }
  | selecao_comando {
        $$ = criarNo("CMD", NULL, linha);
        adicionarFilho($$, $1);
    }
  | iteracao_comando {
        $$ = criarNo("CMD", NULL, linha);
        adicionarFilho($$, $1);
    }
  | retorno_comando {
        $$ = criarNo("CMD", NULL, linha);
        adicionarFilho($$, $1);
    }
  | bloco_comando {
        $$ = $1;
    }
;

/* Bloco de comandos com chaves */
bloco_comando:
    ABRECHAVE declaracao_lista comando_lista FECHACHAVE {
        $$ = criarNo("BLOCO", NULL, linha);
        adicionarFilho($$, $2);
        adicionarFilho($$, $3);
    }
  | ABRECHAVE comando_lista FECHACHAVE {
        $$ = criarNo("BLOCO", NULL, linha);
        adicionarFilho($$, $2);
    }
  | ABRECHAVE declaracao_lista FECHACHAVE {
        $$ = criarNo("BLOCO", NULL, linha);
        adicionarFilho($$, $2);
    }
  | ABRECHAVE FECHACHAVE {
        $$ = criarNo("BLOCO_VAZIO", NULL, linha);
    }
;

/* Comando de expressao (expressao seguida de ;) */
expressao_comando:
    expressao PONTOVIRGULA {
        $$ = criarNo("EXP_CMD", NULL, linha);
        adicionarFilho($$, $1);
    }
  | PONTOVIRGULA {
        $$ = criarNo("EXP_CMD_VAZIO", NULL, linha);
    }
;

/* Comando de selecao: if e if-else */
selecao_comando:
    IF ABREPARENTESES expressao FECHAPARENTESES comando ELSE comando {
        $$ = criarNo("IFELSE", NULL, linha);
        adicionarFilho($$, $3);  /* condicao */
        adicionarFilho($$, $5);  /* bloco then */
        adicionarFilho($$, $7);  /* bloco else */
    }
  | IF ABREPARENTESES expressao FECHAPARENTESES comando {
        $$ = criarNo("IF", NULL, linha);
        adicionarFilho($$, $3);  /* condicao */
        adicionarFilho($$, $5);  /* bloco then */
    }
;

/* Comando de iteracao: while */
iteracao_comando:
    WHILE ABREPARENTESES expressao FECHAPARENTESES comando {
        $$ = criarNo("WHILE", NULL, linha);
        adicionarFilho($$, $3);  /* condicao */
        adicionarFilho($$, $5);  /* corpo do loop */
    }
;

/* Comando de retorno */
retorno_comando:
    RETURN PONTOVIRGULA {
        $$ = criarNo("RETURN", NULL, linha);
    }
  | RETURN expressao PONTOVIRGULA {
        $$ = criarNo("RETURN", NULL, linha);
        adicionarFilho($$, $2);
    }
;

/* Expressao: atribuicao ou expressao simples */
expressao:
    var ATRIBUICAO expressao {
        $$ = criarNo("ATRIB", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | simples_expressao { $$ = $1; }
;

/* Expressao simples: comparacao ou soma */
simples_expressao:
    soma_expressao relacional soma_expressao {
        $$ = criarNo("REL", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $2);
        adicionarFilho($$, $3);
    }
  | soma_expressao { $$ = $1; }
;

/* Operadores relacionais */
relacional:
      MENOR      { $$ = criarNo("OP", "<", linha); }
    | MAIOR      { $$ = criarNo("OP", ">", linha); }
    | MENORIGUAL { $$ = criarNo("OP", "<=", linha); }
    | MAIORIGUAL { $$ = criarNo("OP", ">=", linha); }
    | IGUAL      { $$ = criarNo("OP", "==", linha); }
    | DIFERENTE  { $$ = criarNo("OP", "!=", linha); }
;

/* Soma e subtracao */
soma_expressao:
    soma_expressao MAIS termo {
        $$ = criarNo("SOMA", "+", linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | soma_expressao MENOS termo {
        $$ = criarNo("SUB", "-", linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | termo { $$ = $1; }
;

/* Multiplicacao e divisao */
termo:
    termo VEZES fator {
        $$ = criarNo("MULT", "*", linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | termo DIVIDIDO fator {
        $$ = criarNo("DIV", "/", linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | fator { $$ = $1; }
;

/* Fator: parenteses, variavel, numero ou chamada de funcao */
fator:
    ABREPARENTESES expressao FECHAPARENTESES { $$ = $2; }
  | var { $$ = $1; }
  | NUM {
        char numstr[16];
        sprintf(numstr, "%d", $1);
        $$ = criarNo("NUM", numstr, linha);
    }
  | ID ABREPARENTESES FECHAPARENTESES {
        $$ = criarNo("CHAMADA_FUNCAO", $1, linha);
    }
  | ID ABREPARENTESES args_funcao FECHAPARENTESES {
        $$ = criarNo("CHAMADA_FUNCAO", $1, linha);
        adicionarFilho($$, $3);
    }
;

/* Argumentos de funcao */
args_funcao:
    expressao {
        $$ = criarNo("ARGS", NULL, linha);
        adicionarFilho($$, $1);
    }
  | args_funcao VIRGULA expressao {
        adicionarFilho($1, $3);
        $$ = $1;
    }
;

/* Variavel simples ou acesso a array */
var:
      ID {
        $$ = criarNo("ID", $1, linha);
    }
    | ID ABRECOLCHETE expressao FECHACOLCHETE {
        $$ = criarNo("ID_ARRAY", $1, linha);
        adicionarFilho($$, $3);
    }
;

%%

int main(int argc, char *argv[]) {
    if (argc == 2) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            printf("Erro ao abrir arquivo %s\n", argv[1]);
            return 1;
        }
        extern FILE *yyin;
        yyin = fp;

        /* Desliga debug para saida limpa */
        yydebug = 0;

        printf("=== COMPILADOR C- ===\n\n");
        printf("Analisando arquivo: %s\n\n", argv[1]);

        /* FASE 1: Analise Sintatica */
        printf("--- FASE 1: ANALISE SINTATICA ---\n");
        int resultado = yyparse();
        
        if (resultado == 0) {
            printf("Analise sintatica concluida com sucesso!\n\n");
            
            /* FASE 2: Analise Semantica */
            printf("--- FASE 2: ANALISE SEMANTICA ---\n");
            int erros = analisarSemantica(raiz);
            
            if (erros == 0) {
                printf("Analise semantica concluida sem erros!\n");
            } else {
                printf("\nTotal de erros semanticos: %d\n", erros);
            }
            
            /* Imprime Arvore Sintatica */
            printf("\n--- ARVORE SINTATICA ---\n");
            imprimirArvore(raiz, 0);
            
            /* FASE 3: Geracao de Codigo Intermediario */
            printf("\n--- FASE 3: GERACAO DE CODIGO INTERMEDIARIO ---\n");
            int numInstrucoes = gerarCodigoIntermediario(raiz);
            printf("Codigo intermediario gerado: %d instrucoes\n", numInstrucoes);
            
            /* Imprime Codigo Intermediario */
            imprimirCodigoIntermediario();
            
            /* Imprime Tabela de Simbolos */
            imprimirTabelaSimbolos();
            
            /* Libera memoria */
            liberarCodigoIntermediario();
            liberarTabelaSimbolos();
        } else {
            printf("\nCompilacao abortada devido a erros sintaticos.\n");
        }
        
        fclose(fp);
    } else {
        printf("Uso: %s <arquivo.cm>\n", argv[0]);
        return 1;
    }
    return 0;
}
