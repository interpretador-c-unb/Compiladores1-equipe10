# Gramática Livre de Contexto (BNF) e Precedência

Nesta seção está documentada a estrutura sintática suportada pelo interpretador, escrita em notação **BNF (Backus-Naur Form)**, bem como a hierarquia de operadores configurada no Bison.

## 1. terminais (Tokens de Lexer)

Os seguintes símbolos terminais foram definidos no Flex e servem de base para a gramática:
* **Palavras Reservadas:** `if`, `else`, `while`, `for`, `return`, `int`, `char`, `float`, `void`
* **Literais e Identificadores:** `ID` (variáveis/funções), `NUM` (inteiros e flutuantes), `STRING` (textos), `CHARLIT` (caracteres isolados)
* **Operadores Relacionais e Lógicos:** `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`
* **Operadores Aritméticos e Atribuição:** `=`, `+`, `-`, `*`, `/`, `%`, `++`, `--`
* **Delimitadores:** `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`

---

## 2. Estrutura da Gramática (BNF)

A BNF abaixo foi preservada nesta revisão documental. Ela inclui construções planejadas que ainda não possuem regras em `src/interpretador_c.y`, como funções, chamadas, `for`, `return` e incrementos. O parser atual é a referência para a sintaxe efetivamente aceita.

```bnf
<programa> ::= <lista_declaracoes>

<lista_declaracoes> ::= <declaracao> <lista_declaracoes>
                      | <declaracao>

<declaracao> ::= <decl_variavel>
               | <decl_funcao>

<tipo> ::= "int" | "float" | "char" | "void"

<decl_variavel> ::= <tipo> ID ";"
                  | <tipo> ID "=" <expressao> ";"

<decl_funcao> ::= <tipo> ID "(" <parametros> ")" <bloco>

<parametros> ::= <lista_parametros>
               | "void"
               | <vazio>

<lista_parametros> ::= <parametro> "," <lista_parametros>
                     | <parametro>

<parametro> ::= <tipo> ID

<bloco> ::= "{" <lista_comandos> "}"

<lista_comandos> ::= <comando> <lista_comandos>
                   | <vazio>

<comando> ::= <comando_expr>
            | <comando_if>
            | <comando_while>
            | <comando_for>
            | <comando_return>
            | <bloco>
            | <decl_variavel>

<comando_expr> ::= <expressao> ";"
                 | ";"

<comando_if> ::= "if" "(" <expressao> ")" <comando>
               | "if" "(" <expressao> ")" <comando> "else" <comando>

<comando_while> ::= "while" "(" <expressao> ")" <comando>

<comando_for> ::= "for" "(" <expressao_opc> ";" <expressao_opc> ";" <expressao_opc> ")" <comando>

<comando_return> ::= "return" <expressao_opc> ";"

<expressao_opc> ::= <expressao>
                  | <vazio>

<expressao> ::= ID "=" <expressao>
              | <expressao_logica>

<expressao_logica> ::= <expressao_logica> "||" <expressao_and>
                     | <expressao_and>

<expressao_and> ::= <expressao_and> "&&" <expressao_igualdade>
                  | <expressao_igualdade>

<expressao_igualdade> ::= <expressao_igualdade> "==" <expressao_relacional>
                        | <expressao_igualdade> "!=" <expressao_relacional>
                        | <expressao_relacional>

<expressao_relacional> ::= <expressao_relacional> "<" <expressao_aditiva>
                         | <expressao_relacional> "<=" <expressao_aditiva>
                         | <expressao_relacional> ">" <expressao_aditiva>
                         | <expressao_relacional> ">=" <expressao_aditiva>
                         | <expressao_aditiva>

<expressao_aditiva> ::= <expressao_aditiva> "+" <expressao_multiplicativa>
                      | <expressao_aditiva> "-" <expressao_multiplicativa>
                      | <expressao_multiplicativa>

<expressao_multiplicativa> ::= <expressao_multiplicativa> "*" <expressao_unaria>
                             | <expressao_multiplicativa> "/" <expressao_unaria>
                             | <expressao_multiplicativa> "%" <expressao_unaria>
                             | <expressao_unaria>

<expressao_unaria> ::= "-" <expressao_unaria>
                     | "!" <expressao_unaria>
                     | "++" ID
                     | "--" ID
                     | ID "++"
                     | ID "--"
                     | <fator>

<fator> ::= "(" <expressao> ")"
          | ID
          | ID "(" <argumentos> ")"
          | NUM
          | STRING
          | CHARLIT

<argumentos> ::= <lista_argumentos>
               | <vazio>

<lista_argumentos> ::= <expressao> "," <lista_argumentos>
                     | <expressao>

```

## Precedência e Associatividade de Operadores

As declarações abaixo reproduzem os nomes dos tokens de `src/interpretador_c.y`. A prioridade aumenta de cima para baixo: multiplicação e divisão têm prioridade maior que adição e subtração.

| Categoria | Operadores | Associatividade | Declaração no Bison |
| --- | --- | --- | --- |
| Atribuição | `=` | Direita | `%right ASSIGN` |
| Lógico OU | `\|\|` | Esquerda | `%left OROR` |
| Lógico E | `&&` | Esquerda | `%left ANDAND` |
| Igualdade | `==`, `!=` | Esquerda | `%left EQ NE` |
| Relacionais | `<`, `<=`, `>`, `>=` | Esquerda | `%left LT LE GT GE` |
| Aditivos | `+`, `-` | Esquerda | `%left PLUS MINUS` |
| Multiplicativos | `*`, `/`, `%` | Esquerda | `%left STAR SLASH PERCENT` |
| Unários | `!`, `-` unário; tokens `++`, `--` | Direita | `%right NOT UMINUS PLUSPLUS MINUSMINUS` |

- `%left` agrupa operadores de mesma prioridade da esquerda para a direita: `10 - 3 - 2` equivale a `(10 - 3) - 2`.
- `%right` agrupa da direita para a esquerda.
- `%nonassoc` impede encadeamentos sem parênteses entre operadores de mesma precedência.
- `%prec` atribui a uma produção uma precedência explícita. O menos unário usa `%prec UMINUS`.

A declaração de precedência, sozinha, não cria uma regra sintática: embora `ASSIGN` seja declarado com `%right`, o parser atual aceita atribuição como comando (`a = 5;`), mas não aceita `a = b = 5;`. Os tokens `PLUSPLUS` e `MINUSMINUS` também não têm produções de expressão implementadas.

### Declarações no Bison

```bison
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGN
%left OROR
%left ANDAND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR SLASH PERCENT
%right NOT UMINUS PLUSPLUS MINUSMINUS
```

`LOWER_THAN_ELSE` e `ELSE` são níveis usados para resolver a ambiguidade do `else`. A produção de `if` sem `else` recebe `%prec LOWER_THAN_ELSE`; como `ELSE` tem prioridade maior, o `else` se associa ao `if` mais próximo ainda sem `else`.

### Exemplos de agrupamento

| Expressão | Agrupamento |
| --- | --- |
| `2 + 3 * 4` | `2 + (3 * 4)` |
| `10 - 3 - 2` | `(10 - 3) - 2` |
| `-2 * 3` | `(-2) * 3` |
| `(2 + 3) * 4` | Os parênteses fazem a soma ser agrupada primeiro. |

Consulte o [guia de uso](guia-uso.md) para compilar o driver e visualizar a AST dos exemplos.
