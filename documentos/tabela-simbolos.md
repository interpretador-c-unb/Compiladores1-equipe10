# Arquitetura da Tabela de Símbolos

## 1. Objetivo

A tabela de símbolos será o componente responsável por registrar e consultar os identificadores reconhecidos pelo compilador. Ela permitirá associar cada nome usado no programa às informações necessárias para as próximas etapas do interpretador, como análise semântica, verificação de tipos e execução.

Nesta etapa, este documento especifica a arquitetura prevista para os arquivos:

```text
src/symbol_table.h
src/symbol_table.c
A implementação do módulo e sua integração ao parser são atividades separadas.

2. Responsabilidades do Módulo
A tabela de símbolos deverá:

inserir símbolos declarados no programa;

localizar símbolos por nome;

armazenar o tipo declarado e a categoria do símbolo;

detectar redeclarações no mesmo escopo;

representar escopos aninhados;

permitir a consulta de um símbolo no escopo atual e nos escopos externos;

liberar toda a memória alocada pelo módulo.

A tabela não deverá executar expressões nem produzir a AST. Sua responsabilidade é manter as informações de identificação e escopo que serão consumidas pela análise semântica.

3. Modelo de Dados Previsto
A implementação deverá utilizar tipos enumerados para evitar que os demais módulos dependam de números mágicos.

3.1 Tipos básicos
Uma representação inicial possível para tipos da linguagem é:

typedef enum {
    SYMBOL_TYPE_INT,
    SYMBOL_TYPE_FLOAT,
    SYMBOL_TYPE_CHAR,
    SYMBOL_TYPE_VOID
} SymbolType;
A categoria do símbolo identifica como o nome foi declarado:

typedef enum {
    SYMBOL_KIND_VARIABLE,
    SYMBOL_KIND_FUNCTION,
    SYMBOL_KIND_PARAMETER
} SymbolKind;
As categorias de função e parâmetro podem ser utilizadas quando a gramática for expandida. Na primeira implementação, a categoria mais importante será SYMBOL_KIND_VARIABLE.

3.2 Entrada da tabela
Cada símbolo deverá armazenar, no mínimo:

typedef struct Symbol {
    char *name;
    SymbolType type;
    SymbolKind kind;
    int declaration_line;
    struct Symbol *next;
} Symbol;
O campo next permite organizar as entradas em uma lista encadeada ou em um bucket de uma tabela hash. A escolha da estrutura interna pode ser refinada durante a implementação, sem alterar a interface pública.

3.3 Escopo
Os escopos deverão formar uma pilha ou cadeia de escopos:

typedef struct Scope {
    Symbol *symbols;
    struct Scope *parent;
    unsigned int depth;
} Scope;
symbols: contém os símbolos declarados naquele escopo;

parent: aponta para o escopo externo;

depth: identifica a profundidade para fins de diagnóstico e depuração.

O escopo global não possui parent. Ao entrar em um bloco, um novo escopo é criado apontando para o escopo anterior. Ao sair do bloco, o escopo atual é destruído ou armazenado conforme a estratégia definida para a AST.

4. Interface Pública Prevista em symbol_table.h
A interface inicial deverá ser pequena e suficiente para a análise semântica:

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include 

typedef enum {
    SYMBOL_TYPE_INT,
    SYMBOL_TYPE_FLOAT,
    SYMBOL_TYPE_CHAR,
    SYMBOL_TYPE_VOID
} SymbolType;

typedef enum {
    SYMBOL_KIND_VARIABLE,
    SYMBOL_KIND_FUNCTION,
    SYMBOL_KIND_PARAMETER
} SymbolKind;

typedef struct Symbol Symbol;
typedef struct Scope Scope;

typedef struct {
    Scope *current;
} SymbolTable;

void symbol_table_init(SymbolTable *table);
void symbol_table_destroy(SymbolTable *table);

int symbol_table_enter_scope(SymbolTable *table);
int symbol_table_leave_scope(SymbolTable *table);

int symbol_table_insert(SymbolTable *table,
                        const char *name,
                        SymbolType type,
                        SymbolKind kind,
                        int declaration_line);

const Symbol *symbol_table_lookup(const SymbolTable *table,
                                  const char *name);

const Symbol *symbol_table_lookup_current_scope(const SymbolTable *table,
                                                const char *name);

#endif
A implementação poderá retornar códigos de erro mais específicos posteriormente. A interface deve evitar expor a organização interna dos buckets ou das listas.

5. Operações e Regras
5.1 Inicialização
symbol_table_init() cria o escopo global e deixa a tabela pronta para receber declarações.

5.2 Inserção
symbol_table_insert() registra um novo símbolo no escopo atual.

A inserção deverá falhar quando:

table for nula;

name for nulo ou vazio;

já existir um símbolo com o mesmo nome no escopo atual;

houver falha de alocação de memória.

A mesma identificação pode existir em escopos diferentes, pois um escopo interno pode sombrear um símbolo externo. A decisão de permitir ou não esse sombreamento deve ser mantida como regra explícita da análise semântica.

5.3 Consulta no escopo atual
symbol_table_lookup_current_scope() procura apenas no escopo atual. Essa operação é necessária para detectar redeclarações antes da inserção.

5.4 Consulta lexical
symbol_table_lookup() procura primeiro no escopo atual e depois percorre parent até o escopo global. Assim, uma referência utiliza o símbolo mais próximo que tenha sido declarado.

Exemplo:

// escopo global:
int valor;

// escopo interno:
float valor;
Dentro do escopo interno, a consulta encontra float valor. Fora dele, encontra int valor.

5.5 Entrada e saída de escopos
symbol_table_enter_scope() cria um escopo filho do atual.

symbol_table_leave_scope() retorna ao escopo pai e libera as entradas que pertencem ao escopo encerrado. A função deve rejeitar a tentativa de sair do escopo global.

6. Política de Memória
O módulo será responsável por copiar o nome recebido em symbol_table_insert(). Dessa forma, a tabela não dependerá do tempo de vida da string original produzida pelo lexer ou pela AST.

A destruição deverá ocorrer em dois níveis:

symbol_table_leave_scope() libera os símbolos do escopo que terminou;

symbol_table_destroy() libera o escopo global e qualquer estrutura restante.

As funções de consulta retornam ponteiros de leitura (const Symbol *). O chamador não deverá liberar nem modificar essas entradas.

7. Integração Planejada com o Parser e a Análise Semântica
A tabela de símbolos não deve ser acoplada diretamente às regras léxicas. O lexer continua responsável por reconhecer identificadores e produzir o token ID.

A integração deverá ocorrer nas ações semânticas do Bison ou em uma etapa semântica posterior:

A declaração int contador; é reconhecida pelo parser;

A ação semântica converte o tipo sintático para SymbolType;

A ação chama symbol_table_insert();

Uma inserção duplicada gera diagnóstico semântico;

Uma referência a contador usa symbol_table_lookup();

Uma consulta sem resultado gera diagnóstico de identificador não declarado.

A tabela deve ser criada antes do início da análise e destruída ao final, preferencialmente pelo driver ou pelo módulo semântico. A definição final desse ciclo de vida deve ser tomada junto com a arquitetura da AST.

8. Diagnósticos Esperados
A tabela deve fornecer informação suficiente para mensagens como:

Erro semântico: identificador 'total' não declarado na linha 4
Erro semântico: redeclaração de 'total' na linha 7
A formatação final das mensagens pertence à camada semântica. O módulo da tabela deve apenas indicar sucesso, ausência do símbolo ou conflito de declaração.

9. Decisões que Ficam para a Implementação
Tabela hash ou lista encadeada;

Tamanho inicial e política de redimensionamento;

Permissão de sombreamento em escopos internos;

Representação de parâmetros e assinaturas de funções;

Armazenamento de valor inicial ou apenas de metadados;

Estratégia de preservação dos escopos para a AST e para diagnósticos posteriores.

Essas decisões não alteram a interface conceitual descrita neste documento.

10. Relação com as Próximas Etapas
A arquitetura foi pensada para permitir a evolução do projeto:

Sprint 5: implementar symbol_table.h e symbol_table.c;

Análise Semântica: verificar declarações, referências e tipos;

Escopos: integrar blocos compostos e funções quando a gramática for expandida;

Interpretador: consultar informações de símbolos durante a execução, caso a arquitetura da AST necessite.

Até que a implementação seja adicionada, este documento representa a especificação técnica do módulo, não uma declaração de que os arquivos C já existem.