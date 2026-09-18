%{
#define _POSIX_C_SOURCE 200809L
#include "interpretador_c.tab.h"  /* Inclusão do cabeçalho gerado pelo Bison, que contém as definições de tokens */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

/* Definições de padrões de token e ações */
%%

[ \t\n]+                        { /* Não faz nada, apenas ignora */ }

[+\-]?[0-9]{1,10}               {
                                /* Converter o lexema para inteiro (pode estourar se for maior que INT, 
                                    mas servirá como exemplo didático) */
                                yylval.intValue = atoi(yytext);
                                return NUM;
                                }

^[A-Za-z][A-Za-z0-9]+           {   
                                /* Converter o lexema para string*/
                                yylval.idValue = strdup(yytext);
                                return ID;
                                }

[A-Za-zà-ÿ0-9.,]*               {
                                /* Converter string */
                                yylval.stringValue = strdup(yytext);

                                return STRING;
                                }
[\"]                            {
                                /* Aspas */
                                return QUOT;
                                }

[\;]                            {
                                /* Ponto e virgula */
                                return SEMI;
                                }


[\=]                            {
                                return EQ;
                                }

.                               {/* Valor padrão | não faz nada */}

%%

/* Função auxiliar do Flex (quando termina o arquivo de entrada) */
int yywrap(void) {
    return 1;
}

/* Função principal: chamará o parser ao ser executado */
int main(int argc, char **argv) {
    /* Se houver arquivo de entrada como parâmetro, redireciona o yyin */
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (f) {
            yyin = f;
        }
    }
    return yyparse();
}