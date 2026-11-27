#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interpreter.h"

Environment* create_environment(void) {
    Environment *env = (Environment *) malloc(sizeof(Environment));
    env->head = NULL;
    env->current_scope = 0;
    env->had_error = 0;
    return env;
}

void env_enter_scope(Environment *env) {
    if (env) env->current_scope++;
}

void env_exit_scope(Environment *env) {
    if (!env) return;
    
    RuntimeVar **curr = &(env->head);
    while (*curr) {
        if ((*curr)->scope_level == env->current_scope) {
            RuntimeVar *temp = *curr;
            *curr = (*curr)->next;
            if (temp->name) free(temp->name);
            if (temp->value.type == VAL_STRING && temp->value.data.str_val) {
                free(temp->value.data.str_val);
            }
            free(temp);
        } else {
            curr = &((*curr)->next);
        }
    }
    if (env->current_scope > 0) env->current_scope--;
}

void free_environment(Environment *env) {
    if (!env) return;
    while (env->head) {
        RuntimeVar *temp = env->head;
        env->head = env->head->next;
        if (temp->name) free(temp->name);
        if (temp->value.type == VAL_STRING && temp->value.data.str_val) {
            free(temp->value.data.str_val);
        }
        free(temp);
    }
    free(env);
}

void set_variable(Environment *env, const char *name, Value val) {
    if (!env || !name) return;
    
    RuntimeVar *var = env->head;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            if (var->value.type == VAL_STRING && var->value.data.str_val) {
                free(var->value.data.str_val);
            }
            var->value = val;
            if (val.type == VAL_STRING && val.data.str_val) {
                var->value.data.str_val = strdup(val.data.str_val);
            }
            return;
        }
        var = var->next;
    }

    RuntimeVar *new_var = (RuntimeVar *) malloc(sizeof(RuntimeVar));
    new_var->name = strdup(name);
    new_var->scope_level = env->current_scope;
    new_var->value = val;
    if (val.type == VAL_STRING && val.data.str_val) {
        new_var->value.data.str_val = strdup(val.data.str_val);
    }
    new_var->next = env->head;
    env->head = new_var;
}

Value get_variable(Environment *env, const char *name) {
    Value val;
    val.type = VAL_VOID;
    if (!env || !name) return val;

    RuntimeVar *var = env->head;
    while (var) {
        if (strcmp(var->name, name) == 0) {
            val = var->value;
            if (val.type == VAL_STRING && val.data.str_val) {
                val.data.str_val = strdup(val.data.str_val);
            }
            return val;
        }
        var = var->next;
    }
    return val;
}

static int is_truthy(Value v) {
    if (v.type == VAL_INT) return v.data.int_val != 0;
    if (v.type == VAL_FLOAT) return v.data.float_val != 0.0;
    if (v.type == VAL_STRING) return v.data.str_val != NULL && strlen(v.data.str_val) > 0;
    return 0;
}

static void execute_list(ASTNode *list, Environment *env);

