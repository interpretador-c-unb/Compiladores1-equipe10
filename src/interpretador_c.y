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
    atribuicao programa
    | atribuicao
    ;

atribuicao:
    ID ASSIGN NUM SEMI {
        printf("%s = %.15g\n", $1, $3);
        free($1);
    }
    | ID ASSIGN STRING SEMI {
        printf("%s = \"%s\"\n", $1, $3);
        free($1);
        free($3);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s na linha %d, coluna %d\n",
            s, yylineno, lex_column());
}
