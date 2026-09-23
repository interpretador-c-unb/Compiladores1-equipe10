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

Abaixo está a gramática que mapeia a sintaxe do nosso subconjunto da linguagem C.

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