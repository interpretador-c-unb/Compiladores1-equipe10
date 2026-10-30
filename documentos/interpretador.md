# Interpretador e Environment — Sprint 9

**Estado verificado em 30/10/2026:** a AST é construída e impressa, mas ainda não é executada. Não existem `interpreter.c`, `interpreter.h`, `evaluate_expr()` ou uma estrutura `Environment` no código consultado. A issue #39 de implementação e a #40 de documentação permanecem abertas.

Esta página descreve a base existente e o modelo conceitual previsto para a Sprint 9. As estruturas e operações propostas abaixo precisam ser validadas com a equipe de desenvolvimento e com Paulo Nina, par de documentação; não representam uma API implementada.

## Ciclo atual

```text
Arquivo C → scanner Flex → parser Bison → AST encadeada
                               │              │
                               ▼              ▼
                      Tabela de símbolos   print_ast()
                               │              │
                               └──────┬───────┘
                                      ▼
                           Liberação pelo main.c
```

`main.c` abre o arquivo, cria a tabela de símbolos e chama `yyparse()`. Durante o parsing, as ações constroem a AST e fazem verificações iniciais de declaração e escopo. Ao terminar, o driver fecha o arquivo, informa o resultado sintático, imprime a AST e libera as estruturas.

A raiz `ast_root` aponta para uma lista de nós ligada por `next`. Embora `NODE_PROGRAM` exista no enum, a regra inicial do parser entrega diretamente essa lista, sem criar um nó de programa.

## Tabela de símbolos e memória de execução

| Estrutura | Responsabilidade | Estado |
| --- | --- | --- |
| `SymbolTable` | Registrar nome, tipo declarado e nível de escopo durante o parsing. | Implementada em `symbol_table.c/h`. |
| AST | Representar literais, referências, expressões, declarações e comandos. | Implementada em `ast.c/h`. |
| `Environment` | Guardar e atualizar valores das variáveis durante a execução. | Pendente. |

`Symbol` possui `name`, `data_type`, `scope_level` e `next`; não possui um campo para o valor corrente da variável. Ao fechar um bloco durante o parsing, `exit_scope()` remove seus símbolos. Portanto, essa tabela não fornece, por si só, a memória necessária para executar a AST posteriormente.

## Modelo proposto para execução

O planejamento prevê execução direta da AST, com avaliação recursiva das expressões. O fluxo conceitual é:

```text
AST construída
      │
      ▼
Validação semântica                     [pendente — Sprint 8]
      │ somente se válida
      ▼
Criar Environment global                [pendente — Sprint 9]
      │
      ▼
Percorrer comandos por next
      │
      ├── avaliar expressões recursivamente
      └── consultar/atualizar variáveis no Environment
      │
      ▼
Liberar Environment e AST
```

O driver ainda precisa integrar essas etapas. O sucesso de `yyparse()` sozinho não comprova validade semântica; os diagnósticos atuais não são propagados como falha, conforme a [documentação da Sprint 8](analise-semantica.md).

### Avaliação dos nós

A tabela usa os nomes e campos reais da AST. A coluna de avaliação descreve comportamento proposto, ainda não implementado.

| Nó atual | Dados existentes | Avaliação proposta |
| --- | --- | --- |
| `NODE_NUM_LITERAL` | `num_value` | Produzir um valor numérico. |
| `NODE_STRING_LITERAL` | `string_value` | Produzir um valor textual conforme a política de tipos a definir. |
| `NODE_VAR_REF` | `name` | Buscar o valor da variável no ambiente visível. |
| `NODE_BINARY_EXPR` | Operador em `name`, operandos em `left` e `right`. | Avaliar operandos e aplicar o operador, respeitando suas regras de tipos e avaliação. |
| `NODE_DECLARATION` | Nome em `name`, inicializador opcional em `left`. | Criar uma variável no escopo atual e avaliar o inicializador, quando presente. |
| `NODE_ASSIGNMENT` | Destino em `name`, expressão em `left`. | Avaliar a expressão e atualizar a variável visível correspondente. |
| `NODE_BLOCK` | Lista de comandos em `left`. | Abrir escopo de execução, executar a lista e fechar o escopo. |
| `NODE_IF` | Condição em `left`, ramo principal em `right`, alternativo em `else_branch`. | Executar somente o ramo selecionado; previsto para a Sprint 11. |
| `NODE_WHILE` | Condição em `left`, corpo em `right`. | Reavaliar a condição a cada repetição; previsto para a Sprint 11. |

O planejamento usa o nome `NODE_ASSIGN`, mas o identificador implementado é **`NODE_ASSIGNMENT`**. Além disso, o parser representa `!` e o menos unário por `NODE_BINARY_EXPR` com `right == NULL`: o avaliador deve distinguir esses casos das operações binárias.

Uma futura `evaluate_expr()` poderá avaliar, por exemplo, a árvore de `2 + 3 * 4` calculando primeiro `3 * 4` e depois a soma, resultando em `14`. Esse agrupamento já está na AST por causa da precedência do parser; o cálculo ainda não ocorre. Para operadores lógicos, a equipe precisa definir e implementar a avaliação com curto-circuito, sem avaliar automaticamente ambos os operandos.

