%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cminus.tab.h"   /* tokens e YYSTYPE do Bison */

int linha = 1;
extern FILE *yyin;
extern int yylineno;  /* Variável do Flex que rastreia o número da linha */

/* Tabela de tokens para registro */
typedef struct {
    int numero;           /* Número do token */
    char tipo[50];        /* Tipo do token (IF, ID, NUM, etc) */
    char lexema[100];     /* Valor original do código */
    int linhaToken;       /* Linha onde foi encontrado */
} Token;

/* Tabela de erros léxicos */
typedef struct {
    int numero;           /* Número do erro */
    char caractere[10];   /* Caractere inválido */
    int linhaErro;        /* Linha onde ocorreu */
} ErroLexico;

Token *tabela_tokens = NULL;
int total_tokens = 0;
int capacidade_tokens = 100;

ErroLexico *tabela_erros_lexicos = NULL;
int total_erros_lexicos = 0;
int capacidade_erros = 50;

void registrarToken(const char *tipo, const char *lexema) {
    if (tabela_tokens == NULL) {
        tabela_tokens = (Token*)malloc(sizeof(Token) * capacidade_tokens);
    }
    
    if (total_tokens >= capacidade_tokens) {
        capacidade_tokens *= 2;
        tabela_tokens = (Token*)realloc(tabela_tokens, sizeof(Token) * capacidade_tokens);
    }
    
    tabela_tokens[total_tokens].numero = total_tokens + 1;
    strncpy(tabela_tokens[total_tokens].tipo, tipo, sizeof(tabela_tokens[total_tokens].tipo) - 1);
    tabela_tokens[total_tokens].tipo[sizeof(tabela_tokens[total_tokens].tipo) - 1] = '\0';
    strncpy(tabela_tokens[total_tokens].lexema, lexema, sizeof(tabela_tokens[total_tokens].lexema) - 1);
    tabela_tokens[total_tokens].lexema[sizeof(tabela_tokens[total_tokens].lexema) - 1] = '\0';
    tabela_tokens[total_tokens].linhaToken = yylineno;  /* Usar yylineno do Flex */
    total_tokens++;
}

void registrarErroLexico(const char *caractere) {
    if (tabela_erros_lexicos == NULL) {
        tabela_erros_lexicos = (ErroLexico*)malloc(sizeof(ErroLexico) * capacidade_erros);
    }
    
    if (total_erros_lexicos >= capacidade_erros) {
        capacidade_erros *= 2;
        tabela_erros_lexicos = (ErroLexico*)realloc(tabela_erros_lexicos, sizeof(ErroLexico) * capacidade_erros);
    }
    
    tabela_erros_lexicos[total_erros_lexicos].numero = total_erros_lexicos + 1;
    strncpy(tabela_erros_lexicos[total_erros_lexicos].caractere, caractere, sizeof(tabela_erros_lexicos[total_erros_lexicos].caractere) - 1);
    tabela_erros_lexicos[total_erros_lexicos].caractere[sizeof(tabela_erros_lexicos[total_erros_lexicos].caractere) - 1] = '\0';
    tabela_erros_lexicos[total_erros_lexicos].linhaErro = yylineno;
    total_erros_lexicos++;
}

void imprimirTabelaTokens(void) {
    if (total_tokens == 0) {
        printf("Nenhum token foi reconhecido.\n");
        return;
    }
    
    printf("\n");
    printf("==========================================================================\n");
    printf("                   TABELA DE TOKENS (ANALISE LEXICA)\n");
    printf("==========================================================================\n\n");
    
    printf("%-8s %-20s %-30s %-10s\n", "Num", "Tipo", "Lexema", "Linha");
    printf("--------------------------------------------------------------------------\n");
    
    for (int i = 0; i < total_tokens; i++) {
        printf("%-8d %-20s %-30s %-10d\n", 
               tabela_tokens[i].numero,
               tabela_tokens[i].tipo,
               tabela_tokens[i].lexema,
               tabela_tokens[i].linhaToken);
    }
    
    printf("\nTotal de tokens: %d\n\n", total_tokens);
}

