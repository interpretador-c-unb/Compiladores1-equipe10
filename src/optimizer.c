#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optimizer.h"
#include "ast.h"

static int is_constant_node(ASTNode *node) {
    return node && node->type == NODE_NUM_LITERAL;
}

ASTNode* optimize_ast(ASTNode *node) {
    if (!node) return NULL;

    /* Percurso pós-ordem: otimiza primeiro os filhos */
    node->left = optimize_ast(node->left);
    node->right = optimize_ast(node->right);
    if (node->else_branch) {
        node->else_branch = optimize_ast(node->else_branch);
    }

    /* Constant Folding em expressões binárias */
    if (node->type == NODE_BINARY_EXPR) {
        if (is_constant_node(node->left) && is_constant_node(node->right)) {
            double left_val = node->left->num_value;
            double right_val = node->right->num_value;
            double result_val = 0;
            int is_float = (node->left->name && strchr(node->left->name, '.')) ||
                           (node->right->name && strchr(node->right->name, '.'));
            int foldable = 1;

            if (strcmp(node->name, "+") == 0) result_val = left_val + right_val;
            else if (strcmp(node->name, "-") == 0) result_val = left_val - right_val;
            else if (strcmp(node->name, "*") == 0) result_val = left_val * right_val;
            else if (strcmp(node->name, "/") == 0) {
                if (right_val != 0) result_val = left_val / right_val;
                else foldable = 0; /* Evita divisão por zero em tempo de compilação */
            }
            else if (strcmp(node->name, "%") == 0) {
                if ((int)right_val != 0) result_val = (int)left_val % (int)right_val;
                else foldable = 0;
            }
            else if (strcmp(node->name, ">") == 0) result_val = left_val > right_val;
            else if (strcmp(node->name, "<") == 0) result_val = left_val < right_val;
            else if (strcmp(node->name, ">=") == 0) result_val = left_val >= right_val;
            else if (strcmp(node->name, "<=") == 0) result_val = left_val <= right_val;
            else if (strcmp(node->name, "==") == 0) result_val = left_val == right_val;
            else if (strcmp(node->name, "!=") == 0) result_val = left_val != right_val;
            else if (strcmp(node->name, "&&") == 0) result_val = (int)left_val && (int)right_val;
            else if (strcmp(node->name, "||") == 0) result_val = (int)left_val || (int)right_val;
            else foldable = 0;

            if (foldable) {
                ASTNode *folded = create_num_node(result_val);
                if (is_float) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%g", result_val);
                    if (!strchr(buf, '.')) strcat(buf, ".0");
                    folded->name = strdup(buf);
                }
                folded->next = node->next;

                /* Desaloca os nós antigos */
                node->next = NULL;
                free_ast(node);

                return folded;
            }
        }
    }

    if (node->next) {
        node->next = optimize_ast(node->next);
    }

    return node;
}