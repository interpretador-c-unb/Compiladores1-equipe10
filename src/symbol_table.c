#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

SymbolTable *create_symbol_table(void) {
    SymbolTable *table = (SymbolTable *) calloc(1, sizeof(SymbolTable));
    if (!table) {
        fprintf(stderr, "Erro de alocação da Tabela de Símbolos\n");
        exit(EXIT_FAILURE);
    }
    table->head = NULL;
    table->current_scope = 0;
    return table;
}

int insert_symbol(SymbolTable *table, const char *name, const char *data_type) {
    if (!table || !name) return 0;

    Symbol *curr = table->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0 && curr->scope_level == table->current_scope) {
            fprintf(stderr, "[ERRO SEMÂNTICO] Variável '%s' já declarada no escopo %d\n", name, table->current_scope);
            return 0;
        }
        curr = curr->next;
    }

    Symbol *sym = (Symbol *) malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->data_type = strdup(data_type ? data_type : "unknown");
    sym->scope_level = table->current_scope;
    sym->next = table->head;
    table->head = sym;

    printf("[TABELA DE SÍMBOLOS] Inserida variável '%s' (%s) no Escopo %d\n", name, sym->data_type, sym->scope_level);
    return 1;
}

Symbol *lookup_symbol(SymbolTable *table, const char *name) {
    if (!table || !name) return NULL;
    Symbol *curr = table->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void enter_scope(SymbolTable *table) {
    if (table) table->current_scope++;
}

void exit_scope(SymbolTable *table) {
    if (!table || table->current_scope == 0) return;

    Symbol **pp = &table->head;
    while (*pp) {
        if ((*pp)->scope_level == table->current_scope) {
            Symbol *tmp = *pp;
            *pp = tmp->next;
            free(tmp->name);
            free(tmp->data_type);
            free(tmp);
        } else {
            pp = &(*pp)->next;
        }
    }
    table->current_scope--;
}

void free_symbol_table(SymbolTable *table) {
    if (!table) return;
    Symbol *curr = table->head;
    while (curr) {
        Symbol *tmp = curr;
        curr = curr->next;
        free(tmp->name);
        free(tmp->data_type);
        free(tmp);
    }
    free(table);
}