%{
#include "interpretador_c.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
static const char *token_name(int tok);
static void print_escaped(const char *s);
static void print_charlit(int c);
static void print_token(int tok);

int lex_column(void);
%}

%option noyywrap
%option nounput
%option noinput
%option nodefault
%option yylineno
%x COMMENT

DIGIT   [0-9]
LETTER  [A-Za-z_]

/* Palavras reservadas ficam antes da regra de ID.
   No mesmo tamanho de lexema, o Flex escolhe a primeira regra. */

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

static const char *token_name(int tok) {
    switch (tok) {
    case IF: return "IF";
    case ELSE: return "ELSE";
    case WHILE: return "WHILE";
    case FOR: return "FOR";
    case RETURN: return "RETURN";
    case INT: return "INT";
    case CHAR: return "CHAR";
    case FLOAT: return "FLOAT";
    case VOID: return "VOID";
    case ASSIGN: return "ASSIGN";
    case PLUS: return "PLUS";
    case MINUS: return "MINUS";
    case STAR: return "STAR";
    case SLASH: return "SLASH";
    case PERCENT: return "PERCENT";
    case EQ: return "EQ";
    case NE: return "NE";
    case LT: return "LT";
    case GT: return "GT";
    case LE: return "LE";
    case GE: return "GE";
    case ANDAND: return "ANDAND";
    case OROR: return "OROR";
    case NOT: return "NOT";
    case PLUSPLUS: return "PLUSPLUS";
    case MINUSMINUS: return "MINUSMINUS";
    case LPAREN: return "LPAREN";
    case RPAREN: return "RPAREN";
    case LBRACE: return "LBRACE";
    case RBRACE: return "RBRACE";
    case LBRACK: return "LBRACK";
    case RBRACK: return "RBRACK";
    case SEMI: return "SEMI";
    case COMMA: return "COMMA";
    default: return NULL;
    }
}

static void print_escaped(const char *s) {
    fputc('"', stdout);
    for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
        switch (*p) {
        case '\n': fputs("\\n", stdout); break;
        case '\t': fputs("\\t", stdout); break;
        case '\r': fputs("\\r", stdout); break;
        case '\\': fputs("\\\\", stdout); break;
        case '"': fputs("\\\"", stdout); break;
        default:
            if (*p >= 32 && *p < 127)
                fputc(*p, stdout);
            else
                printf("\\x%02x", *p);
        }
    }
    fputc('"', stdout);
}

static void print_charlit(int c) {
    if (c == '\n') fputs("CHARLIT('\\n')", stdout);
    else if (c == '\t') fputs("CHARLIT('\\t')", stdout);
    else if (c == '\r') fputs("CHARLIT('\\r')", stdout);
    else if (c == '\\') fputs("CHARLIT('\\\\')", stdout);
    else if (c == '\'') fputs("CHARLIT('\\'')", stdout);
    else if (c >= 32 && c < 127) printf("CHARLIT('%c')", c);
    else printf("CHARLIT(%d)", c);
}

static void print_token(int tok) {
    printf("%d:%d ", yylineno, yystartcol);
    if (tok == ID) {
        printf("ID(%s)\n", yylval.idValue);
        free(yylval.idValue);
    } else if (tok == STRING) {
        fputs("STRING(", stdout);
        print_escaped(yylval.stringValue);
        fputs(")\n", stdout);
        free(yylval.stringValue);
    } else if (tok == NUM) {
        printf("NUM(%.15g)\n", yylval.numValue);
    } else if (tok == CHARLIT) {
        print_charlit(yylval.charValue);
        fputc('\n', stdout);
    } else {
        const char *name = token_name(tok);
        if (name) {
            puts(name);
        } else if (tok > 0 && tok < 256 && tok >= 32) {
            printf("INVALID('%c')\n", tok);
        } else {
            printf("INVALID(%d)\n", tok);
        }
    }
}

/* ./parser arquivo          reconhece e analisa o programa
   ./parser --tokens arquivo lista linha, coluna e token */
int main(int argc, char **argv) {
    int dump = 0;
    int argi = 1;
    FILE *opened = NULL;

    if (argc > 1 && strcmp(argv[1], "--tokens") == 0) {
        dump = 1;
        argi = 2;
    }

    if (argc > argi) {
        opened = fopen(argv[argi], "r");
        if (!opened) {
            fprintf(stderr, "Não foi possível abrir '%s'\n", argv[argi]);
            return 1;
        }
        yyin = opened;
    }

    if (dump) {
        int tok;
        while ((tok = yylex()) != 0)
            print_token(tok);
        if (opened)
            fclose(opened);
        return lex_errors ? 1 : 0;
    }

    int status = yyparse();
    if (opened)
        fclose(opened);
    if (lex_errors)
        return 1;
    return status;
}
