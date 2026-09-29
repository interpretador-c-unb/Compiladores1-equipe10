# Ferramentas
CC = gcc
FLEX = flex
BISON = bison

# Opções de compilação
CFLAGS = -Wall -Wextra -std=gnu11
LDLIBS =

# Executável na raiz do repositório (./interpreter)
TARGET = ../interpreter
MAIN_C = main.c
LEXER = interpretador_c.l
PARSER = interpretador_c.y
LEXER_C = lex.yy.c
PARSER_C = interpretador_c.tab.c
PARSER_H = interpretador_c.tab.h

.PHONY: all clean rebuild

all: $(TARGET)

# Compila e liga o main.c, o parser e o lexer
$(TARGET): $(PARSER_C) $(LEXER_C) $(MAIN_C)
	$(CC) $(CFLAGS) -o $@ $(MAIN_C) $(PARSER_C) $(LEXER_C) $(LDLIBS)

# -d gera o cabeçalho compartilhado pelo lexer e pelo parser
$(PARSER_C) $(PARSER_H): $(PARSER)
	$(BISON) -d -o $(PARSER_C) $(PARSER)

# O lexer inclui o cabeçalho gerado pelo Bison
$(LEXER_C): $(LEXER) $(PARSER_H)
	$(FLEX) -o $@ $(LEXER)

rebuild: clean all

clean:
	rm -f $(TARGET) parser $(LEXER_C) $(PARSER_C) $(PARSER_H)