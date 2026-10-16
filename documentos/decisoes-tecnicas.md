# Decisões Técnicas

Este documento registra as decisões adotadas na arquitetura e implementação do interpretador do subconjunto da linguagem C.

## 1. Organização dos Arquivos do Módulo

Todos os componentes da análise léxica, sintática e estruturas de dados residem no diretório `src/`:

- `interpretador_c.l`: especificação do analisador léxico em Flex;
- `interpretador_c.y`: especificação do analisador sintático em Bison;
- `ast.h` / `ast.c`: definição da estrutura de nós e construção dinâmica da Árvore Sintática Abstrata (AST);
- `symbol_table.h` / `symbol_table.c`: gerenciador de tabela de símbolos com controle dinâmico de escopo;
- `main.c`: driver principal que coordena a leitura de arquivos, acionamento do parser e liberação de memória;
- `Makefile`: automação local de build com targets de compilação limpa (`rebuild`, `clean`).

Os artefatos gerados (`lex.yy.c`, `interpretador_c.tab.c`, `interpretador_c.tab.h`) são construídos automaticamente na compilação.

## 2. Pilha Tecnológica

- **Flex:** Gerador de analisador léxico (scanner);
- **Bison:** Gerador de analisador sintático LALR (parser);
- **GCC (C11):** Compilador C para construção do executável final (`./interpreter`);
- **Make:** Ferramenta de automação do build.

## 3. Estruturas de Dados e Arquitetura

1. **Análise Léxica & Localização:** Rastreamento contínuo de linha (`yylineno`) e coluna (`lex_column()`) para relatórios detalhados em `yyerror()`.
2. **Árvore Sintática Abstrata (AST):** Estrutura genérica de nós representada por `struct ASTNode`, contendo ponteiros dinâmicos (`left`, `right`, `next`) para expressões, comandos e blocos.
3. **Tabela de Símbolos por Escopo:** Gerenciamento baseado em lista ligada encadeada com níveis de escopo (`current_scope`). Ao entrar em um bloco `{}` o escopo é incrementado e os símbolos correspondentes são descartados ao sair do bloco.