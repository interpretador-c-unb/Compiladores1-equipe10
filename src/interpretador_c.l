%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpretador_c.tab.h"

/* Posição do lexema que acabou de casar. O parser usa a coluna no yyerror. */
static int yycolumn = 1;
static int yystartcol = 1;
static int comment_line = 1;
static int comment_col = 1;
static int lex_errors = 0;

#define YY_USER_ACTION update_location();

static void update_location(void);
static void lex_error(const char *msg);
static void *xmalloc(size_t n);
static char *dup_text(void);
static int escaped_char(char c);
static char *unescape(const char *s, int n);

int lex_column(void);
int yyparse(void);
%}

%option noyywrap
%option nounput
%option noinput
%option nodefault
%option yylineno
%x COMMENT

DIGIT   [0-9]
LETTER  [A-Za-z_]

%%

[ \t\r\n]+                      { /* ignora espaço em branco */ }
"//".*                          { /* comentário de linha */ }
"/*"                            {
                                    comment_line = yylineno;
                                    comment_col = yystartcol;
                                    BEGIN(COMMENT);
                                }
<COMMENT>"*/"                   { BEGIN(INITIAL); }
<COMMENT>\n                     { }
<COMMENT>.                      { }
<COMMENT><<EOF>>                {
                                    fprintf(stderr,
                                            "Erro léxico: comentário não terminado na linha %d, coluna %d\n",
                                            comment_line, comment_col);
                                    lex_errors++;
                                    return 0;
                                }

"if"                            { return IF; }
"else"                          { return ELSE; }
"while"                         { return WHILE; }
"for"                           { return FOR; }
"return"                        { return RETURN; }
"int"                           { return INT; }
"char"                          { return CHAR; }
"float"                         { return FLOAT; }
"void"                          { return VOID; }

{LETTER}({LETTER}|{DIGIT})*     {
                                    yylval.idValue = dup_text();
                                    return ID;
                                }

{DIGIT}+(\.{DIGIT}+)?           {
                                    /* O sinal de -3 fica no token MINUS, para a-3 ser subtração. */
                                    yylval.numValue = strtod(yytext, NULL);
                                    return NUM;
                                }

\"([^\"\\\n]|\\.)*\"            {
                                    yylval.stringValue = unescape(yytext + 1, yyleng - 2);
                                    return STRING;
                                }
\"([^\"\\\n]|\\.)*              {
                                    lex_error("string não terminada");
                                    return '"';
                                }

'([^'\\\n]|\\.)'                {
                                    if (yytext[1] == '\\')
                                        yylval.charValue = escaped_char(yytext[2]);
                                    else
                                        yylval.charValue = (unsigned char)yytext[1];
                                    return CHARLIT;
                                }
'([^'\\\n]|\\.)*'               {
                                    lex_error("literal de caractere inválido");
                                    return '\'';
                                }
'([^'\\\n]|\\.)*                {
                                    lex_error("literal de caractere não terminado");
                                    return '\'';
                                }

"=="                            { return EQ; }
"!="                            { return NE; }
"<="                            { return LE; }
">="                            { return GE; }
"&&"                            { return ANDAND; }
"||"                            { return OROR; }
"++"                            { return PLUSPLUS; }
"--"                            { return MINUSMINUS; }

"="                             { return ASSIGN; }
"+"                             { return PLUS; }
"-"                             { return MINUS; }
"*"                             { return STAR; }
"/"                             { return SLASH; }
"%"                             { return PERCENT; }
"<"                             { return LT; }
">"                             { return GT; }
"!"                             { return NOT; }

"("                             { return LPAREN; }
")"                             { return RPAREN; }
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
"["                             { return LBRACK; }
"]"                             { return RBRACK; }
";"                             { return SEMI; }
","                             { return COMMA; }

.                               {
                                    unsigned char c = (unsigned char)yytext[0];
                                    char msg[80];
                                    if (c >= 32 && c < 127)
                                        snprintf(msg, sizeof msg, "caractere inválido '%c'", c);
                                    else
                                        snprintf(msg, sizeof msg, "caractere inválido 0x%02x", c);
                                    lex_error(msg);
                                    return (int)c;
                                }

%%

int lex_column(void) {
    return yystartcol;
}

static void update_location(void) {
    yystartcol = yycolumn;
    for (int i = 0; i < yyleng; i++) {
        if (yytext[i] == '\n')
            yycolumn = 1;
        else
            yycolumn++;
    }
}

static void lex_error(const char *msg) {
    fprintf(stderr, "Erro léxico: %s na linha %d, coluna %d\n",
            msg, yylineno, yystartcol);
    lex_errors++;
}

static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        fprintf(stderr, "Erro: memória insuficiente\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

static char *dup_text(void) {
    char *out = xmalloc((size_t)yyleng + 1);
    memcpy(out, yytext, (size_t)yyleng);
    out[yyleng] = '\0';
    return out;
}

static int escaped_char(char c) {
    switch (c) {
    case 'n': return '\n';
    case 't': return '\t';
    case 'r': return '\r';
    case '\\': return '\\';
    case '\'': return '\'';
    case '"': return '"';
    case '0': return '\0';
    default: return (unsigned char)c;
    }
}

static char *unescape(const char *s, int n) {
    char *out = xmalloc((size_t)n + 1);
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '\\' && i + 1 < n) {
            i++;
            out[j++] = (char)escaped_char(s[i]);
        } else {
            out[j++] = s[i];
        }
    }
    out[j] = '\0';
    return out;
}
