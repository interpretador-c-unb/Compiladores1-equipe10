## Decisões a serem feitas

1. Quais tokens de `c`devemos implementar.
2. Quais comandos? "if, else, switch, printf, etc"

## Sugestões do grok para tokens

Lista curta, no nível de um lexer didático (Flex):

## Palavras reservadas
- `IF` `ELSE` `WHILE` `FOR` `RETURN`
- `INT` `CHAR` `FLOAT` `VOID` `DOUBLE`
- `STRUCT` `TYPEDEF` `CONST`

## Identificadores e literais
- `ID` — nome de variável/função (`x`, `soma`, `_tmp`)
- `NUM` — número (`42`, `-3`, `3.14`)
- `STRING` — `"texto"`
- `CHARLIT` — `'a'`

## Operadores
- `ASSIGN` `=`
- `PLUS` `MINUS` `STAR` `SLASH` `PERCENT` → `+ - * / %`
- `EQ` `NE` `LT` `GT` `LE` `GE` → `== != < > <= >=`
- `ANDAND` `OROR` `NOT` → `&& || !`
- `PLUSPLUS` `MINUSMINUS` → `++ --`

## Ponteiro e endereço
- `STAR` `*` — declara ponteiro **e** desreferencia (`int *p`, `*p`)
- `AMP` `&` — endereço (`&x`)
- `ARROW` `->` — membro via ponteiro (`p->x`)
- `DOT` `.` — membro de struct (`s.x`)

## Delimitadores
- `LPAREN` `RPAREN` `( )`
- `LBRACE` `RBRACE` `{ }`
- `LBRACK` `RBRACK` `[ ]`
- `SEMI` `;`
- `COMMA` `,`

## Declaração de variável (não é 1 token)
É sequência, por exemplo:

`INT` `STAR`? `ID` (`ASSIGN` expressão)? `SEMI`

`int *p = &x;` → `INT` `STAR` `ID` `ASSIGN` `AMP` `ID` `SEMI`

Isso já dá para um parser mínimo de C.