static void exec_printf(ASTNode *node, Environment *env) {
    const char *fmt = node->string_value ? node->string_value : "";
    ASTNode *arg = node->left;

    for (const char *p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        p++;
        if (*p == '\0') break;
        if (*p == '%') { putchar('%'); continue; }

        if (!arg) {
            fprintf(stderr, "[ERRO RUNTIME] printf: faltam argumentos para '%%%c'\n", *p);
            continue;
        }

        Value v = evaluate_ast(arg, env);
        arg = arg->next;

        if (env->had_error) return;

        switch (*p) {
            case 'd':
                if (v.type == VAL_INT) printf("%d", v.data.int_val);
                else if (v.type == VAL_FLOAT) printf("%d", (int)v.data.float_val);
                else fprintf(stderr, "[ERRO RUNTIME] printf: '%%d' exige número\n");
                break;
            case 'f':
                if (v.type == VAL_INT) printf("%f", (double)v.data.int_val);
                else if (v.type == VAL_FLOAT) printf("%f", v.data.float_val);
                else fprintf(stderr, "[ERRO RUNTIME] printf: '%%f' exige número\n");
                break;
            case 's':
                if (v.type == VAL_STRING && v.data.str_val) printf("%s", v.data.str_val);
                else fprintf(stderr, "[ERRO RUNTIME] printf: '%%s' exige texto\n");
                break;
            case 'c':
                if (v.type == VAL_STRING && v.data.str_val && v.data.str_val[0])
                    putchar(v.data.str_val[0]);
                else if (v.type == VAL_INT) putchar(v.data.int_val);
                else fprintf(stderr, "[ERRO RUNTIME] printf: '%%c' exige caractere\n");
                break;
            default:
                fprintf(stderr, "[ERRO RUNTIME] printf: formato '%%%c' não suportado\n", *p);
        }

        if (v.type == VAL_STRING && v.data.str_val) free(v.data.str_val);
    }
}

