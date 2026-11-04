#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "ast.h"
#include "symbol_table.h"

extern int yylineno;

const char* check_node_type(ASTNode *node, SymbolTable *table, int *error_count) {
    if (!node) return "void";

    switch (node->type) {
        case NODE_NUM_LITERAL:
            /* Se name contém ponto decimal ou se o num_value tem parte fracionária */
            if (node->name && strchr(node->name, '.')) {
                return "float";
            }
            return "int";

        case NODE_STRING_LITERAL:
            return "string";

        case NODE_VAR_REF: {
            Symbol *sym = lookup_symbol(table, node->name);
            if (!sym) {
                fprintf(stderr, "[ERRO SEMÂNTICO] Variável '%s' usada sem declaração na linha %d.\n", 
                        node->name, yylineno);
                if (error_count) (*error_count)++;
                return "unknown";
            }
            return sym->data_type ? sym->data_type : "int";
        }

        case NODE_BINARY_EXPR: {
            const char *left_type = check_node_type(node->left, table, error_count);
            const char *right_type = check_node_type(node->right, table, error_count);

            if (strcmp(left_type, "unknown") == 0 || strcmp(right_type, "unknown") == 0) {
                return "unknown";
            }

            /* Regra de coerção: se um dos lados for float, o resultado da expressão é float */
            if (strcmp(left_type, "float") == 0 || strcmp(right_type, "float") == 0) {
                return "float";
            }
            return "int";
        }

        default:
            return "void";
    }
}

int analyze_semantics(ASTNode *node, SymbolTable *table) {
    if (!node) return 0;

    int errors = 0;

    switch (node->type) {
        case NODE_DECLARATION: {
            if (node->left) {
                check_node_type(node->left, table, &errors);
            }
            break;
        }

        case NODE_ASSIGNMENT: {
            Symbol *sym = lookup_symbol(table, node->name);
            if (!sym) {
                fprintf(stderr, "[ERRO SEMÂNTICO] Atribuição a variável não declarada '%s' na linha %d.\n", 
                        node->name, yylineno);
                errors++;
            } else {
                const char *expr_type = check_node_type(node->left, table, &errors);
                
                if (sym->data_type && strcmp(sym->data_type, "int") == 0 && strcmp(expr_type, "float") == 0) {
                    fprintf(stderr, "[AVISO SEMÂNTICO] Atribuição de 'float' para variável 'int %s' na linha %d (pode haver perda de precisão).\n", 
                            node->name, yylineno);
                }
            }
            break;
        }

        case NODE_IF:
            check_node_type(node->left, table, &errors);
            errors += analyze_semantics(node->right, table);
            if (node->else_branch) {
                errors += analyze_semantics(node->else_branch, table);
            }
            break;

        case NODE_WHILE:
            check_node_type(node->left, table, &errors);
            errors += analyze_semantics(node->right, table);
            break;

        case NODE_BLOCK:
            enter_scope(table);
            if (node->left) {
                errors += analyze_semantics(node->left, table);
            }
            exit_scope(table);
            break;

        default:
            if (node->left) errors += analyze_semantics(node->left, table);
            if (node->right) errors += analyze_semantics(node->right, table);
            break;
    }

    if (node->type != NODE_IF && node->next) {
        errors += analyze_semantics(node->next, table);
    }

    return errors;
}