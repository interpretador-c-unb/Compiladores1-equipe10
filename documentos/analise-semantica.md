# Análise Semântica

**Estado verificado em 04/11/2026.** Esta página descreve o comportamento real do analisador (`src/semantic.c`), levantado a partir do código-fonte e de execuções de teste. Quando o comportamento atual diverge do esperado, ele aparece em [Limitações conhecidas](#limitacoes-conhecidas) e não é apresentado como regra da linguagem.

## Visão geral

A análise semântica roda **depois** do parsing, em uma passada própria sobre a AST. O fluxo do programa principal (`src/main.c`) é:

1. `yyparse()` faz a análise sintática, monta a AST e insere as declarações na tabela de símbolos. A detecção de **redeclaração** acontece nesta etapa.
2. Opcionalmente, `--optimize` aplica constant folding e `--print-ast` imprime a árvore.
3. `analyze_semantics()` percorre a AST, infere o tipo das expressões e verifica declaração de variáveis e atribuições.
4. Se houver erros contados, o programa imprime `[FALHA] Execução abortada devido a N erro(s) semântico(s).` e termina com código `1`. Caso contrário, executa o programa.

Todos os diagnósticos semânticos são escritos em `stderr`.

Cada bloco `{ ... }` abre um novo escopo. O mesmo nome pode ser declarado em escopos diferentes, mas não duas vezes no mesmo escopo.

## Tipos da linguagem

| Tipo | Como aparece | Observação |
| --- | --- | --- |
| `int` | declaração e literal inteiro | Tipo padrão de qualquer literal numérico (ver [limitações](#limitacoes-conhecidas)). |
| `float` | declaração | Tipo resultante quando algum operando é `float`. |
| `char` | declaração | Aceito pela gramática, sem regras próprias na análise. |
| `void` | declaração | Aceito pela gramática, sem regras próprias na análise. |
| `string` | literais de texto | Tipo interno dos literais de texto; não pode ser declarado. A gramática também cria o literal de caractere como nó de texto. |
| `unknown` | variável não declarada | Tipo interno. Propaga-se: uma expressão com operando `unknown` também é `unknown`. |

O tipo de uma variável é o que foi registrado na tabela de símbolos na declaração.

## Compatibilidade em atribuições

Matriz para `destino = origem;`. Os valores abaixo foram obtidos executando o interpretador.

| Destino \ Origem | `int` | `float` | `char` | `string` |
| --- | --- | --- | --- | --- |
| `int` | aceita | **aviso SEM-04** | sem checagem | sem checagem |
| `float` | aceita (promoção implícita) | aceita | sem checagem | sem checagem* |
| `char` | sem checagem | sem checagem | aceita | sem checagem |

**Legenda:** *aceita* = tipos iguais ou promoção prevista; *aviso* = a análise emite aviso e segue; *sem checagem* = nenhuma regra existe no código, então a atribuição passa sem diagnóstico. Isso não significa que a combinação tenha sido aprovada como regra da linguagem.

\* `float` recebendo `string` foi deduzido do código, não executado.

A única regra de incompatibilidade implementada é `int` recebendo `float`, e ela gera **aviso**, não erro: o programa continua e retorna código `0`.

**Declaração com valor inicial.** Em `int x = 2.5;` ou `int x = "texto";` a análise só procura variáveis não declaradas no inicializador. O tipo do inicializador **não é comparado** com o tipo declarado, então não há aviso nem erro.

## Tipo do resultado de expressões

### Operadores binários

A regra é a mesma para **todos** os operadores binários: aritméticos (`+ - * / %`), de igualdade e relacionais (`== != < <= > >=`) e lógicos (`&&` e `||`).

| Esquerdo \ Direito | `int` | `float` | `char` | `string` |
| --- | --- | --- | --- | --- |
| `int` | `int` | `float` | `int` | `int` |
| `float` | `float` | `float` | `float` | `float` |
| `char` | `int` | `float` | `int` | `int` |
| `string` | `int` | `float` | `int` | `int` |

Resumindo: se algum operando for `float`, o resultado é `float`; caso contrário, é `int`. Se algum operando for `unknown` (variável não declarada), o resultado é `unknown`.

Consequências desta regra, conforme o código:

- Comparações e operadores lógicos não produzem um tipo booleano. Comparar dois `float` resulta em `float`.
- Não há verificação de operandos por operador. `char` e `string` participam de qualquer operação sem diagnóstico.

### Operadores unários

Os operadores unários `-` e `!` resultam em `float` se o operando for `float`; caso contrário, em `int`.

### Condições de `if` e `while`

A expressão da condição é analisada apenas para detectar variáveis não declaradas. Qualquer tipo é aceito como condição.

## Conversões implícitas

| Conversão | Onde ocorre | Comportamento |
| --- | --- | --- |
| `int` → `float` | expressão com operandos mistos; atribuição a variável `float` | Aceita em silêncio. O resultado da expressão é `float`. |
| `float` → `int` | atribuição a variável `int` | Aviso SEM-04 na análise. Na execução, o valor é truncado: no teste `int a; float b = 2.5; a = b;` a variável `a` terminou com `2`. |
| Demais combinações com `char` e `string` | atribuições e expressões | Sem regra e sem diagnóstico. |

A análise semântica apenas **infere e avisa**. Ela não insere nós de conversão na AST.

## Erros e avisos semânticos

O programa não imprime códigos numéricos. Os identificadores `SEM-xx` abaixo existem só nesta documentação, para facilitar a referência.

| ID | Tipo | Mensagem emitida | Quando é emitida | Interrompe a execução? | Teste |
| --- | --- | --- | --- | --- | --- |
| SEM-01 | Erro | `[ERRO SEMÂNTICO] Variável 'x' usada sem declaração na linha N.` | Um nome usado em expressão ou condição não está na tabela de símbolos visível. | Sim, código `1` | `testes/invalidos/08_var_nao_declarada.c`, `15_variavel_fora_de_escopo.c` |
| SEM-02 | Erro | `[ERRO SEMÂNTICO] Atribuição a variável não declarada 'x' na linha N.` | O alvo de uma atribuição não está na tabela. | Sim, código `1` | `testes/invalidos/17_atribuicao_nao_declarada.c` |
| SEM-03 | Erro | `[ERRO SEMÂNTICO] Variável 'x' já declarada no escopo N` | O mesmo nome é declarado duas vezes no mesmo escopo. | **Não** (ver limitações) | `testes/invalidos/09_redeclaracao.c` |
| SEM-04 | Aviso | `[AVISO SEMÂNTICO] Atribuição de 'float' para variável 'int x' na linha N (pode haver perda de precisão).` | Atribuição de expressão `float` a variável `int`. | Não | `testes/invalidos/18_aviso_float_para_int.c` |

Exemplos:

```c
/* SEM-01 */
int x = 2;
x = x + z;      // 'z' não foi declarada
```

```c
/* SEM-02 */
int a = 1;
q = a;          // 'q' não foi declarada
```

```c
/* SEM-03 */
int x = 9;
int x = 8;      // mesmo nome, mesmo escopo
```

```c
/* SEM-04 */
int a = 1;
float b = 2.5;
a = b;          // float -> int
```

Para corrigir SEM-01 e SEM-02, declare a variável em um escopo visível antes de usá-la ou corrija o nome digitado. Para SEM-03, remova a declaração duplicada, use outro nome ou, se a intenção é atualizar o valor, escreva uma atribuição (`x = 8;`). Para SEM-04, declare a variável como `float` ou aceite o truncamento.

## Limitações conhecidas

Os itens abaixo foram confirmados por execução. Descrevem o comportamento **atual**, não o desejado.

1. **Redeclaração não derruba o programa (SEM-03).** A mensagem é impressa durante o parsing, mas não entra na contagem de erros semânticos. O programa imprime `[SUCESSO] Análise semântica concluída sem erros!`, executa e retorna `0`.
2. **Comandos logo após um `if` não são analisados.** Em `if (a > 0) { a = 2; }` seguido de `z = 5;`, nenhum erro é emitido para `z`, e o código de saída é `0`. A causa provável é a exceção `node->type != NODE_IF` ao percorrer `next` em `analyze_semantics`, herdada da época em que o `else` era guardado em `next`.
3. **Variáveis declaradas dentro de blocos geram erro falso.** Em `testes/validos/03_declaracoes.c` e `12_escopo_blocos.c`, o uso de variáveis declaradas dentro de `{ }` produz `usada sem declaração` e o programa retorna `1`, embora o código seja válido. A causa provável é que as declarações entram na tabela durante o parsing e os escopos dos blocos já foram descartados quando a análise percorre a AST.
4. **Número da linha incorreto.** A linha vem de `yylineno`, lido depois do parsing, e por isso aponta para o fim do arquivo, e não para o comando com problema. Exemplo: um erro na linha 4 de um arquivo de 5 linhas foi reportado na linha 6. As mensagens também não informam a coluna.
5. **Literais decimais são tratados como `int`.** O tipo de um literal numérico é decidido por `node->name`, que o nó numérico não preenche. Em `int a = 1; a = 2.5;` não há aviso SEM-04, e expressões com literal decimal têm resultado `int`.
6. **Sem checagem de tipos em declarações com valor inicial, condições, `char`, `string` e `void`**, conforme descrito nas seções anteriores.
7. **Estado final da execução.** No teste `float b = 2.5; ... b = a;` com `a` inteiro, o estado final do runtime mostrou `b` como `int`. Isso ocorre na execução, fora da análise semântica, e foi registrado aqui só como observação.

## Como conferir

Na raiz do projeto:

```bash
make rebuild
./interpreter testes/invalidos/08_var_nao_declarada.c; echo "código: $?"
./interpreter testes/invalidos/09_redeclaracao.c; echo "código: $?"
```

O primeiro comando deve mostrar SEM-01 e código `1`; o segundo, SEM-03 e código `0` (ver limitação 1).

Para ver a árvore que a análise percorre, use `./interpreter --print-ast arquivo.c`.

Para validar a documentação, rode `mkdocs serve` e abra **Análise Semântica** no menu.

## Veja também

- [Tabela de símbolos](tabela-simbolos.md)
- [Guia de uso](guia-uso.md)
- [Planejamento da Sprint 8](planejamento-sprints.md#sprint-8-analise-semantica-integrada-a-ast)