Value evaluate_ast(ASTNode *node, Environment *env) {
    Value result;
    result.type = VAL_VOID;
    if (!node) return result;

    switch (node->type) {
        case NODE_NUM_LITERAL:
            if (node->name && strchr(node->name, '.')) {
                result.type = VAL_FLOAT;
                result.data.float_val = node->num_value;
            } else {
                result.type = VAL_INT;
                result.data.int_val = (int)node->num_value;
            }
            break;

        case NODE_STRING_LITERAL:
            result.type = VAL_STRING;
            result.data.str_val = node->string_value ? strdup(node->string_value) : strdup("");
            break;

        case NODE_VAR_REF:
            return get_variable(env, node->name);

        case NODE_DECLARATION: {
            Value init_val;
            init_val.type = VAL_INT;
            init_val.data.int_val = 0;
            if (node->left) {
                init_val = evaluate_ast(node->left, env);
            }
            if (env->had_error) break;
            set_variable(env, node->name, init_val);
            break;
        }

        case NODE_ASSIGNMENT: {
            Value val = evaluate_ast(node->left, env);
            if (env->had_error) return val;
            set_variable(env, node->name, val);
            return val;
        }

        case NODE_BINARY_EXPR: {
            Value left = evaluate_ast(node->left, env);
            Value right = evaluate_ast(node->right, env);

            if (env->had_error) break;
            if (strcmp(node->name, "/") == 0 || strcmp(node->name, "%") == 0) {
                int divisor_zero = 0;
                if (right.type == VAL_FLOAT) divisor_zero = (right.data.float_val == 0.0);
                else if (right.type == VAL_INT) divisor_zero = (right.data.int_val == 0);

                if (divisor_zero) {
                    fprintf(stderr, "[ERRO RUNTIME] Divisão por zero (operador '%s')\n", node->name);
                    env->had_error = 1;
                    break;  /* sai do case com result.type = VAL_VOID */
                }
            }
            if (left.type == VAL_FLOAT || right.type == VAL_FLOAT) {
                double l = (left.type == VAL_FLOAT) ? left.data.float_val : left.data.int_val;
                double r = (right.type == VAL_FLOAT) ? right.data.float_val : right.data.int_val;
                result.type = VAL_FLOAT;

                if (strcmp(node->name, "+") == 0) result.data.float_val = l + r;
                else if (strcmp(node->name, "-") == 0) result.data.float_val = l - r;
                else if (strcmp(node->name, "*") == 0) result.data.float_val = l * r;
                else if (strcmp(node->name, "/") == 0) result.data.float_val = (r != 0) ? l / r : 0;
                else if (strcmp(node->name, ">") == 0) { result.type = VAL_INT; result.data.int_val = l > r; }
                else if (strcmp(node->name, "<") == 0) { result.type = VAL_INT; result.data.int_val = l < r; }
                else if (strcmp(node->name, ">=") == 0) { result.type = VAL_INT; result.data.int_val = l >= r; }
                else if (strcmp(node->name, "<=") == 0) { result.type = VAL_INT; result.data.int_val = l <= r; }
                else if (strcmp(node->name, "==") == 0) { result.type = VAL_INT; result.data.int_val = l == r; }
                else if (strcmp(node->name, "!=") == 0) { result.type = VAL_INT; result.data.int_val = l != r; }
            } else {
                int l = left.data.int_val;
                int r = right.data.int_val;
                result.type = VAL_INT;

                if (strcmp(node->name, "+") == 0) result.data.int_val = l + r;
                else if (strcmp(node->name, "-") == 0) result.data.int_val = l - r;
                else if (strcmp(node->name, "*") == 0) result.data.int_val = l * r;
                else if (strcmp(node->name, "/") == 0) result.data.int_val = (r != 0) ? l / r : 0;
                else if (strcmp(node->name, "%") == 0) result.data.int_val = (r != 0) ? l % r : 0;
                else if (strcmp(node->name, ">") == 0) result.data.int_val = l > r;
                else if (strcmp(node->name, "<") == 0) result.data.int_val = l < r;
                else if (strcmp(node->name, ">=") == 0) result.data.int_val = l >= r;
                else if (strcmp(node->name, "<=") == 0) result.data.int_val = l <= r;
                else if (strcmp(node->name, "==") == 0) result.data.int_val = l == r;
                else if (strcmp(node->name, "!=") == 0) result.data.int_val = l != r;
                else if (strcmp(node->name, "&&") == 0) result.data.int_val = l && r;
                else if (strcmp(node->name, "||") == 0) result.data.int_val = l || r;
            }
            break;
        }

        case NODE_IF: {
            Value cond = evaluate_ast(node->left, env);

            if (env->had_error) break;

            if (is_truthy(cond)) {
                evaluate_ast(node->right, env);
            } else if (node->else_branch) {
                evaluate_ast(node->else_branch, env);
            }
            break;
        }

        case NODE_WHILE: {
            while (!env->had_error && is_truthy(evaluate_ast(node->left, env))) {
                evaluate_ast(node->right, env);
            }
            break;
        }

        case NODE_BLOCK: {
            env_enter_scope(env);
            execute_list(node->left, env);
            env_exit_scope(env);
            return result;
        }

        case NODE_PRINTF:
            exec_printf(node, env);
            break;
        
        default:
            if (node->left) evaluate_ast(node->left, env);
            if (node->right) evaluate_ast(node->right, env);
            break;
    }

    /*if (node->type != NODE_IF && node->type != NODE_BLOCK && node->next) {
        evaluate_ast(node->next, env);
    }*/

    return result;
}

static void execute_list(ASTNode *list, Environment *env) {
       for (ASTNode *cur = list; cur && !env->had_error; cur = cur->next)
           evaluate_ast(cur, env);
   }

int execute_program(ASTNode *root) {
    if (!root) return 0;

    Environment *env = create_environment();
    execute_list(root, env);

    int status = env->had_error ? 1 : 0;
    if (status) fprintf(stderr, "[ERRO RUNTIME] Execução abortada.\n");

    printf("\n--- Estado Final das Variáveis no Ambiente de Runtime ---\n");
    RuntimeVar *curr = env->head;
    while (curr) {
        if (curr->value.type == VAL_INT) {
            printf("%s (int) = %d\n", curr->name, curr->value.data.int_val);
        } else if (curr->value.type == VAL_FLOAT) {
            printf("%s (float) = %g\n", curr->name, curr->value.data.float_val);
        } else if (curr->value.type == VAL_STRING) {
            printf("%s (string) = \"%s\"\n", curr->name, curr->value.data.str_val ? curr->value.data.str_val : "");
        }
        curr = curr->next;
    }

    free_environment(env);
    return status;
}