# Estrutura do Projeto

Nesta seção é apresentada a organização de diretórios e arquivos do repositório, destacando as responsabilidades de cada módulo no desenvolvimento do interpretador de C (**Nosso C**).

> **Sprint 14 integrada.** Escopos locais, curto-circuito e tratamento de divisão por zero estão na `main`. Veja os [resultados dos testes e pendências](status-projeto.md).

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
│   ├── decisoes-tecnicas.md
│   ├── gramatica-bnf.md
│   ├── guia-uso.md
│   ├── index.md
│   ├── interpretador.md
│   ├── planejamento-sprints.md
│   ├── tabela-simbolos.md
|   ├── status-projeto.md
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