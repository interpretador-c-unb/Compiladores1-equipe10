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
make -C src clean && make -C src
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

Salve o exemplo abaixo em `soma.c`:

```c
int soma = 2 + 3;
printf("%d\n", soma);
```

Execute o programa:

```bash
./interpreter soma.c
```

Na saída, confira o `5` impresso antes do estado final:

```text
--- Executando o Programa (Runtime) ---
5

--- Estado Final das Variáveis no Ambiente de Runtime ---
soma (int) = 5
```

Para testar `if`, `while` e `printf()` juntos, rode o exemplo do repositório. Ele imprime `soma: 15`:

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
make -C src clean && make -C src
```

Para executar um caso individual:

```bash
./interpreter testes/validos/14_while_execucao.c
```

Para executar a suíte de casos válidos e inválidos:

```bash
bash testes/executar_testes.sh
```

Quando o script da Sprint 12 estiver integrado, a validação de memória será executada por:

```bash
chmod +x testes/valgrind/run_valgrind.sh
./testes/valgrind/run_valgrind.sh
```

## Documentação técnica

Consulte o [guia de uso](documentos/guia-uso.md), a [arquitetura](documentos/decisoes-tecnicas.md) e o [status do projeto](documentos/status-projeto.md). Para gerar a documentação com MkDocs Material:

```bash
sudo apt install python3 python3-venv
python3 -m venv .venv
source .venv/bin/activate
python -m pip install mkdocs mkdocs-material
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

**Universidade de Brasília | FCTE**

| Informações | Detalhes |
| :--- | :--- |
| **Disciplina** | Compiladores 1 |
| **Professor** | Dr. Sergio Antônio Andrade de Freitas |
| **Semestre** | 2º / 2026 |
| **Grupo** | 10 |

---

## Equipe

| Nome | Matrícula |
| :--- | :--- |
| Bernardo Campos Caxito Silveira | 232024966 |
| Danielly Mendes do Espírito Santo | 242015817 |
| Nicole Jovita Fernandes | 241012347 |
| Paulo Nina Denczuk de Alencar | 242004920 |
| Rafael Souto Lopes Laube | 211062428 |
