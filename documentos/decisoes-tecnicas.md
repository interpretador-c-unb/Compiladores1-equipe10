# Decisões Técnicas

Este documento registra as decisões adotadas na implementação inicial do interpretador de um subconjunto da linguagem C.

## 1. Organização dos Arquivos

Os arquivos da fase léxica e sintática ficam alocados no diretório `src/`:

- `interpretador_c.l`: especificação do analisador léxico em Flex;
- `interpretador_c.y`: especificação inicial do analisador sintático em Bison;
- `Makefile`: automação local da geração e compilação do projeto.

Os arquivos `lex.yy.c`, `parser.tab.c` e `parser.tab.h` são gerados automaticamente pelo Flex e pelo Bison durante a compilação e não devem ser versionados nem editados manualmente.

## 2. Ferramentas Utilizadas

O projeto utiliza a seguinte pilha tecnológica:

- **Flex:** Gerador de analisador léxico (scanner);
- **Bison:** Gerador de analisador sintático (parser);
- **GCC:** Compilador C para construir o executável do interpretador;
- **Make:** Ferramenta de automação de build.

O comando `make` executa em sequência: a geração do parser, a geração do lexer e a compilação final do executável.

## 3. Organização dos Tokens

As palavras reservadas são reconhecidas com prioridade em relação à regra genérica de identificadores. Dessa forma, termos como `int`, `float`, `if`, `else`, `while`, `return` e `printf` geram tokens específicos diretamente.

Os identificadores válidos seguem a expressão regular:
```text
[A-Za-z_][A-Za-z0-9_]*