# Decisões Técnicas

Este documento registra as decisões adotadas na implementação inicial do interpretador de um subconjunto da linguagem C.

## 1. Organização dos arquivos

Os arquivos da fase léxica e sintática ficam na pasta `src/`:

- `lexer.l`: especificação do analisador léxico em Flex;
- `parser.y`: especificação inicial do analisador sintático em Bison;
- `Makefile`: automação da geração e compilação do projeto.

Os arquivos `lex.yy.c`, `parser.tab.c` e `parser.tab.h` são gerados automaticamente pelo Flex e pelo Bison e não devem ser editados manualmente.

## 2. Ferramentas utilizadas

O projeto utiliza:

- **Flex** para gerar o analisador léxico;
- **Bison** para gerar o analisador sintático;
- **GCC** para compilar os arquivos C gerados;
- **Make** para automatizar o processo de build.

O comando `make` executa as etapas de geração do parser, geração do lexer e compilação do executável.

## 3. Organização dos tokens

As palavras reservadas são reconhecidas antes da regra genérica de identificadores. Dessa forma, palavras como `int`, `float`, `if`, `else`, `while`, `return` e `printf` recebem tokens específicos.

Os identificadores seguem o padrão:

``` [A-Za-z_][A-Za-z0-9_]*