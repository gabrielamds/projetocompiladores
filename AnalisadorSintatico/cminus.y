%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

extern int yylex();
extern int linha;

/* raiz da árvore sintática */
NoArvore* raiz = NULL;

/* declara yydebug para poder ligar o modo de depuração */
int yydebug;

void yyerror(char *s) {
    fprintf(stderr, "ERRO SINTATICO: %s LINHA: %d\n", s, linha);
}
%}

%union {
    void* no;   /* ponteiro genérico para NoArvore */
    char* str;
    int num;
}

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

%type <no> programa declaracao_lista declaracao var_declaracao tipo_especificador
%type <no> fun_declaracao parametros parametro_lista parametro corpo_funcao
%type <no> comando_lista comando expressao_comando selecao_comando
%type <no> iteracao_comando retorno_comando expressao simples_expressao relacional
%type <no> soma_expressao termo fator var

%%

programa:
    declaracao_lista   { raiz = $1; }
;

/* lista de declarações: NÃO vazia, como no seu original */
declaracao_lista:
    declaracao_lista declaracao {
        $$ = criarNo("DECL_LIST", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $2);
    }
  | declaracao { $$ = $1; }
;

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

var_declaracao:
    tipo_especificador ID PONTOVIRGULA {
        $$ = criarNo("VAR", $2, linha);
        adicionarFilho($$, $1);
    }
  | tipo_especificador ID ABRECOLCHETE NUM FECHACOLCHETE PONTOVIRGULA {
        char numstr[16]; sprintf(numstr, "%d", $4);
        $$ = criarNo("ARRAY_VAR", $2, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, criarNo("TAM", numstr, linha));
    }
;

tipo_especificador:
    INT  { $$ = criarNo("TIPO", "int", linha); }
  | VOID { $$ = criarNo("TIPO", "void", linha); }
;

fun_declaracao:
    tipo_especificador ID ABREPARENTESES parametros FECHAPARENTESES corpo_funcao {
        $$ = criarNo("FUN_DEF", $2, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $4);
        adicionarFilho($$, $6);
    }
;

parametros:
    VOID { $$ = criarNo("PARAMS", "void", linha); }
  | parametro_lista {
        $$ = criarNo("PARAMS", NULL, linha);
        adicionarFilho($$, $1);
    }
;

parametro_lista:
    parametro_lista VIRGULA parametro {
        $$ = criarNo("PARAM_LIST", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | parametro { $$ = $1; }
;

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

/* corpo_funcao agora aceita:
   - { declaracao_lista comando_lista }
   - { comando_lista }   (função sem declarações internas) */
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

/* comando_lista pode ser vazia */
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
;

expressao_comando:
    expressao PONTOVIRGULA {
        $$ = criarNo("EXP_CMD", NULL, linha);
        adicionarFilho($$, $1);
    }
  | PONTOVIRGULA {
        $$ = criarNo("EXP_CMD_VAZIO", NULL, linha);
    }
;

selecao_comando:
    IF ABREPARENTESES expressao FECHAPARENTESES comando ELSE comando {
        $$ = criarNo("IFELSE", NULL, linha);
        adicionarFilho($$, $3);
        adicionarFilho($$, $5);
        adicionarFilho($$, $7);
    }
  | IF ABREPARENTESES expressao FECHAPARENTESES comando {
        $$ = criarNo("IF", NULL, linha);
        adicionarFilho($$, $3);
        adicionarFilho($$, $5);
    }
;

iteracao_comando:
    WHILE ABREPARENTESES expressao FECHAPARENTESES comando {
        $$ = criarNo("WHILE", NULL, linha);
        adicionarFilho($$, $3);
        adicionarFilho($$, $5);
    }
;

retorno_comando:
    RETURN PONTOVIRGULA {
        $$ = criarNo("RETURN", NULL, linha);
    }
  | RETURN expressao PONTOVIRGULA {
        $$ = criarNo("RETURN", NULL, linha);
        adicionarFilho($$, $2);
    }
;

expressao:
    var ATRIBUICAO expressao {
        $$ = criarNo("ATRIB", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $3);
    }
  | simples_expressao { $$ = $1; }
;

simples_expressao:
    soma_expressao relacional soma_expressao {
        $$ = criarNo("REL", NULL, linha);
        adicionarFilho($$, $1);
        adicionarFilho($$, $2);
        adicionarFilho($$, $3);
    }
  | soma_expressao { $$ = $1; }
;

relacional:
      MENOR      { $$ = criarNo("OP", "<", linha); }
    | MAIOR      { $$ = criarNo("OP", ">", linha); }
    | MENORIGUAL { $$ = criarNo("OP", "<=", linha); }
    | MAIORIGUAL { $$ = criarNo("OP", ">=", linha); }
    | IGUAL      { $$ = criarNo("OP", "==", linha); }
    | DIFERENTE  { $$ = criarNo("OP", "!=", linha); }
;

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

fator:
    ABREPARENTESES expressao FECHAPARENTESES { $$ = $2; }
  | var { $$ = $1; }
  | NUM {
        char numstr[16];
        sprintf(numstr, "%d", $1);
        $$ = criarNo("NUM", numstr, linha);
    }
;

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

        /* liga o modo debug do Bison */
        yydebug = 1;

        yyparse();
        printf("\n=== ÁRVORE SINTÁTICA ===\n");
        imprimirArvore(raiz, 0);
        fclose(fp);
    } else {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }
    return 0;
}
