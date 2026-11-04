#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ast.h"

/* Otimiza a AST aplicando Constant Folding (dobramento de constantes) */
ASTNode* optimize_ast(ASTNode *node);

#endif