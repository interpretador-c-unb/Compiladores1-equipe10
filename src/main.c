#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

extern FILE *yyin;
extern int yyparse(void);
extern ASTNode *ast_root;
extern SymbolTable *sym_table;
extern int semantic_errors;

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
    if (semantic_errors > 0)
        result = 1;

    if (argc > 1 && yyin) {
        fclose(yyin);
    }

    if (result == 0) {
        printf("\n[SUCESSO] Análise sintática e construção da AST/Tabela concluídas com sucesso!\n");
    } else {
        printf("\n[FALHA] Foram encontrados erros no código.\n");
    }

    free_ast(ast_root);
    free_symbol_table(sym_table);

    return result;
}
