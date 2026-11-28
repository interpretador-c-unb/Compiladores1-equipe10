# Guia de Uso

Este guia descreve como preparar o ambiente, compilar o interpretador, executar um programa do subconjunto de C e validar a documentação.

## Pré-requisitos

São necessários GCC, Make, Flex e Bison. Para gerar o site, também são necessários Python 3, MkDocs e MkDocs Material. Para a validação de memória, instale Valgrind.

Em Ubuntu ou WSL:

```bash
sudo apt update
sudo apt install build-essential flex bison make
# Para gerar a documentação, instale também:
sudo apt install python3 python3-venv
python3 -m venv .venv
source .venv/bin/activate
python -m pip install mkdocs mkdocs-material
```

## Build

Execute a partir da raiz:

```bash
make -C src clean && make -C src
```

O Makefile remove os artefatos anteriores, executa Bison e Flex e compila todos os módulos do interpretador. O executável é criado na raiz como `./interpreter`.

Para remover os artefatos:

```bash
make -C src clean
```

## Execução

```bash
./interpreter testes/validos/17_if_while_printf.c
```

As opções de diagnóstico são:

```bash
./interpreter --print-ast testes/validos/expressoes_constantes.c
./interpreter --optimize --print-ast testes/validos/expressoes_constantes.c
```

O programa de entrada não precisa conter `main()`. Ele é escrito conforme a gramática do subconjunto implementado.

## Fluxo de execução

1. `main.c` interpreta as opções e abre o arquivo de entrada.
2. Bison e Flex realizam as análises léxica e sintática.
3. O parser constrói a AST e a tabela de símbolos.
4. A análise semântica verifica declarações, escopos e tipos aceitos.
5. O otimizador pode aplicar constant folding com `--optimize`.
6. `interpreter.c` avalia a AST no ambiente de runtime.
7. A AST, a tabela de símbolos e o ambiente são liberados ao final.

## Casos de execução

```bash
./interpreter testes/validos/14_while_execucao.c
./interpreter testes/validos/15_if_ramos.c
./interpreter testes/validos/16_printf.c
./interpreter testes/validos/17_if_while_printf.c
```

Esses casos cobrem laços, ramos condicionais, `printf()` e integração entre controle de fluxo e execução.

## Testes

Para executar a suíte de casos válidos:

```bash
printf 'testes/validos\n' | bash testes/rodar_testes.sh
```

Para executar os testes léxicos:

```bash
bash testes/1_lexico/run.sh
```

A validação de memória usa o script da Sprint 12, quando ele estiver integrado:

```bash
./testes/valgrind/run_valgrind.sh
```

O script deve terminar sem vazamentos `definitely lost` ou `indirectly lost`, acessos inválidos e uso de valores não inicializados.

## Mensagens e códigos de saída

A ausência de argumento gera uma mensagem de uso e código diferente de zero. Arquivo inexistente também interrompe a execução. Erros sintáticos e semânticos são informados pelo interpretador e impedem a execução quando detectados pela versão integrada.

A saída final do runtime apresenta os valores mantidos no ambiente, por exemplo:

```text
--- Estado Final das Variáveis no Ambiente de Runtime ---
x (int) = 5
```

## MkDocs

Para revisar o site localmente:

```bash
mkdocs serve
```

Para validar links, navegação e geração sem avisos:

```bash
mkdocs build --strict
```

Depois da integração na `main`, o workflow de publicação executa `mkdocs gh-deploy --force` e atualiza a branch `gh-pages`.