## Environment e escopos: proposta conceitual

Um modelo possível é manter uma cadeia de ambientes: cada bloco cria um ambiente local ligado ao ambiente externo. A busca começa no local e segue para os externos até encontrar o nome.

```text
Environment do bloco (escopo ativo)
  variáveis: nome → tipo, valor, estado de inicialização
  externo ───────────────────────────────────────┐
                                                 ▼
                                    Environment global
                                      variáveis: nome → tipo, valor,
                                                  estado de inicialização
                                      externo: nenhum
```

O diagrama representa responsabilidades, não uma definição de `struct` já aprovada. A escolha concreta entre lista, mapa ou outra estrutura, bem como os nomes das funções, continua pendente.

### Operações necessárias

1. Criar o ambiente global antes da execução.
2. Declarar variáveis no ambiente ativo, verificando duplicação nesse escopo.
3. Consultar nomes do escopo mais interno para o mais externo, permitindo sombreamento conforme a regra já usada pela tabela de símbolos.
4. Atualizar o ambiente onde a variável foi encontrada, sem criar outra variável implicitamente na atribuição.
5. Ao entrar em um bloco, criar o ambiente local; ao sair, restaurar o ambiente externo e liberar os valores locais sob sua responsabilidade.
6. Ao terminar ou falhar, liberar os ambientes restantes, sem liberar um ambiente externo ainda em uso.

Essa cadeia funciona como uma pilha de escopos léxicos. Não implica uma pilha de chamadas de funções: chamadas e declarações de funções ainda não são suportadas pelo parser atual.

### Exemplo de valores esperados no modelo proposto

```c
int x = 2;
{
    int y = 3;
    x = x + y;
}
x = x + 1;
```

| Etapa | Ambiente global | Ambiente local |
| --- | --- | --- |
| Declarar `x` | `x = 2` | Não existe. |
| Entrar no bloco e declarar `y` | `x = 2` | `y = 3`. |
| Executar `x = x + y` | `x = 5` | `y = 3`. |
| Sair do bloco | `x = 5` | Liberado; `y` deixa de estar visível. |
| Executar a última atribuição | `x = 6` | Não existe. |

Esses valores são uma simulação do comportamento pretendido, não saída do executável atual, que apenas imprime a AST.

## Tipos e memória: decisões pendentes

- **Representação de valores:** definir como distinguir `int`, `float`, `char` e textos no runtime. O `double` usado em `num_value` não implementa, sozinho, a semântica desses tipos.
- **Preservação do tipo declarado:** o nó de declaração atual não armazena esse tipo, e os símbolos locais são removidos após o parsing. A equipe deve definir como a fase de execução obterá essa informação.
- **Inicialização:** definir o comportamento de uma declaração sem inicializador e de uma leitura antes da inicialização; não presumir valor zero.
- **Conversões e operadores:** implementar as regras aprovadas na Sprint 8, incluindo a coerção prevista de `int` para `float` e o comportamento de divisão e resto.
- **Textos e caracteres:** o parser transforma `CHARLIT` em nó de texto. Definir a distinção de tipos e quem possui e libera a memória dos valores textuais.
- **Falhas de execução:** definir a propagação de erro para o driver e as mensagens para operações inválidas, sem apresentar essas mensagens como existentes.

A ausência dessas decisões impede afirmar que o `Environment` esteja pronto ou que os programas já possam ser executados.

## Entregas e pendências

| Entrega | Responsável no planejamento | Estado |
| --- | --- | --- |
| Documento de arquitetura do runtime | Rafael Laube | Base atual e proposta documentadas; validação com implementação e revisão pelo par pendentes. |
| Diagrama arquitetural | Paulo Nina | Diagrama conceitual incluído nesta revisão; validação pelo responsável pendente. |
| `interpreter.c` e `evaluate_expr()` | Nicole Jovita | Pendente. |
| `Environment` e atualização de variáveis | Danielly Mendes | Pendente. |
| Avaliação aritmética em runtime | Bernardo Campos | Pendente. |
| Publicação no GitHub Pages | Rafael Laube e Paulo Nina | Pendente para esta revisão. |

A issue #39 agrega interpretador e ambiente de runtime sob Nicole; a divisão acima segue o detalhamento do planejamento. A issue #40 atribui a documentação a Rafael, com revisão de Paulo. Não foi presumida aprovação ou conclusão de trabalho de outro integrante.

## Validação da documentação

Com as dependências do MkDocs instaladas, execute na raiz:

```bash
mkdocs build --strict
mkdocs serve
```

Abra `http://127.0.0.1:8000` e confira **Interpretador e Runtime** no menu, incluindo os diagramas, a tabela de nós e os estados de pendência. Os diagramas são blocos de texto, sem dependência de renderizador adicional.

Para observar a base implementada, use os comandos do [guia de uso](guia-uso.md). A suíte atual verifica o front-end; testes de resultados aritméticos, atualização de valores e ciclo de vida do `Environment` ainda precisam ser criados junto com a implementação.
