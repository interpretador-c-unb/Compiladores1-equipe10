# Interpretador C — Equipe 10

Projeto de um interpretador para um subconjunto da linguagem C, desenvolvido com Flex, Bison e GCC. O pipeline realiza análise léxica, análise sintática, construção da AST, análise semântica, otimização por constant folding e execução do runtime.

## Requisitos

Em Linux, Debian, Ubuntu ou WSL, instale:

```bash
sudo apt update
sudo apt install build-essential flex bison make
```

Para a validação de memória da Sprint 12, instale também:

```bash
sudo apt install valgrind
```

No Windows, o build recomendado é feito no WSL ou em outro ambiente Unix, pois o Makefile usa ferramentas e comandos compatíveis com esse ambiente.

## Obter o projeto

```bash
git clone https://github.com/interpretador-c-unb/Compiladores1-equipe10.git
cd Compiladores1-equipe10
```

## Compilar

O build oficial é executado pelo Makefile de `src/`:

```bash
make -C src rebuild
```

O comando gera o parser com Bison, o lexer com Flex e o executável:

```text
./interpreter
```

Para limpar os artefatos gerados:

```bash
make -C src clean
```

## Executar um programa

O interpretador recebe um arquivo `.c` como argumento:

```bash
./interpreter testes/validos/17_if_while_printf.c
```

Opções disponíveis:

```bash
./interpreter --print-ast testes/validos/expressoes_constantes.c
./interpreter --optimize testes/validos/expressoes_constantes.c
./interpreter --optimize --print-ast testes/validos/expressoes_constantes.c
```

`--print-ast` imprime a AST antes da análise semântica. `--optimize` aplica constant folding antes da impressão ou execução. Os arquivos de entrada usam o subconjunto de C definido pela gramática do projeto e não precisam conter uma função `main()`.

## Funcionalidades disponíveis

A versão documentada suporta declarações de `int`, `float` e `char`, inicialização e atribuição, literais numéricos, strings e caracteres, operadores aritméticos, relacionais e lógicos, precedência definida no Bison, `if`, `else`, `while`, blocos com escopo, `printf()`, análise semântica, constant folding e execução da AST em um ambiente de runtime.

Funções, chamadas de função e `return` não fazem parte do escopo final documentado desta versão.

## Executar os testes

Recompile antes da execução:

```bash
make -C src rebuild
```

Para executar um caso individual:

```bash
./interpreter testes/validos/14_while_execucao.c
```

Para executar a suíte de casos válidos:

```bash
printf 'testes/validos\n' | bash testes/rodar_testes.sh
```

A suíte de testes léxicos possui um script próprio:

```bash
bash testes/1_lexico/run.sh
```

Quando o script da Sprint 12 estiver integrado, a validação de memória será executada por:

```bash
chmod +x testes/valgrind/run_valgrind.sh
./testes/valgrind/run_valgrind.sh
```

## Documentação técnica

A documentação está em `documentos/` e pode ser gerada localmente com MkDocs Material:

```bash
python3 -m pip install mkdocs mkdocs-material
mkdocs serve
```

Acesse `http://127.0.0.1:8000/`. Antes de abrir um PR documental, valide a geração estrita:

```bash
mkdocs build --strict
```

As páginas principais são a estrutura do projeto, o guia de uso, a gramática e BNF, a análise semântica, o interpretador e runtime, o status do projeto e o planejamento das Sprints.

## Publicação

A publicação é feita pelo workflow `.github/workflows/deploy-docs.yml` quando uma alteração é integrada à `main`. O workflow instala MkDocs Material e executa `mkdocs gh-deploy --force`, atualizando a branch `gh-pages`.

A publicação está disponível em:

<https://interpretador-c-unb.github.io/Compiladores1-equipe10/>

## Equipe

- Bernardo Campos Caxito Silveira
- Danielly Mendes do Espírito Santo
- Nicole Jovita Fernandes
- Paulo Nina Denczuk de Alencar
- Rafael Souto Lopes Laube
