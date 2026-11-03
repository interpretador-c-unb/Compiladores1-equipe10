# Analisador Sintático

Esta página documenta a implementação de `src/interpretador_c.y`: a resolução do *Dangling Else* e a emissão de erros sintáticos. O scanner está em `src/interpretador_c.l` e o driver em `src/main.c`.

## O problema do Dangling Else

Quando dois comandos `if` estão aninhados sem chaves, a gramática precisa definir a qual deles pertence o `else`:

```c
int a = 1;
int b = 0;
int x = 0;
if (a)
    if (b)
        x = 1;
    else
        x = 2;
```

A indentação ajuda a leitura, mas não determina a estrutura sintática. Sem uma regra de desambiguação, o `else` poderia pertencer ao `if (a)` ou ao `if (b)`. O projeto adota a convenção de associá-lo ao `if` mais próximo ainda sem `else`: neste exemplo, `if (b)`.

No Bison, essa escolha aparece como uma decisão entre reduzir o `if` interno sem `else` ou consumir o token `ELSE` para completar esse mesmo `if` (conflito *shift/reduce*).

## Como o código resolve a ambiguidade

O parser declara dois níveis de precedência, nesta ordem:

```bison
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
```

As declarações posteriores têm prioridade maior. `LOWER_THAN_ELSE` é um marcador de precedência; ele não corresponde a texto reconhecido pelo scanner. A regra sem `else` recebe esse nível explicitamente com `%prec`:

```bison
comando_if:
    IF LPAREN expressao RPAREN comando %prec LOWER_THAN_ELSE {
        $$ = create_if_node($3, $5, NULL);
    }
    | IF LPAREN expressao RPAREN comando ELSE comando {
        $$ = create_if_node($3, $5, $7);
    }
    ;
```

Quando o próximo token é `ELSE`, sua precedência é maior que a da regra sem `else`. O Bison prefere consumir o token (*shift*) a encerrar essa regra (*reduce*). Assim, o `else` completa o `if` interno.

`%nonassoc` declara níveis sem associatividade; neste caso, o que resolve o conflito é a diferença de precedência entre os dois níveis, junto com `%prec`. Não se trata de proibir comandos `if` aninhados.

As ações semânticas constroem a AST com `create_if_node`: a primeira passa `NULL` como ramo alternativo; a segunda passa o comando após `ELSE`. Na AST atual, a condição fica em `left`, o ramo principal em `right` e o alternativo em `else_branch`. O campo `next` encadeia comandos seguintes.

Para associar o `else` ao `if` externo, o programa pode delimitar o bloco interno com chaves:

```c
if (a) {
    if (b)
        x = 1;
} else {
    x = 2;
}
```

## Emissão de erros sintáticos

O parser habilita mensagens detalhadas do Bison:

```bison
%define parse.error verbose
```

Quando encontra um token incompatível com a regra em análise, o Bison chama `yyerror()` e fornece a descrição do erro. A mensagem pode indicar o token inesperado e, conforme o estado do parser, os tokens esperados. Nem todo diagnóstico contém uma lista de tokens esperados.

A implementação acrescenta a posição informada pelo scanner e escreve em `stderr`:

```c
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s na linha %d, coluna %d\n",
            s, yylineno, lex_column());
}
```

| Parte | Origem e significado |
| --- | --- |
| `s` | Descrição gerada pelo Bison, como `syntax error, unexpected IF`. |
| `yylineno` | Contador de linhas habilitado no Flex por `%option yylineno`. |
| `lex_column()` | Retorna `yystartcol`, a coluna inicial do último lexema reconhecido. |

O scanner chama `update_location()` por meio de `YY_USER_ACTION`. Essa função guarda a coluna inicial do lexema, avança a coluna pelos caracteres reconhecidos e a reinicia em `1` após uma quebra de linha.

A posição é a observada pelo scanner quando o erro é detectado. Ela pode apontar para o token seguinte ao elemento que faltou. Em fim de arquivo, o código não calcula uma posição de EOF independente: usa os contadores e a coluna disponíveis no scanner.

### Exemplo: ponto e vírgula ausente

O arquivo `testes/invalidos/01_sem_ponto_virgula.c` começa com:

```c
int x = 10
if (x > 0) {
    x = 5;
}
```

Falta `;` ao final da primeira linha. O erro é detectado quando o parser encontra `if` na linha seguinte. Ao executar na raiz do projeto:

```bash
make -C src
./interpreter testes/invalidos/01_sem_ponto_virgula.c
```

O diagnóstico observado é:

```text
Erro sintático: syntax error, unexpected IF na linha 2, coluna 1
```

O driver também imprime em `stdout`:

```text
[FALHA] Foram encontrados erros no código.
```

O texto interno do diagnóstico pode variar com a versão do Bison. Nesse caso, `yyparse()` retorna `1`, e `main()` repassa esse resultado ao sistema operacional.

## Recuperação: comportamento atual

A gramática não possui produções com o token especial `error`, nem pontos de sincronização para retomar a análise em um `;` ou `}`. Portanto, o tratamento atual relata o erro e encerra a análise sintática com falha; ele não acumula vários erros sintáticos do mesmo programa nem corrige automaticamente a entrada.

`yyerror()` apenas emite o diagnóstico. Após o retorno de `yyparse()`, o driver fecha o arquivo, informa sucesso ou falha, chama `print_ast(ast_root)` e libera a raiz da AST e a tabela de símbolos disponíveis. Isso não constitui recuperação sintática.

Os avisos de variável não declarada e os erros de redeclaração são verificações semânticas distintas. Eles não passam por `yyerror()` e, na implementação atual, podem ocorrer mesmo quando `yyparse()` retorna `0`. Consulte o [guia de uso](guia-uso.md) para os códigos de saída e demais mensagens.

## Como validar

1. Compile com `make -C src` e execute o exemplo sem ponto e vírgula acima.
2. Salve o exemplo de `if` aninhado sem chaves em um arquivo e execute `./interpreter caminho/do/arquivo.c`. Na AST impressa, confira que o ramo `else` pertence ao `if (b)`.
3. Execute `bash testes/executar_testes.sh` para conferir os casos válidos e inválidos existentes.
4. Execute `mkdocs serve` e abra `http://127.0.0.1:8000`. No menu **Analisador Sintático**, confira exemplos, tabela e links.

Veja também a [tabela de precedência dos operadores](gramatica-bnf.md#precedencia-e-associatividade-de-operadores).
