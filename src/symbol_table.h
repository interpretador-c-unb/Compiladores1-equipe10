#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol {
    char *name;
    char *data_type;          /* "int", "float", "char", "void" */
    int scope_level;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    Symbol *head;
    int current_scope;
} SymbolTable;

SymbolTable *create_symbol_table(void);
int insert_symbol(SymbolTable *table, const char *name, const char *data_type);
Symbol *lookup_symbol(SymbolTable *table, const char *name);
void enter_scope(SymbolTable *table);
void exit_scope(SymbolTable *table);
void free_symbol_table(SymbolTable *table);

#endif