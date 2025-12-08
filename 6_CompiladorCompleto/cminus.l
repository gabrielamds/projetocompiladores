%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cminus.tab.h"   /* tokens e YYSTYPE do Bison */

int linha = 1;
extern FILE *yyin;
%}

/* Macros */

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*
number      {digit}+

%%

"/*"([^*]|\*+[^*/])*\*+"/"      { /* ignora comentario de bloco */ }
"//".*                          { /* ignora comentario de linha */ }

"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"return"    { return RETURN; }
"int"       { return INT; }
"void"      { return VOID; }

"=="        { return IGUAL; }
"!="        { return DIFERENTE; }
"<="        { return MENORIGUAL; }
">="        { return MAIORIGUAL; }
"<"         { return MENOR; }
">"         { return MAIOR; }
"="         { return ATRIBUICAO; }
"+"         { return MAIS; }
"-"         { return MENOS; }
"*"         { return VEZES; }
"/"         { return DIVIDIDO; }
";"         { return PONTOVIRGULA; }
","         { return VIRGULA; }
"("         { return ABREPARENTESES; }
")"         { return FECHAPARENTESES; }
"["         { return ABRECOLCHETE; }
"]"         { return FECHACOLCHETE; }
"{"         { return ABRECHAVE; }
"}"         { return FECHACHAVE; }

{identifier} {
    yylval.str = strdup(yytext);   /* passa lexema para o parser */
    return ID;
}

{number} {
    yylval.num = atoi(yytext);     /* passa valor inteiro para o parser */
    return NUM;
}

[ \t\r]+        { /* ignora espacos */ }

\n              { linha++; }

.               { printf("ERRO LEXICO: '%s' LINHA: %d\n", yytext, linha); }

%%

int yywrap(void) {
    return 1;
}
