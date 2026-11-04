#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "semantic.h"
#include "interpreter.h"
#include "optimizer.h"

extern FILE *yyin;
extern int yyparse(void);
extern ASTNode *ast_root;
extern SymbolTable *sym_table;

int main(int argc, char **argv) {
    char *filename = NULL;
    int enable_optimize = 0;
    int enable_print_ast = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--optimize") == 0 || strcmp(argv[i], "-O") == 0 || strcmp(argv[i], "-O1") == 0) {
            enable_optimize = 1;
        } else if (strcmp(argv[i], "--print-ast") == 0) {
            enable_print_ast = 1;
        } else if (argv[i][0] != '-') {
            filename = argv[i];
        }
    }

    if (!filename) {
        printf("Uso: %s [--optimize|-O] [--print-ast] <arquivo_fonte.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }
    yyin = fp;

    sym_table = create_symbol_table();

    int result = yyparse();
    fclose(yyin);

    if (result != 0) {
        printf("\n[FALHA] Foram encontrados erros sintáticos no código.\n");
        free_ast(ast_root);
        free_symbol_table(sym_table);
        return result;
    }

    printf("\n[SUCESSO] Análise sintática concluída com sucesso!\n");

    if (enable_optimize) {
        printf("\n--- Aplicando Otimizações na AST (Constant Folding) ---\n");
        ast_root = optimize_ast(ast_root);
        printf("[SUCESSO] AST otimizada com sucesso!\n");
    }

    if (enable_print_ast) {
        printf("\n--- Árvore Sintática Abstrata (AST) ---\n");
        print_ast(ast_root);
    }

    printf("\n--- Iniciando Análise Semântica ---\n");
    int semantic_errors = analyze_semantics(ast_root, sym_table);

    if (semantic_errors != 0) {
        printf("[FALHA] Execução abortada devido a %d erro(s) semântico(s).\n", semantic_errors);
        free_ast(ast_root);
        free_symbol_table(sym_table);
        return 1;
    }

    printf("[SUCESSO] Análise semântica concluída sem erros!\n");

    printf("\n--- Executando o Programa (Runtime) ---\n");
    execute_program(ast_root);

    free_ast(ast_root);
    free_symbol_table(sym_table);

    return 0;
}