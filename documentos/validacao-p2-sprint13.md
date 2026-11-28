# Validação da Sprint 13 — Apresentação P2 e backlog de ajustes

## Objetivo

Registrar a validação das funcionalidades demonstradas no P2 e manter rastreáveis os problemas e ajustes identificados para a etapa final do projeto. Esta página atende à documentação da Issue #64 sem introduzir uma funcionalidade que não tenha sido solicitada ou observada durante a demonstração.

## Escopo demonstrado

A demonstração deve utilizar a versão da `main` aprovada para o P2 e cobrir:

- análise léxica e sintática;
- análise semântica;
- construção e impressão da AST;
- avaliação de expressões;
- execução de `if`, `else` e `while`;
- comando `printf()`;
- otimização por constant folding, quando apresentada;
- liberação da AST e do ambiente de runtime.

## Casos de demonstração

A validação pode ser reproduzida a partir da raiz do repositório:

```bash
make -C src rebuild
./interpreter testes/validos/14_while_execucao.c
./interpreter testes/validos/15_if_ramos.c
./interpreter testes/validos/16_printf.c
./interpreter testes/validos/17_if_while_printf.c
```

A Issue #64 menciona `if_while.c` e `printf.c`, mas esses nomes não existem na árvore atual. Os equivalentes versionados são `17_if_while_printf.c` e `16_printf.c`; usar os nomes reais evita um falso erro de demonstração.

Para conferir o modo de otimização:

```bash
./interpreter --optimize --print-ast testes/validos/expressoes_constantes.c
```

## Registro de problemas

A Issue #64 não identifica uma falha concreta de execução que possa ser corrigida sem inventar um requisito. Os feedbacks e as prioridades já registradas após o P2 estão em [Status do Projeto](status-projeto.md#11-feedbacks-do-p2-e-priorizacao-do-backlog).

Qualquer correção adicional deve ser registrada no PR correspondente com:

1. o caso de entrada que reproduz o problema;
2. a saída observada;
3. a saída esperada;
4. o arquivo responsável pela correção;
5. a regressão executada após a mudança.

Não foram adicionadas alterações ao parser, ao runtime ou à gramática apenas para preencher a Issue #64.

## Critério de validação

A atividade é considerada documentalmente registrada quando:

- os casos usados na apresentação são reproduzíveis;
- os caminhos dos testes correspondem aos arquivos existentes;
- os feedbacks do P2 e o backlog estão centralizados no status do projeto;
- nenhuma correção de código é declarada sem um caso reproduzível;
- a página é incluída na navegação do MkDocs.
