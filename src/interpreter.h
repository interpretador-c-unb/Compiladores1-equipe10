#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef enum {
    VAL_INT,
    VAL_FLOAT,
    VAL_STRING,
    VAL_VOID
} ValueType;

typedef struct {
    ValueType type;
    union {
        int int_val;
        double float_val;
        char *str_val;
    } data;
} Value;

typedef struct RuntimeVar {
    char *name;
    Value value;
    int scope_level;
    struct RuntimeVar *next;
} RuntimeVar;

typedef struct {
    RuntimeVar *head;
    int current_scope;
} Environment;

Environment* create_environment(void);
void free_environment(Environment *env);
void env_enter_scope(Environment *env);
void env_exit_scope(Environment *env);

void set_variable(Environment *env, const char *name, Value val);
Value get_variable(Environment *env, const char *name);

Value evaluate_ast(ASTNode *node, Environment *env);
int execute_program(ASTNode *root);

#endif