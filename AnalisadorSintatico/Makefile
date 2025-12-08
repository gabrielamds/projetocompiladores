# =============================================================================
# Makefile - Compilador C-
# 
# Este Makefile compila todos os modulos do compilador:
# - Analisador Lexico (flex)
# - Analisador Sintatico (bison)
# - Analisador Semantico
# - Tabela de Simbolos
# =============================================================================

# Compilador e flags
CC = gcc
CFLAGS = -Wall -g

# Ferramentas Flex e Bison
FLEX = flex
BISON = bison

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

# =============================================================================
# Regras de compilacao
# =============================================================================

# Regra principal: compila o compilador completo
all: $(TARGET)
	@echo ""
	@echo "============================================"
	@echo "Compilador C- gerado com sucesso!"
	@echo "Uso: ./$(TARGET) <arquivo.cm>"
	@echo "============================================"

# Linka todos os objetos para criar o executavel
$(TARGET): $(LEX_OUT) $(YACC_OUT) $(ARVORE_SRC) $(SIMBOLOS_SRC) $(SEMANTICO_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(LEX_OUT) $(YACC_OUT) $(ARVORE_SRC) $(SIMBOLOS_SRC) $(SEMANTICO_SRC)

# Gera o analisador lexico com Flex
$(LEX_OUT): $(LEX_SRC) $(YACC_HEADER)
	$(FLEX) $(LEX_SRC)

# Gera o analisador sintatico com Bison
$(YACC_OUT) $(YACC_HEADER): $(YACC_SRC)
	$(BISON) -d -v $(YACC_SRC)

# Limpa arquivos gerados
clean:
	rm -f $(LEX_OUT) $(YACC_OUT) $(YACC_HEADER) cminus.output $(TARGET) *.exe

# Limpa e recompila
rebuild: clean all

# Testa com um arquivo de exemplo
test: $(TARGET)
	./$(TARGET) teste.cm

# =============================================================================
# Informacoes
# =============================================================================
.PHONY: all clean rebuild test
