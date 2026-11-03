# Arquitetura da Tabela de Símbolos

## 1. Objetivo

A tabela de símbolos registra os identificadores declarados e permite consultar o tipo e o escopo de cada declaração durante a análise. O módulo está em `src/symbol_table.h` e `src/symbol_table.c`.

## 2. Responsabilidades do módulo

- Inserir símbolos declarados no programa e localizá-los por nome.
- Armazenar o tipo declarado e o nível de escopo.
- Detectar redeclarações no mesmo escopo.
- Manter visíveis as declarações do bloco atual e dos blocos externos.
- Liberar a memória das entradas ao encerrar o escopo ou destruir a tabela.

## 3. Modelo de dados

### 3.1 Tipos básicos

O campo `data_type` guarda o tipo declarado como texto, por exemplo `int`, `float` ou `char`. Esse campo registra a declaração para uso na análise.

### 3.2 Entrada da tabela

```c
typedef struct Symbol {
    char *name;
    char *data_type;
    int scope_level;
    struct Symbol *next;
} Symbol;
```

`name` identifica a variável, `scope_level` indica onde ela foi declarada e `next` aponta para a próxima entrada da lista encadeada.

### 3.3 Escopo

```c
typedef struct SymbolTable {
    Symbol *head;
    int current_scope;
} SymbolTable;
```

`head` aponta para a entrada mais recente. O nível global é 0 e cada bloco aberto incrementa `current_scope`. Todas as entradas visíveis ficam na mesma lista.

## 4. Interface pública em `symbol_table.h`

```c
SymbolTable *create_symbol_table(void);
int insert_symbol(SymbolTable *table, const char *name, const char *data_type);
Symbol *lookup_symbol(SymbolTable *table, const char *name);
void enter_scope(SymbolTable *table);
void exit_scope(SymbolTable *table);
void free_symbol_table(SymbolTable *table);
```

## 5. Operações e regras

### 5.1 Inicialização

`create_symbol_table()` cria uma lista vazia com o nível de escopo em 0.

### 5.2 Inserção

`insert_symbol()` insere a declaração no início da lista e retorna `1`. Retorna `0` se a tabela ou o nome forem nulos ou se o mesmo nome já existir no nível atual. O mesmo nome pode aparecer em níveis diferentes.

### 5.3 Consulta no escopo atual

Durante a inserção, a verificação de redeclaração compara o nome e o nível de escopo da entrada com os da nova declaração.

### 5.4 Consulta lexical

`lookup_symbol()` percorre a lista a partir de `head` e retorna a primeira entrada com o nome solicitado. Se o nome estiver ausente, retorna `NULL`. Como as declarações do bloco atual entram na frente da lista, elas podem sombrear declarações externas.

```c
int valor = 1;
{
    float valor = 2.5;
    valor = valor + 1;
}
valor = valor + 1;
```

```text
Antes do bloco     nível 0    head → valor (int, 0)
Dentro do bloco    nível 1    head → valor (float, 1) → valor (int, 0)
Depois do bloco    nível 0    head → valor (int, 0)
```

Dentro do bloco, a consulta encontra `float valor`. Fora dele, encontra `int valor`.

### 5.5 Entrada e saída de escopos

`enter_scope()` incrementa o nível atual. `exit_scope()` remove as entradas daquele nível, libera a memória delas e decrementa o nível. No nível global, a chamada de saída retorna imediatamente.

## 6. Política de memória

`insert_symbol()` copia o nome e o tipo. Assim, as entradas da tabela têm suas próprias strings. `exit_scope()` libera as entradas do bloco encerrado e `free_symbol_table()` libera as entradas restantes e a própria tabela.

O ponteiro retornado por `lookup_symbol()` pertence à tabela e vale enquanto a entrada estiver presente.

## 7. Integração com o parser e a análise semântica

O lexer reconhece identificadores e produz o token `ID`. O driver cria a tabela antes de chamar `yyparse()` e a libera ao terminar.

Ao reconhecer uma declaração como `int contador;`, a ação do Bison chama `insert_symbol()`. Nas referências e nos destinos de atribuição, o parser chama `lookup_symbol()` para verificar se o nome está visível. As regras de bloco chamam `enter_scope()` na abertura e `exit_scope()` no fechamento.

## 8. Diagnósticos

Uma consulta sem resultado emite um aviso no parser. Uma redeclaração no mesmo nível emite um erro em `insert_symbol()`.

```text
[AVISO SEMÂNTICO] Variável 'total' usada sem declaração na linha 4
[ERRO SEMÂNTICO] Variável 'total' já declarada no escopo 0
```

O [guia de análise semântica](analise-semantica.md) explica os diagnósticos e os códigos de saída atuais.

## 9. Decisões pendentes

A representação de parâmetros e assinaturas depende da expansão da gramática para funções. A validação de tipos e a localização mais precisa dos diagnósticos também dependem da evolução da análise semântica.

## 10. Relação com as próximas etapas

A análise semântica pode usar os metadados de tipo e escopo das declarações. Os valores durante a execução ficam a cargo do [ambiente de runtime planejado](interpretador.md).
