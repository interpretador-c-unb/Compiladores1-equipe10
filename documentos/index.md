# Estrutura do Projeto

Nesta seção é apresentada a organização de diretórios e arquivos do repositório, destacando as responsabilidades de cada módulo no desenvolvimento do interpretador de C (**Nosso C**).

> **Status Atual (Sprint 12):** Desenvolvimento concluído até a Sprint 11 (`semantic.c`, `interpreter.c`, `optimizer.c`, estruturas `if`/`while` e comando `printf()` funcionais). Entregáveis do Ponto de Controle 2 concluídos e equipe atualmente focada nos testes de memória com Valgrind e estabilização de runtime (Sprint 12).

```text
COMPILADORES1-EQUIPE10/
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   └── sprint_task.md
│   └── workflows/
│       └── deploy-docs.yml
├── documentos/
│   ├── analisador-lexico.md
│   ├── analisador-sintatico.md
│   ├── analise-semantica.md
│   ├── debitos-tecnicos.md
│   ├── decisoes-tecnicas.md
│   ├── gramatica-bnf.md
│   ├── guia-uso.md
│   ├── index.md
│   ├── interpretador.md
│   ├── planejamento-sprints.md
│   ├── plano-pos-p1.md
│   ├── problemas-e-solucoes.md
│   ├── tabela-simbolos.md
│   └── scripts/
│       ├── header.tex
│       └── mdtopdf.sh
├── src/
│   ├── ast.c
│   ├── ast.h
│   ├── compilar.README.md
│   ├── interpretador_c.l
│   ├── interpretador_c.y
│   ├── interpreter.c
│   ├── interpreter.h
│   ├── main.c
│   ├── Makefile
│   ├── optimizer.c
│   ├── optimizer.h
│   ├── semantic.c
│   ├── semantic.h
│   ├── symbol_table.c
│   └── symbol_table.h
├── testes/
│   ├── 1_lexico/
│   ├── invalidos/
│   ├── validos/
│   ├── executar_testes.sh
│   └── rodar_testes.sh
├── .gitignore
├── Makefile
├── mkdocs.yml
└── README.md