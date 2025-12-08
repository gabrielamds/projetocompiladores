# ==============================================================================
# Makefile - Compilador C-
# ==============================================================================
#
# Este Makefile compila todos os modulos do compilador C-.
#
# ESTRUTURA DO PROJETO:
#   1_AnalisadorLexico/   - Scanner (2.0 pontos)
#   2_TabelaSimbolos/     - Tabela de Simbolos (1.5 pontos)
#   3_AnalisadorSintatico/- Parser (2.5 pontos)
#   4_ArvoreSintatica/    - Arvore Sintatica (1.5 pontos)
#   5_AnalisadorSemantico/- Analisador Semantico (2.5 pontos)
#   6_CompiladorCompleto/ - Todos os modulos integrados
#
# USO:
#   make          - Compila o compilador
#   make test     - Testa com arquivo de exemplo
#   make clean    - Remove arquivos gerados
#
# ==============================================================================

# Compilador C e flags
CC = gcc
CFLAGS = -g

# Ferramentas Flex e Bison
FLEX = C:/msys64/usr/bin/flex.exe
BISON = C:/msys64/usr/bin/bison.exe

# Arquivos fonte
LEX_SRC = cminus.l
YACC_SRC = cminus.y
ARVORE_SRC = arvore.c
SIMBOLOS_SRC = simbolos.c
SEMANTICO_SRC = semantico.c

# Arquivos gerados
LEX_OUT = lex.yy.c
YACC_OUT = cminus.tab.c
YACC_HEADER = cminus.tab.h

# Executavel final
TARGET = compilador

# ==============================================================================
# REGRAS
# ==============================================================================

# Regra principal
all: $(TARGET)
	@echo.
	@echo ================================================
	@echo   Compilador C- gerado com sucesso!
	@echo   Uso: compilador.exe arquivo.cm
	@echo ================================================

# Gera o executavel
$(TARGET): $(LEX_OUT) $(YACC_OUT) $(ARVORE_SRC) $(SIMBOLOS_SRC) $(SEMANTICO_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(LEX_OUT) $(YACC_OUT) $(ARVORE_SRC) $(SIMBOLOS_SRC) $(SEMANTICO_SRC)

# Gera o scanner com Flex
$(LEX_OUT): $(LEX_SRC) $(YACC_HEADER)
	$(FLEX) $(LEX_SRC)

# Gera o parser com Bison
$(YACC_OUT) $(YACC_HEADER): $(YACC_SRC)
	$(BISON) -d -v $(YACC_SRC)

# Limpa arquivos gerados
clean:
	del /Q $(LEX_OUT) $(YACC_OUT) $(YACC_HEADER) cminus.output $(TARGET).exe 2>nul

# Testa com arquivo de exemplo
test: $(TARGET)
	./$(TARGET) teste.cm

.PHONY: all clean test
