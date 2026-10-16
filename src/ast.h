#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_BINARY_EXPR,
    NODE_VAR_REF,
    NODE_NUM_LITERAL,
    NODE_STRING_LITERAL,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *name;               /* Nome de variável ou operador */
    double num_value;         /* Valor para literais numéricos */
    char *string_value;       /* Valor para literais de texto */
    struct ASTNode *left;     /* Filho esquerdo / Condição */
    struct ASTNode *right;    /* Filho direito / Corpo */
    struct ASTNode *next;     /* Próximo nó na lista de comandos */
    struct ASTNode *else_branch;  /* Bloco else (somente NODE_IF) */
} ASTNode;

ASTNode *create_node(NodeType type);
ASTNode *create_num_node(double value);
ASTNode *create_string_node(const char *value);
ASTNode *create_var_node(const char *name);
ASTNode *create_binary_node(const char *op, ASTNode *left, ASTNode *right);
ASTNode *create_decl_node(const char *name, ASTNode *init_expr);
ASTNode *create_assign_node(const char *name, ASTNode *expr);
ASTNode *create_if_node(ASTNode *cond, ASTNode *then_block, ASTNode *else_block);
ASTNode *create_while_node(ASTNode *cond, ASTNode *body);
ASTNode *append_node(ASTNode *list, ASTNode *node);
void free_ast(ASTNode *node);

#endif
