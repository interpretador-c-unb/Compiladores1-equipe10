# Interpretador, escopos e destruição de variáveis — Sprint 14

**Estado verificado em 27/11/2026.** Esta página documenta o código de `src/interpreter.c/h`, `src/semantic.c`, `src/symbol_table.c` e `src/ast.c/h`, incluindo as correções de escopo da Sprint 12. O runtime está implementado; a descrição conceitual anterior da Sprint 9 foi substituída pelo comportamento atual.

## Fluxo de execução

```text
Arquivo C → Flex/Bison → AST com tipos das declarações
                              ↓
                   Otimização opcional (-O)
                              ↓
              Análise semântica e SymbolTable
                              ↓ somente sem erros semânticos
               execute_program() e Environment
                              ↓
                Liberação do ambiente de runtime
                              ↓
                   Liberação da AST e tabela
```

`main.c` cria uma tabela vazia antes do parsing. O parser constrói a AST, preservando `declared_type` em cada declaração; a inserção dos símbolos ocorre depois, em `analyze_semantics()`. A AST é impressa somente com `--print-ast`.

A raiz é uma lista ligada por `next`. `execute_program()` cria o ambiente, percorre essa lista, imprime o estado final das variáveis restantes e chama `free_environment()`. A avaliação dos nós usa `evaluate_ast()`; não existe uma função chamada `evaluate_expr()`.

## Dois controles de escopo independentes

| Estrutura | Conteúdo | Quando é usada |
| --- | --- | --- |
| `SymbolTable` | Nome, tipo declarado e nível de escopo | Na análise semântica, para verificar declarações e referências. |
| `Environment` | Lista de `RuntimeVar`, com nome, `Value` e nível de escopo | Na execução, para guardar e atualizar valores. |

Ambas usam uma lista encadeada com as entradas mais recentes no início e um contador `current_scope`, iniciado em zero. Não há um objeto de ambiente separado para cada bloco nem uma cadeia de ponteiros para ambientes externos.

### Análise semântica

Ao visitar `NODE_DECLARATION`, a análise chama `insert_symbol()` com o nome e o tipo guardados na AST. Uma declaração repetida no mesmo nível conta como erro e impede a execução, com código de saída `1`. O mesmo nome em um bloco interno é permitido.

Ao visitar `NODE_BLOCK`, a análise chama `enter_scope()`, percorre os comandos e chama `exit_scope()`. As declarações locais ficam disponíveis durante a análise do bloco e são removidas ao sair. Referências e atribuições procuram o primeiro símbolo com o nome solicitado, respeitando o sombreamento.

A análise percorre também os comandos após um `if` e verifica expressões isoladas. Uso antes da declaração e uso depois de sair do bloco são rejeitados. Essa passada verifica os dois ramos de um `if`, independentemente de qual será executado.

### Execução

| Operação | Comportamento implementado |
| --- | --- |
| Entrada em `{}` | `env_enter_scope()` incrementa o nível. |
| Declaração | `declare_variable()` cria uma entrada no nível atual, mesmo que exista uma variável externa com o mesmo nome. |
| Consulta | `get_variable()` retorna o valor da primeira entrada com aquele nome. |
| Atribuição | `set_variable()` atualiza a primeira entrada com aquele nome; não cria uma declaração implícita. |
| Saída de `{}` | `env_exit_scope()` remove e libera as entradas do nível atual e decrementa o nível. |
| Fim do programa | `free_environment()` libera as entradas restantes e o próprio ambiente. |

Declaração e atribuição têm funções distintas: declarar uma variável interna não altera a externa. Uma atribuição sem declaração local atualiza a variável externa visível e essa alteração permanece após o bloco.

`if` e `while` não abrem escopo por si próprios: o escopo é aberto pelo corpo quando ele é um `NODE_BLOCK`. Em cada iteração de um `while` com chaves, as variáveis locais são criadas novamente e destruídas ao sair daquele bloco.

## Exemplo: sombreamento e restauração da visibilidade

```c
int x = 1;
{
    int x = 2;
    printf("%d,", x);
}
printf("%d", x);
```

A saída dos dois `printf()` é `2,1`, além dos registros do interpretador.