void imprimirErrosLexicos(void) {
    if (total_erros_lexicos == 0) {
        printf("Nenhum erro lexico encontrado.\n\n");
        return;
    }
    
    printf("\n");
    printf("==========================================================================\n");
    printf("                     ERROS LEXICOS ENCONTRADOS\n");
    printf("==========================================================================\n\n");
    
    printf("%-8s %-20s %-10s\n", "Num", "Caractere Inválido", "Linha");
    printf("--------------------------------------------------------------------------\n");
    
    for (int i = 0; i < total_erros_lexicos; i++) {
        printf("%-8d %-20s %-10d\n", 
               tabela_erros_lexicos[i].numero,
               tabela_erros_lexicos[i].caractere,
               tabela_erros_lexicos[i].linhaErro);
    }
    
    printf("\nTotal de erros lexicos: %d\n\n", total_erros_lexicos);
}

void resetarTabelaTokens(void) {
    if (tabela_tokens != NULL) {
        free(tabela_tokens);
        tabela_tokens = NULL;
    }
    total_tokens = 0;
    capacidade_tokens = 100;
}

void resetarErrosLexicos(void) {
    if (tabela_erros_lexicos != NULL) {
        free(tabela_erros_lexicos);
        tabela_erros_lexicos = NULL;
    }
    total_erros_lexicos = 0;
    capacidade_erros = 50;
}
%}

/* Macros */

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*
number      {digit}+

%option yylineno

%%

"/*"([^*]|\*+[^*/])*\*+"/"      { /* ignora comentario de bloco */ }
"//".*                          { /* ignora comentario de linha */ }

"if"        { registrarToken("IF", yytext); return IF; }
"else"      { registrarToken("ELSE", yytext); return ELSE; }
"while"     { registrarToken("WHILE", yytext); return WHILE; }
"return"    { registrarToken("RETURN", yytext); return RETURN; }
"int"       { registrarToken("INT", yytext); return INT; }
"void"      { registrarToken("VOID", yytext); return VOID; }

"=="        { registrarToken("IGUAL", yytext); return IGUAL; }
"!="        { registrarToken("DIFERENTE", yytext); return DIFERENTE; }
"<="        { registrarToken("MENORIGUAL", yytext); return MENORIGUAL; }
">="        { registrarToken("MAIORIGUAL", yytext); return MAIORIGUAL; }
"<"         { registrarToken("MENOR", yytext); return MENOR; }
">"         { registrarToken("MAIOR", yytext); return MAIOR; }
"="         { registrarToken("ATRIBUICAO", yytext); return ATRIBUICAO; }
"+"         { registrarToken("MAIS", yytext); return MAIS; }
"-"         { registrarToken("MENOS", yytext); return MENOS; }
"*"         { registrarToken("VEZES", yytext); return VEZES; }
"/"         { registrarToken("DIVIDIDO", yytext); return DIVIDIDO; }
";"         { registrarToken("PONTOVIRGULA", yytext); return PONTOVIRGULA; }
","         { registrarToken("VIRGULA", yytext); return VIRGULA; }
"("         { registrarToken("ABREPARENTESES", yytext); return ABREPARENTESES; }
")"         { registrarToken("FECHAPARENTESES", yytext); return FECHAPARENTESES; }
"["         { registrarToken("ABRECOLCHETE", yytext); return ABRECOLCHETE; }
"]"         { registrarToken("FECHACOLCHETE", yytext); return FECHACOLCHETE; }
"{"         { registrarToken("ABRECHAVE", yytext); return ABRECHAVE; }
"}"         { registrarToken("FECHACHAVE", yytext); return FECHACHAVE; }

{identifier} {
    yylval.str = strdup(yytext);   /* passa lexema para o parser */
    registrarToken("ID", yytext);
    return ID;
}

{number} {
    yylval.num = atoi(yytext);     /* passa valor inteiro para o parser */
    registrarToken("NUM", yytext);
    return NUM;
}

[ \t\r]+        { /* ignora espacos */ }

\n              { linha++; }

.               { registrarErroLexico(yytext); }  /* Registra erro mas continua */

%%

int yywrap(void) {
    return 1;
}
