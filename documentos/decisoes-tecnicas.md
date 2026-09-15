# Decisões Técnicas (A SER PREENCHIDO)

Esta página registra as justificativas arquiteturais e escolhas de implementação adotadas pela equipe para o desenvolvimento do interpretador.

## 1. Escopo da Linguagem Interpretada
* **Tipos de dados suportados:** `[Ex: int, float]`
* **Estruturas de controle:** `[Ex: if/else, while]`
* **Operadores:** `[Ex: Aritméticos (+, -, *, /), Relacionais (==, !=, <, >)]`
* **Entrada/Saída:** `[Ex: Suporte básico a printf]`

## 2. Ferramentas e Padrões Adotados

### Análise Léxica e Sintática (Flex & Bison)
Escolha do **Flex** e **Bison** para automatizar a geração do scanner e parser LR, garantindo integração nativa com a linguagem C.

### Representação Intermediária via AST
Opção por representar o programa como uma Árvore Sintática Abstrata (AST) dinamicamente alocada na memória, permitindo percursos múltiplos (validação semântica, otimizações e interpretação direta).

### Gerenciamento de Escopo e Tabela de Símbolos
Implementação da Tabela de Símbolos via `[Ex: Pilha de Tabelas Hash / Lista Encadeada]`, permitindo suporte a escopos estáticos e aninhamento de blocos `{}`.