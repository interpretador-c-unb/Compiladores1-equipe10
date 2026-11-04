# Ferramentas
CC = gcc
FLEX = flex
BISON = bison

# Opções de compilação
CFLAGS = -Wall -Wextra -std=gnu11
LDLIBS = -lm

# Executável na raiz do repositório (./interpreter)
TARGET = ../interpreter
MAIN_C = main.c
AST_C = ast.c
SYMBOL_C = symbol_table.c
SEMANTIC_C = semantic.c
INTERPRETER_C = interpreter.c
OPTIMIZER_C = optimizer.c
LEXER = interpretador_c.l
PARSER = interpretador_c.y
LEXER_C = lex.yy.c
PARSER_C = interpretador_c.tab.c
PARSER_H = interpretador_c.tab.h

.PHONY: all clean rebuild

all: $(TARGET)

# Compila e liga todos os módulos
$(TARGET): $(PARSER_C) $(LEXER_C) $(MAIN_C) $(AST_C) $(SYMBOL_C) $(SEMANTIC_C) $(INTERPRETER_C) $(OPTIMIZER_C)
	$(CC) $(CFLAGS) -o $@ $(MAIN_C) $(AST_C) $(SYMBOL_C) $(SEMANTIC_C) $(INTERPRETER_C) $(OPTIMIZER_C) $(PARSER_C) $(LEXER_C) $(LDLIBS)

# -d gera o cabeçalho compartilhado pelo lexer e pelo parser
$(PARSER_C) $(PARSER_H): $(PARSER)
	$(BISON) -d -o $(PARSER_C) $(PARSER)

# O lexer inclui o cabeçalho gerado pelo Bison
$(LEXER_C): $(LEXER) $(PARSER_H)
	$(FLEX) -o $@ $(LEXER)

rebuild: clean all

clean:
	rm -f $(TARGET) parser $(LEXER_C) $(PARSER_C) $(PARSER_H)