```text
Antes do bloco   nível 0   head → x=1 (nível 0)
Dentro do bloco  nível 1   head → x=2 (nível 1) → x=1 (nível 0)
Depois do bloco  nível 0   head → x=1 (nível 0)
```

O valor externo não precisa ser restaurado: sua entrada permaneceu intacta. Apenas a entrada local foi removida. Se o bloco contivesse `x = 2;` sem uma nova declaração, a entrada externa seria atualizada e o resultado final seria `2`.

## Propriedade da memória e destruição

| Memória | Responsável pela liberação |
| --- | --- |
| Nome e tipo copiados em `Symbol` | `exit_scope()` para símbolos locais; `free_symbol_table()` para os restantes. |
| Nome e entrada `RuntimeVar` | `env_exit_scope()` ao sair do bloco; `free_environment()` no encerramento normal. |
| Texto armazenado em `Value` de uma variável | As mesmas funções de saída; `set_variable()` também libera o texto anterior ao substituir o valor. |
| Nome, `declared_type`, literais e nós da AST | `free_ast()`, chamado pelo driver ao encerrar ou abortar após análise sintática/semântica. |

A declaração copia o nome e, para `VAL_STRING`, o texto para a entrada de runtime. O valor textual temporário usado como inicializador é liberado após essa cópia. `get_variable()` também devolve uma cópia quando o valor é textual; o consumidor deve liberá-la. `exec_printf()` libera os valores textuais avaliados para seus argumentos.

A destruição das entradas locais está implementada, mas isso **não comprova ausência de vazamentos em toda a execução**: valores temporários têm um ciclo de vida separado das entradas do ambiente.

## Limitações conhecidas

- **Funções e chamadas:** o parser não implementa declarações/chamadas de funções nem `return`. Não há escopo por função, pilha de chamadas ou destruição associada a retorno. O item de implementação da Sprint 14 que menciona blocos e funções não pode ser considerado integralmente concluído.
- **Temporários de texto:** `NODE_ASSIGNMENT` retorna o valor avaliado, mas `execute_list()` descarta esse retorno sem liberar textos. Há também caminhos de avaliação de condições e expressões sem liberação dos temporários textuais. A liberação das variáveis ao fechar o bloco não resolve esses caminhos; a validação de memória da Issue #61 permanece necessária.
- **Inicialização:** uma declaração sem inicializador recebe `VAL_INT` com valor zero no runtime, independentemente do tipo declarado. Não há controle de leitura antes da inicialização; autorreferência em inicializadores não está coberta por essa regra de validação de escopo.
- **Tipos:** `declared_type` é usado na análise semântica, mas o runtime não o usa para converter o valor armazenado. Literais decimais continuam sujeitos à classificação como inteiro, e `char` é representado como texto. Consulte as [limitações semânticas](analise-semantica.md#limitacoes-conhecidas).
- **Falhas de runtime:** não há propagação estruturada de erro até o driver nem desenrolamento de escopos por exceção. Divisão por zero atualmente produz zero; não deve ser descrita como exceção tratada. A limpeza documentada corresponde ao fluxo normal de execução.
- **Uso direto da API:** a execução depende da validação semântica prévia. `get_variable()` devolve `VAL_VOID` se o nome não existir; `set_variable()` não emite diagnóstico quando não encontra o destino. `env_exit_scope()` no nível zero remove as entradas globais; o avaliador só o chama após entrar em um bloco.

## Validação reproduzível

Na raiz do repositório:

```bash
make -C src
python3 testes/testar_escopos.py
```

A suíte específica contém 17 cenários, executados com e sem `-O`: **34 verificações**. Cobre sombreamento, atribuição externa, blocos aninhados e irmãos, variável local em `while`, comandos após `if`, texto local, bloco vazio, redeclarações e usos fora do escopo ou antes da declaração. Ela verifica saída e rejeição antes do runtime; não mede vazamentos de memória.

Para validar o site, com MkDocs e Material instalados:

```bash
mkdocs build --strict
```

## Entrega da Sprint 14

A documentação de escopos locais e destruição atribuída a Rafael Laube está registrada nesta página. A publicação no GitHub Pages, o guia de exceções e a validação integral de memória permanecem entregas separadas no [planejamento](planejamento-sprints.md).
