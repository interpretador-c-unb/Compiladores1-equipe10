#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol_table.h"

/* Realiza a análise semântica completa percorrendo a AST */
int analyze_semantics(ASTNode *node, SymbolTable *table);

/* Retorna o tipo de dado resultante de um nó ("int", "float", etc.) */
const char* check_node_type(ASTNode *node, SymbolTable *table, int *error_count);

#endif