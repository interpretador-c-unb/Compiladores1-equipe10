/* interpretador_c.y */

%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
int lex_column(void);
extern int yylineno;
%}

%union {
    double numValue;
    int charValue;
    char *idValue;
    char *stringValue;
}

%token <numValue> NUM
%token <stringValue> STRING
%token <idValue> ID
%token <charValue> CHARLIT

%token IF ELSE WHILE FOR RETURN
%token INT CHAR FLOAT VOID

%token ASSIGN
%token PLUS MINUS STAR SLASH PERCENT
%token EQ NE LT GT LE GE
%token ANDAND OROR NOT
%token PLUSPLUS MINUSMINUS

%token LPAREN RPAREN
%token LBRACE RBRACE
%token LBRACK RBRACK
%token SEMI COMMA

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

%destructor { free($$); } ID STRING

%%

programa:
    lista_declaracoes
    ;

lista_declaracoes:
    lista_declaracoes elemento
    | elemento
    ;

elemento:
    declaracao_variavel
    | comando
    ;

tipo:
    INT
    | FLOAT
    | CHAR
    | VOID
    ;

declaracao_variavel:
    tipo ID SEMI {
        printf("[DECLARAÇÃO] Variavel '%s'\n", $2);
        free($2);
    }
    | tipo ID ASSIGN expressao SEMI {
        printf("[DECLARAÇÃO COM INICIALIZAÇÃO] Variavel '%s'\n", $2);
        free($2);
    }
    ;

comando:
    atribuicao
    | expressao SEMI
    ;

atribuicao:
    ID ASSIGN expressao SEMI {
        printf("[ATRIBUIÇÃO] Variavel '%s'\n", $1);
        free($1);
    }
    ;

expressao:
    ID {
        free($1);
    }
    | NUM
    | STRING {
        free($1);
    }
    | CHARLIT
    | expressao PLUS expressao
    | expressao MINUS expressao
    | expressao STAR expressao
    | expressao SLASH expressao
    | expressao PERCENT expressao
    | expressao EQ expressao
    | expressao NE expressao
    | expressao LT expressao
    | expressao LE expressao
    | expressao GT expressao
    | expressao GE expressao
    | expressao ANDAND expressao
    | expressao OROR expressao
    | NOT expressao
    | MINUS expressao %prec UMINUS
    | LPAREN expressao RPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s na linha %d, coluna %d\n",
            s, yylineno, lex_column());
}
