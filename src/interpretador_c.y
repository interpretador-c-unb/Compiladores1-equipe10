/* interpretador_c.y */

%{
#include <stdio.h>
#include <stdlib.h>

/* 
   Declarações explícitas para evitar warnings de 
   “implicit declaration of function yylex/yyerror”
*/
int yylex(void);
void yyerror(const char *s);

%}

%token NUM
%token SEMI
%token STRING
%token ID
%token EQ
%token QUOT

%union {
    int intValue;
    char *idValue;
    char *stringValue;
}

%type <intValue> NUM
%type <idValue> ID
%type <stringValue> STRING

%%
/* A gramática */
programa:
    atribuicao programa 
    | atribuicao
    ;


atribuicao:
    ID EQ NUM SEMI {printf("%s = %d\n", $1, $3 );} 
    | ID EQ QUOT STRING QUOT SEMI {printf("%s = \"%s\"\n", $1, $4 );}

%%

/* Definição de yyerror */
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}
