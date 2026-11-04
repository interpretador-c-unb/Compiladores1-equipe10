#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"
#include "semantic.h"

extern FILE *yyin;
extern int yyparse(void);
extern ASTNode *ast_root;
extern SymbolTable *sym_table;

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

    sym_table = create_symbol_table();

    int result = yyparse();

    if (argc > 1 && yyin) {
        fclose(yyin);
    }

    if (result != 0) {
        printf("\n[FALHA] Foram encontrados erros sintáticos no código.\n");
        free_ast(ast_root);
        free_symbol_table(sym_table);
        return result;
    }

    printf("\n[SUCESSO] Análise sintática e construção da AST/Tabela concluídas com sucesso!\n");
    printf("\n--- Árvore Sintática Abstrata (AST) ---\n");
    print_ast(ast_root);

    printf("\n--- Iniciando Análise Semântica ---\n");
    int semantic_errors = analyze_semantics(ast_root, sym_table);

    if (semantic_errors == 0) {
        printf("[SUCESSO] Análise semântica concluída sem erros!\n");
    } else {
        printf("[FALHA] Foram encontrados %d erro(s) semântico(s) no código.\n", semantic_errors);
    }

    free_ast(ast_root);
    free_symbol_table(sym_table);

    return semantic_errors == 0 ? 0 : 1;
}