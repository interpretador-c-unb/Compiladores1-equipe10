# Otimização da AST (Constant Folding)

Esta página documenta o módulo de otimização (`src/optimizer.c` e `src/optimizer.h`) e sua integração no driver principal (`src/main.c`), com base no código-fonte e na execução do caso de teste `testes/validos/expressoes_constantes.c`.

---

## Visão geral e ativação via CLI

O interpretador aplica a técnica de **Constant Folding** (dobramento de constantes) diretamente sobre a Árvore Sintática Abstrata (AST), **após** o parsing (`yyparse()`) e **antes** da análise semântica (`analyze_semantics()`) e da execução (`execute_program()`).

A otimização é opcional e controlada por linha de comando em `src/main.c`:

```bash
./interpreter --optimize arquivo.c
# ou usando as formas abreviadas:
./interpreter -O arquivo.c
./interpreter -O1 arquivo.c
```

Para inspecionar o efeito da otimização na árvore, combine com `--print-ast`:

```bash
./interpreter --print-ast testes/validos/expressoes_constantes.c
./interpreter --optimize --print-ast testes/validos/expressoes_constantes.c
```

---

## Algoritmo e percurso na AST

A função `optimize_ast(ASTNode *node)` em `src/optimizer.c` realiza um **percurso em pós-ordem** (*bottom-up*):

1. **Otimiza primeiro os filhos** (`node->left`, `node->right` e, quando presente, `node->else_branch`). Isso garante que subexpressões compostas (como `2 + 3 * 4`) sejam reduzidas de baixo para cima em uma única passada.
2. **Verifica se o nó atual é dobrável:**
   - O nó precisa ser do tipo `NODE_BINARY_EXPR`.
   - Ambos os operandos (`node->left` e `node->right`) devem satisfazer `is_constant_node()`, ou seja, serem do tipo `NODE_NUM_LITERAL`.
3. **Calcula o novo valor constante** conforme o operador em `node->name`.
4. **Substitui a subárvore:** cria um novo nó `NODE_NUM_LITERAL` com `create_num_node(result_val)`, preserva o ponteiro `next` (`folded->next = node->next`), libera a subárvore antiga com `free_ast(node)` (isolando `node->next = NULL` antes de liberar) e **retorna o novo nó imediatamente**.
5. **Avança para o próximo comando (`node->next`):** quando o nó atual **não** foi dobrado (por exemplo, um nó `Declaration`, `Assignment`, `If` ou `While`), a função chama `optimize_ast(node->next)`. Caso o próprio nó da lista tenha sido dobrado no passo 4, o retorno antecipado faz com que os irmãos posteriores em `next` não sejam percorridos.

### Detecção de `float` nos literais

O código em `src/optimizer.c` verifica se algum dos operandos possui ponto decimal no campo `name` (`strchr(node->left->name, '.')` ou `strchr(node->right->name, '.')`) para preencher `folded->name` com `snprintf(buf, sizeof(buf), "%g", result_val)` (acrescentando `".0"` quando necessário).

Na prática, porém, os literais criados pelo parser via `create_num_node($1)` em `src/ast.c` preenchem apenas `num_value` e deixam `name` como `NULL`. Por isso, `is_float` não é ativado para literais vindos do código-fonte, e expressões com ponto flutuante dobradas pelo otimizador perdem a marcação de `float` na AST atual.

---

## Regras matemáticas implementadas

A tabela abaixo lista todas as operações suportadas por `optimize_ast()` em `src/optimizer.c`:

| Categoria | Operador | Regra aplicada | Restrição / Segurança |
| :--- | :---: | :--- | :--- |
| **Aritméticos** | `+` | `left_val + right_val` | — |
| | `-` | `left_val - right_val` | — |
| | `*` | `left_val * right_val` | — |
| | `/` | `left_val / right_val` | Não dobra se `right_val == 0` (evita divisão por zero em compilação). |
| | `%` | `(int)left_val % (int)right_val` | Não dobra se `(int)right_val == 0`; trunca operandos para `int`. |
| **Relacionais** | `>` | `left_val > right_val` (`1` ou `0`) | — |
| | `<` | `left_val < right_val` (`1` ou `0`) | — |
| | `>=` | `left_val >= right_val` (`1` ou `0`) | — |
| | `<=` | `left_val <= right_val` (`1` ou `0`) | — |
| | `==` | `left_val == right_val` (`1` ou `0`) | — |
| | `!=` | `left_val != right_val` (`1` ou `0`) | — |
| **Lógicos** | `&&` | `(int)left_val && (int)right_val` (`1` ou `0`) | — |
| | `\|\|` | `(int)left_val \|\| (int)right_val` (`1` ou `0`) | — |

Quando um operador não está listado ou ocorre divisão/módulo por zero (`foldable = 0`), a subárvore é mantida intacta para tratamento nas fases seguintes.

---

## Comparativo antes e depois da otimização

Abaixo está o comparativo obtido executando `testes/validos/expressoes_constantes.c`:

```c
int a = 2 + 3 * 4;
int b = (10 - 2) / 2;
int c = a + 5;
```

### Estrutura da AST (saída real de `--print-ast`)

**Sem `--optimize` (16 nós):**

```text
Declaration(a)
  BinaryExpr(+)
    Num(2)
    BinaryExpr(*)
      Num(3)
      Num(4)
Declaration(b)
  BinaryExpr(/)
    BinaryExpr(-)
      Num(10)
      Num(2)
    Num(2)
Declaration(c)
  BinaryExpr(+)
    Var(a)
    Num(5)
```

**Com `--optimize` (8 nós):**

```text
Declaration(a)
  Num(14)
Declaration(b)
  Num(4)
Declaration(c)
  BinaryExpr(+)
    Var(a)
    Num(5)
```

### Diagrama visual da redução (`int a = 2 + 3 * 4;`)

```text
Antes (5 nós na expressão):        Depois (1 nó na expressão):

        BinaryExpr(+)
        /           \
    Num(2)      BinaryExpr(*)      ===>       Num(14)
                 /         \
             Num(3)       Num(4)
```

### Métricas de redução de nós

| Escopo medido | Nós antes | Nós depois | Nós eliminados | Redução |
| :--- | :---: | :---: | :---: | :---: |
| Expressão de `a` (`2 + 3 * 4`) | 5 | 1 | 4 | 80,0% |
| Expressão de `b` (`(10 - 2) / 2`) | 5 | 1 | 4 | 80,0% |
| Expressão de `c` (`a + 5`) | 3 | 3 | 0 | 0,0% |
| **Programa completo (`expressoes_constantes.c`)** | **16** | **8** | **8** | **50,0%** |

Em ambos os casos, a execução final no runtime produz exatamente o mesmo estado de memória: `a = 14`, `b = 4`, `c = 19`.

---

## Limitações conhecidas

- **Sem propagação de constantes (*Constant Propagation*):** variáveis já conhecidas (como `a` em `int c = a + 5;`) permanecem como `NODE_VAR` na AST, pois o otimizador avalia apenas nós `NODE_NUM_LITERAL`.
- **Apenas expressões binárias:** operadores unários ou instruções de controle de fluxo com condição constante (ex.: `if (1)`) não são simplificados pelo otimizador atual.
- **Perda da marcação `float` após o dobramento:** como `create_num_node()` deixa `name` nulo, a verificação de ponto decimal em `optimizer.c` não detecta literais `float` vindos do parser, tratando o resultado dobrado sem a marcação decimal.
- **Retorno antecipado em listas encadeadas:** se uma `NODE_BINARY_EXPR` estiver diretamente na lista encadeada por `next` (ex.: `2 + 3; 4 + 5;`) e for dobrada, `optimize_ast()` retorna antes de processar `node->next`, deixando os irmãos seguintes sem otimização.
