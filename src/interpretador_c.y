/* interpretador_c.y */

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

int yylex(void);
void yyerror(const char *s);
int lex_column(void);
extern int yylineno;
extern char *yytext;

ASTNode *ast_root = NULL;
SymbolTable *sym_table = NULL;
%}

%define parse.error verbose

%union {
    double numValue;
    int charValue;
    char *idValue;
    char *stringValue;
    ASTNode *astNode;
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

%type <astNode> programa lista_declaracoes elemento declaracao_variavel comando atribuicao comando_if comando_while bloco lista_comandos expressao
%type <stringValue> tipo

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
    lista_declaracoes {
        ast_root = $1;
    }
    ;

lista_declaracoes:
    lista_declaracoes elemento {
        $$ = append_node($1, $2);
    }
    | elemento {
        $$ = $1;
    }
    ;

elemento:
    declaracao_variavel
    | comando
    ;

tipo:
    INT { $$ = "int"; }
    | FLOAT { $$ = "float"; }
    | CHAR { $$ = "char"; }
    | VOID { $$ = "void"; }
    ;

declaracao_variavel:
    tipo ID SEMI {
        insert_symbol(sym_table, $2, $1);
        $$ = create_decl_node($2, NULL);
        free($2);
    }
    | tipo ID ASSIGN expressao SEMI {
        insert_symbol(sym_table, $2, $1);
        $$ = create_decl_node($2, $4);
        free($2);
    }
    ;

comando:
    atribuicao
    | comando_if
    | comando_while
    | bloco
    | expressao SEMI { $$ = $1; }
    | SEMI { $$ = NULL; }
    ;

atribuicao:
    ID ASSIGN expressao SEMI {
        if (!lookup_symbol(sym_table, $1)) {
            fprintf(stderr, "[AVISO SEMÂNTICO] Variável '%s' usada sem declaração na linha %d\n", $1, yylineno);
        }
        $$ = create_assign_node($1, $3);
        free($1);
    }
    ;

comando_if:
    IF LPAREN expressao RPAREN comando %prec LOWER_THAN_ELSE {
        $$ = create_if_node($3, $5, NULL);
    }
    | IF LPAREN expressao RPAREN comando ELSE comando {
        $$ = create_if_node($3, $5, $7);
    }
    ;

comando_while:
    WHILE LPAREN expressao RPAREN comando {
        $$ = create_while_node($3, $5);
    }
    ;

bloco:
    LBRACE { enter_scope(sym_table); } RBRACE {
        exit_scope(sym_table);
        $$ = create_node(NODE_BLOCK);
    }
    | LBRACE { enter_scope(sym_table); } lista_comandos RBRACE {
        exit_scope(sym_table);
        ASTNode *block = create_node(NODE_BLOCK);
        block->left = $3;
        $$ = block;
    }
    ;

lista_comandos:
    lista_comandos elemento {
        $$ = append_node($1, $2);
    }
    | elemento {
        $$ = $1;
    }
    ;

expressao:
    ID {
        if (!lookup_symbol(sym_table, $1)) {
            fprintf(stderr, "[AVISO SEMÂNTICO] Variável '%s' usada sem declaração na linha %d\n", $1, yylineno);
        }
        $$ = create_var_node($1);
        free($1);
    }
    | NUM { $$ = create_num_node($1); }
    | STRING {
        $$ = create_string_node($1);
        free($1);
    }
    | CHARLIT {
        char buf[2] = {(char)$1, '\0'};
        $$ = create_string_node(buf);
    }
    | expressao PLUS expressao { $$ = create_binary_node("+", $1, $3); }
    | expressao MINUS expressao { $$ = create_binary_node("-", $1, $3); }
    | expressao STAR expressao { $$ = create_binary_node("*", $1, $3); }
    | expressao SLASH expressao { $$ = create_binary_node("/", $1, $3); }
    | expressao PERCENT expressao { $$ = create_binary_node("%", $1, $3); }
    | expressao EQ expressao { $$ = create_binary_node("==", $1, $3); }
    | expressao NE expressao { $$ = create_binary_node("!=", $1, $3); }
    | expressao LT expressao { $$ = create_binary_node("<", $1, $3); }
    | expressao LE expressao { $$ = create_binary_node("<=", $1, $3); }
    | expressao GT expressao { $$ = create_binary_node(">", $1, $3); }
    | expressao GE expressao { $$ = create_binary_node(">=", $1, $3); }
    | expressao ANDAND expressao { $$ = create_binary_node("&&", $1, $3); }
    | expressao OROR expressao { $$ = create_binary_node("||", $1, $3); }
    | NOT expressao { $$ = create_binary_node("!", $2, NULL); }
    | MINUS expressao %prec UMINUS { $$ = create_binary_node("-", $2, NULL); }
    | LPAREN expressao RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s na linha %d, coluna %d\n",
            s, yylineno, lex_column());
}
