#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
extern int yyparse(void);
extern int lexer_dump_tokens(void);

static void print_usage(const char *program) {
    fprintf(stderr, "Uso: %s [--tokens] <arquivo_fonte.c>\n", program);
}

int main(int argc, char **argv) {
    int dump_tokens = 0;
    const char *input_path = NULL;

    if (argc == 2) {
        input_path = argv[1];
    } else if (argc == 3 && strcmp(argv[1], "--tokens") == 0) {
        dump_tokens = 1;
        input_path = argv[2];
    } else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *input = fopen(input_path, "r");
    if (input == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo de entrada '%s'\n", input_path);
        return EXIT_FAILURE;
    }

    yyin = input;
    int result = dump_tokens ? lexer_dump_tokens() : yyparse();
    fclose(input);

    if (!dump_tokens) {
        if (result == 0) {
            printf("\n[SUCESSO] Análise sintática concluída sem erros!\n");
        } else {
            printf("\n[FALHA] Foram encontrados erros sintáticos no código.\n");
        }
    }

    return result;
}