# Guia de Uso

O driver `src/main.c` recebe um arquivo-fonte, aciona a análise léxica e sintática e imprime a Árvore Sintática Abstrata (AST). Nesta versão, ele constrói e exibe a árvore; ainda não executa os comandos do programa C.

## Compilação

É necessário ter GCC, Flex, Bison e Make instalados. Execute os comandos abaixo na raiz do repositório:

```bash
make -C src
```

O Bison gera o parser e seu cabeçalho, o Flex gera o scanner e o GCC compila esses arquivos junto com `main.c`, `ast.c` e `symbol_table.c`. O executável resultante é `./interpreter`, na raiz do projeto.

Use `make -C src`: o `Makefile` da raiz atualmente procura os fontes no diretório errado. Para uma compilação limpa:

```bash
make -C src clean
make -C src
```

## Execução e argumentos

```bash
./interpreter caminho/para/arquivo.c
```

O primeiro argumento é o caminho do arquivo de entrada, relativo ao diretório atual ou absoluto. Por exemplo, a partir da raiz:

```bash
./interpreter testes/validos/02_declaracoes_e_expressoes.c
```

Não é necessário envolver as declarações e comandos em uma função `main()` no arquivo de entrada. O driver atual não implementa opções como `--tokens`.

## Fluxo do driver

1. Verifica se foi informado um arquivo e o abre para leitura, atribuindo-o a `yyin`.
2. Cria a tabela de símbolos com `create_symbol_table()`.
3. Chama `yyparse()`. O parser solicita tokens ao scanner e constrói a AST, cuja raiz fica em `ast_root`.
4. Fecha o arquivo e informa sucesso ou falha conforme o retorno de `yyparse()`.
5. Chama `print_ast(ast_root)` para exibir a árvore.
6. Libera a AST e a tabela de símbolos e retorna o resultado do parser ao sistema operacional.

Quando a análise sintática termina com sucesso, a saída inclui:

```text
[SUCESSO] Análise sintática e construção da AST/Tabela concluídas com sucesso!
```

Também podem aparecer registros de inserção na tabela de símbolos e, em seguida, a árvore indentada.

## Mensagens e códigos de saída

| Situação | Mensagem ou comportamento |
| --- | --- |
| Nenhum argumento | `Uso: ./interpreter <arquivo_fonte.c>`; retorna `1`. |
| Arquivo inexistente ou sem permissão de leitura | `Erro ao abrir arquivo de entrada: ...`; o detalhe vem do sistema operacional; retorna `1`. |
| Erro sintático | `Erro sintático: ... na linha N, coluna C`, seguido de `[FALHA] Foram encontrados erros no código.`; retorna o código não zero de `yyparse()`. |
| Variável não declarada | `[AVISO SEMÂNTICO] Variável 'nome' usada sem declaração na linha N`. |
| Redeclaração no mesmo escopo | `[ERRO SEMÂNTICO] Variável 'nome' já declarada no escopo N`. |

Os diagnósticos de abertura de arquivo e de análise são enviados a `stderr`; as mensagens de uso, sucesso/falha e a AST são enviadas a `stdout`.

O código de saída reflete o parser: avisos semânticos e alguns erros léxicos podem coexistir com retorno `0` e mensagem de sucesso. Portanto, nesta versão, também é necessário conferir os diagnósticos em `stderr`.

Para experimentar um erro sintático:

```bash
./interpreter testes/invalidos/01_sem_ponto_virgula.c
```

### Erros durante a compilação do interpretador

Se `make -C src` indicar que `gcc`, `flex` ou `bison` não foi encontrado, instale a ferramenta indicada e confirme que está no `PATH`. Erros de geração do parser/scanner ou de compilação C aparecem na saída dessas ferramentas e impedem a conclusão do build. Eles são distintos dos erros encontrados ao analisar um arquivo com `./interpreter`.

## Testes do front-end

Após compilar, execute na raiz:

```bash
bash testes/executar_testes.sh
```

A suíte compara as saídas dos programas válidos com os arquivos `.exp` e verifica os diagnósticos ou códigos de erro dos programas inválidos.

## Validação da documentação

Em um ambiente Python com as dependências do site instaladas:

```bash
python -m pip install mkdocs mkdocs-material
mkdocs serve
```

Abra `http://127.0.0.1:8000` e confira as páginas **Gramática & BNF** e **Guia de uso**, disponíveis no menu do MkDocs. Verifique a tabela de precedência e os blocos de código. Para gerar o site sem iniciar o servidor:

```bash
mkdocs build
```

Veja também a [gramática e a precedência dos operadores](gramatica-bnf.md) e o [status da Sprint 3](planejamento-sprints.md#sprint-3-analise-sintatica-basica).
