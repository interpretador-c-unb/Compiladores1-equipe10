#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yyparse(void);

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            perror("Erro ao abrir arquivo de entrada");
            return 1;
        }
        yyin = fp;
    } else {
        printf("Uso: %s <arquivo_fonte.c>\n", argv[0]);
        return 1;
    }

    int result = yyparse();

    if (argc > 1 && yyin) {
        fclose(yyin);
    }

    if (result == 0) {
        printf("\n[SUCESSO] Análise sintática concluída sem erros!\n");
    } else {
        printf("\n[FALHA] Foram encontrados erros sintáticos no código.\n");
    }

    return result;
}