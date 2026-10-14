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
    node->next = else_block;
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
    if (node->next) free_ast(node->next);
    free(node);
}
