# Interpretador e ambiente de execução

Depois do parsing, `main.c` pode otimizar e imprimir a AST. A análise semântica percorre a árvore e constrói a tabela de símbolos. Se a análise tiver sucesso, `execute_program()`, em `src/interpreter.c`, executa a lista de comandos e devolve seu código de saída ao driver.

| Estrutura | Responsabilidade |
|---|---|
| AST | Representa o programa. As declarações preservam o tipo informado e os literais numéricos preservam o texto original. |
| Tabela de símbolos | Registra declarações e escopos durante a análise semântica. |
| `Environment` | Mantém os valores e os níveis de escopo usados pela execução. |

O avaliador percorre listas por `next` e avalia expressões recursivamente. `if` executa o ramo selecionado e `while` reavalia a condição a cada iteração. Em `&&` e `||`, o segundo operando só é avaliado quando necessário.

## Escopos e valores

Cada bloco abre um nível no ambiente. Uma declaração local cria uma variável nesse nível, mesmo quando existe um nome igual em um bloco externo. Atribuições atualizam a variável visível mais próxima. Ao sair do bloco, o ambiente remove as variáveis daquele nível e volta ao anterior.

O runtime distingue valores inteiros, decimais e textuais. Literais de caractere são representados como texto. O tipo armazenado acompanha o valor atribuído e pode divergir do tipo registrado na tabela de símbolos.

## Erros de execução

Uma divisão ou resto por zero no caminho executado gera diagnóstico em `stderr`. O runtime interrompe a lista de comandos, libera o ambiente e retorna `1`. O driver também libera a AST e a tabela de símbolos. Por exemplo:

```bash
./interpreter testes/invalidos/20_divisao_por_zero.c
```

Veja o [guia de uso](guia-uso.md) e os [resultados da suíte](status-projeto.md#5-resultados-dos-testes).
