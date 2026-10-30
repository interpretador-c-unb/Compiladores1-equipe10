#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *create_node(NodeType type) {
    ASTNode *node = (ASTNode *) calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Erro de alocação de memória para ASTNode\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    return node;
}

ASTNode *create_num_node(double value) {
    ASTNode *node = create_node(NODE_NUM_LITERAL);
    node->num_value = value;
    return node;
}

ASTNode *create_string_node(const char *value) {
    ASTNode *node = create_node(NODE_STRING_LITERAL);
    if (value) node->string_value = strdup(value);
    return node;
}

ASTNode *create_var_node(const char *name) {
    ASTNode *node = create_node(NODE_VAR_REF);
    if (name) node->name = strdup(name);
    return node;
}

ASTNode *create_binary_node(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(NODE_BINARY_EXPR);
    if (op) node->name = strdup(op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *create_decl_node(const char *name, ASTNode *init_expr) {
    ASTNode *node = create_node(NODE_DECLARATION);
    if (name) node->name = strdup(name);
    node->left = init_expr;
    return node;
}

ASTNode *create_assign_node(const char *name, ASTNode *expr) {
    ASTNode *node = create_node(NODE_ASSIGNMENT);
    if (name) node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode *create_if_node(ASTNode *cond, ASTNode *then_block, ASTNode *else_block) {
    ASTNode *node = create_node(NODE_IF);
    node->left = cond;
    node->right = then_block;
    node->else_branch = else_block;
    return node;
}

ASTNode *create_while_node(ASTNode *cond, ASTNode *body) {
    ASTNode *node = create_node(NODE_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

ASTNode *append_node(ASTNode *list, ASTNode *node) {
    if (!list) return node;
    ASTNode *curr = list;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = node;
    return list;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->name) free(node->name);
    if (node->string_value) free(node->string_value);
    if (node->left) free_ast(node->left);
    if (node->right) free_ast(node->right);
    if (node->else_branch) free_ast(node->else_branch);
    if (node->next) free_ast(node->next);
    free(node);
}

static void print_indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}

static void print_list(const ASTNode *node, int level);

static void print_node(const ASTNode *n, int level) {
    print_indent(level);
    switch (n->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            print_list(n->left, level + 1);
            break;
        case NODE_BLOCK:
            printf("Block\n");
            print_list(n->left, level + 1);
            break;
        case NODE_DECLARATION:
            printf("Declaration(%s)\n", n->name ? n->name : "?");
            if (n->left) print_node(n->left, level + 1);
            break;
        case NODE_ASSIGNMENT:
            printf("Assignment(%s)\n", n->name ? n->name : "?");
            if (n->left) print_node(n->left, level + 1);
            break;
        case NODE_BINARY_EXPR:
            printf("BinaryExpr(%s)\n", n->name ? n->name : "?");
            if (n->left)  print_node(n->left, level + 1);
            if (n->right) print_node(n->right, level + 1);
            break;
        case NODE_VAR_REF:
            printf("Var(%s)\n", n->name ? n->name : "?");
            break;
        case NODE_NUM_LITERAL:
            printf("Num(%g)\n", n->num_value);
            break;
        case NODE_STRING_LITERAL:
            printf("String(\"%s\")\n", n->string_value ? n->string_value : "");
            break;
        case NODE_IF:
            printf("If\n");
            print_indent(level + 1); printf("Cond:\n");
            if (n->left) print_node(n->left, level + 2);
            print_indent(level + 1); printf("Then:\n");
            print_list(n->right, level + 2);
            if (n->else_branch) {
                print_indent(level + 1); printf("Else:\n");
                print_list(n->else_branch, level + 2);
            }
            break;
        case NODE_WHILE:
            printf("While\n");
            print_indent(level + 1); printf("Cond:\n");
            if (n->left) print_node(n->left, level + 2);
            print_indent(level + 1); printf("Body:\n");
            print_list(n->right, level + 2);
            break;
        default:
            printf("Unknown(%d)\n", n->type);
    }
}

/* Percorre uma lista de comandos ligada por ->next */
static void print_list(const ASTNode *node, int level) {
    for (const ASTNode *cur = node; cur; cur = cur->next)
        print_node(cur, level);
}

void print_ast(const ASTNode *root) {
    print_list(root, 0);
}