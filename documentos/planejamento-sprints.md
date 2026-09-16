# Planejamento das Sprints

Interpretador de um subconjunto de C — Compiladores 1 · Grupo 10 · 2026.2

Metodologia: Scrum/XP, sprints de uma semana (12/08/2026 a 02/12/2026).

> **Situação atual (16/09/2026):** Sprint 6 em andamento (16/09 a 23/09) — Impressão da AST e preparação do P1.

Legenda de status nas tarefas: `[ ]` pendente · `[x]` concluído.

---

## Estratégia de Rodízio 

Para garantir rastreabilidade, aprendizado contínuo e transferência de conhecimento sem perda de contexto, a equipe opera sob um modelo de **Rodízio Cruzado por Pares**:

- **Distribuição por Sprint:** Em cada sprint, **2 membros** atuam na frente de **Documentação/Gestão (DOC)** e **3 membros** na frente de **Desenvolvimento (DEV)**.
- **Passagem de Bastão (Pares Sobrepostos):** A cada transição de sprint, apenas **1 integrante** da Documentação troca de lugar com **1 integrante** do Código. Dessa forma, a Documentação sempre mantém 1 membro veterano (da sprint anterior) e 1 membro novo, garantindo que o conhecimento do código recente seja documentado com precisão e sem ruídos.
- **Atualização Contínua do Pages:** Em **todas as sprints**, a dupla responsável pela Documentação tem a obrigação de atualizar a situação atual e o status das tarefas na documentação oficial publicada no **GitHub Pages**.

---

## Mapeamento do Rodízio da Equipe

| Sprint | Dupla de Documentação (DOC) | Trio de Desenvolvimento (DEV) |
| :---: | :--- | :--- |
| **Sprint 1** | Nicole Jovita & Bernardo Campos | Danielly Mendes, Rafael Laube, Paulo Nina |
| **Sprint 2** | Nicole Jovita & Danielly Mendes | Bernardo Campos, Rafael Laube, Paulo Nina |
| **Sprint 3** | Danielly Mendes & Rafael Laube | Nicole Jovita, Bernardo Campos, Paulo Nina |
| **Sprint 4** | Rafael Laube & Paulo Nina | Nicole Jovita, Danielly Mendes, Bernardo Campos |
| **Sprint 5** | Paulo Nina & Bernardo Campos | Nicole Jovita, Danielly Mendes, Rafael Laube |
| **Sprint 6** | Bernardo Campos & Nicole Jovita | Danielly Mendes, Rafael Laube, Paulo Nina |
| **Sprint 7** | Nicole Jovita & Danielly Mendes | Bernardo Campos, Rafael Laube, Paulo Nina |
| **Sprint 8** | Danielly Mendes & Rafael Laube | Nicole Jovita, Bernardo Campos, Paulo Nina |
| **Sprint 9** | Rafael Laube & Paulo Nina | Nicole Jovita, Danielly Mendes, Bernardo Campos |
| **Sprint 10** | Paulo Nina & Bernardo Campos | Nicole Jovita, Danielly Mendes, Rafael Laube |
| **Sprint 11** | Bernardo Campos & Nicole Jovita | Danielly Mendes, Rafael Laube, Paulo Nina |
| **Sprint 12** | Nicole Jovita & Danielly Mendes | Bernardo Campos, Rafael Laube, Paulo Nina |
| **Sprint 13** | Danielly Mendes & Rafael Laube | Nicole Jovita, Bernardo Campos, Paulo Nina |
| **Sprint 14** | Rafael Laube & Paulo Nina | Nicole Jovita, Danielly Mendes, Bernardo Campos |
| **Sprint 15** | Paulo Nina & Bernardo Campos | Nicole Jovita, Danielly Mendes, Rafael Laube |
| **Sprint 16** | **Toda a Equipe** (Defesa e Apresentação) | **Toda a Equipe** (Defesa e Apresentação) |

---

## Visão Geral das Sprints

| Sprint | Período | Foco | Entregas-chave |
|:------:|---------|------|----------------|
| 1 | 12/08 – 19/08 | Escopo da linguagem, setup e análise léxica inicial | Repo GitHub · `lexer.l` inicial · README |
| 2 | 19/08 – 26/08 | Léxico completo e gramática inicial no Bison | `lexer.l` completo · `parser.y` inicial · Makefile |
| 3 | 26/08 – 02/09 | Análise sintática básica | Gramática de declarações/expressões · `main.c` |
| 4 | 02/09 – 09/09 | Parser completo e erros sintáticos | `if`/`else`/`while`/blocos · `yyerror()` com linha e coluna |
| 5 | 09/09 – 16/09 | AST e tabela de símbolos | `ast.c`/`ast.h` · `symbol_table.c`/`symbol_table.h` |
| 6 | 16/09 – 23/09 | Impressão da AST e preparação do P1 | `print_ast()` · Formulário P1 · Slides |
| 7 | 23/09 – 30/09 | Apresentação P1 e ajustes de feedback | Apresentação P1 · Relatório de débitos técnicos |
| 8 | 30/09 – 07/10 | Análise semântica integrada à AST | `semantic.c`/`semantic.h` · Checagem de tipos/escopos |
| 9 | 07/10 – 14/10 | Interpretador (execução direta da AST) | `interpreter.c`/`interpreter.h` · Runtime environment |
| 10 | 14/10 – 21/10 | Otimização na interpretação da AST | `optimizer.c`/`optimizer.h` · Constant folding |
| 11 | 21/10 – 28/10 | Fluxo de controle e programas completos | `if`/`while` no interpretador · 10+ testes em C |
| 12 | 28/10 – 04/11 | Consolidação e preparação do P2 | Interpretador estável · Formulário P2 · Slides |
| 13 | 04/11 – 11/11 | Apresentação P2 e refinamento do backlog | Apresentação P2 · Lista de melhorias finais |
| 14 | 11/11 – 18/11 | Polimento, escopos avançados e exceções | Exceções de runtime · Destruição de escopos |
| 15 | 18/11 – 25/11 | Code freeze e documentação final | Repositório congelado · Documentação completa |
| 16 | 25/11 – 02/12 | Entrevistas presenciais e defesa final | Defesa presencial com o professor |

---

## Marcos de Avaliação

| Marco | Data | Sprint | O que entregar |
|-------|------|:------:|----------------|
| **Formulário P1** | até 23/09 | 6 | Formulário P1 preenchido pelo líder |
| **Apresentação P1** | 28/09 ou 30/09 | 7 | Slides (5 min) + demo do pipeline até a AST |
| **Formulário P2** | até 04/11 | 12 | Formulário P2 preenchido (avanços, execução da AST, otimizações) |
| **Apresentação P2** | 09/11 ou 11/11 | 13 | Demo ao vivo do interpretador executando programas C |
| **Code Freeze** | 25/11 | 15 | Repositório finalizado e congelado no GitHub |
| **Entrevistas Finais** | 30/11 e 02/12 | 16 | Defesa presencial — todos os integrantes |

---

## Detalhamento das Sprints

### Sprint 1 — Definição do escopo, setup e análise léxica inicial
- **Período:** 12/08 a 19/08
- **Documentação (DOC):** Nicole Jovita & Bernardo Campos
- **Desenvolvimento (DEV):** Danielly Mendes, Rafael Laube, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Estruturar primeira versão do `README.md` e página inicial do Planejamento no MkDocs | Nicole Jovita |
| [ ] | Documentação | Atualizar status do planejamento e publicar no GitHub Pages | Nicole Jovita & Bernardo Campos |
| [ ] | Escopo/Gestão | Definir subconjunto de C aceito (`int`, `float`, operadores, `if/else`, `while`, `printf`) | Bernardo Campos |
| [ ] | Repositório | Criar repositório GitHub, configurar `.gitignore` e convidar o professor | Danielly Mendes |
| [ ] | Léxico (`lexer.l`) | Implementar ERs para palavras-chave, identificadores e números inteiros | Rafael Laube |
| [ ] | Ambiente/Léxico | Testar ferramentas (GCC, Flex, Bison, Make) e criar contador de linhas/colunas em `lexer.l` | Paulo Nina |

---

### Sprint 2 — Léxico completo e gramática inicial no Bison
- **Período:** 19/08 a 26/08
- **Documentação (DOC):** Nicole Jovita (Veterana) & Danielly Mendes (Nova)
- **Desenvolvimento (DEV):** Bernardo Campos, Rafael Laube, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Escrever a documentação base das Decisões Técnicas e padrão de arquivos | Nicole Jovita |
| [ ] | Documentação | Registrar o tratamento de comentários em bloco e exceções do Flex no MkDocs | Danielly Mendes |
| [ ] | Documentação | Atualizar status das tarefas e sincronizar publicação no GitHub Pages | Nicole Jovita & Danielly Mendes |
| [ ] | Build/Gestão | Criar o `Makefile` de automação e apoiar integração de `%union` e `%token` | Bernardo Campos |
| [ ] | Léxico avançado | Finalizar `lexer.l` com suporte a floats, comentários e operadores compostos | Rafael Laube |
| [ ] | Parser inicial | Criar `parser.y` inicial no Bison garantindo compilação limpa via `bison -d` | Paulo Nina |

---

### Sprint 3 — Análise sintática básica
- **Período:** 26/08 a 02/09
- **Documentação (DOC):** Danielly Mendes (Veterana) & Rafael Laube (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Bernardo Campos, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar em Decisões Técnicas a estrutura da BNF e regras de precedência | Danielly Mendes |
| [ ] | Documentação | Escrever manual de execução do driver `main.c` e primeiros testes `.c` | Rafael Laube |
| [ ] | Documentação | Sincronizar o avanço da Sprint 3 no GitHub Pages | Danielly Mendes & Rafael Laube |
| [ ] | Gramática | Implementar regras para declaração de variáveis (`type id;` e `type id = expr;`) | Nicole Jovita |
| [ ] | Gramática | Desenvolver regras para expressões aritméticas e parênteses | Paulo Nina |
| [ ] | Driver/Gestão | Implementar `main.c` integrando `yyparse()` e resolver conflitos de precedência | Bernardo Campos |

---

### Sprint 4 — Parser completo e erros sintáticos
- **Período:** 02/09 a 09/09
- **Documentação (DOC):** Rafael Laube (Veterano) & Paulo Nina (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Danielly Mendes, Bernardo Campos

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar no MkDocs a resolução do conflito *Dangling Else* com `%nonassoc` | Rafael Laube |
| [ ] | Documentação | Registrar a arquitetura de relatório de erros sintáticos (linha/coluna) | Paulo Nina |
| [ ] | Documentação | Sincronizar o status atual no GitHub Pages | Rafael Laube & Paulo Nina |
| [ ] | Gramática | Expandir gramática para comandos compostos `{}` e estruturas `if`/`else` | Nicole Jovita |
| [ ] | Gramática/Erros | Implementar laços `while` e customizar `yyerror()` com linha e coluna | Danielly Mendes |
| [ ] | Erros/Gestão | Habilitar `%define parse.error verbose` e testar mensagens detalhadas de erro | Bernardo Campos |

---

### Sprint 5 — AST e tabela de símbolos
- **Período:** 09/09 a 16/09
- **Documentação (DOC):** Paulo Nina (Veterano) & Bernardo Campos (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Danielly Mendes, Rafael Laube

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Escrever documentação técnica da estrutura da AST (`ast.h`/`ast.c`) | Paulo Nina |
| [ ] | Documentação | Registrar a arquitetura da Tabela de Símbolos (`symbol_table.h`/`symbol_table.c`) | Bernardo Campos |
| [ ] | Documentação | Atualizar o progresso da Sprint 5 no GitHub Pages | Paulo Nina & Bernardo Campos |
| [ ] | AST | Criar módulo `ast.h`/`ast.c` e conectar ações semânticas no Bison | Nicole Jovita |
| [ ] | Tabela de Símbolos | Implementar `symbol_table.c` com `insert_symbol()` e `lookup_symbol()` | Danielly Mendes |
| [ ] | Integração | Fazer o parser retornar a raiz da AST ao final de `yyparse()` | Rafael Laube |

---

### Sprint 6 — Visualização da AST e preparação do P1
- **Período:** 16/09 a 23/09
- **Documentação (DOC):** Bernardo Campos (Veterano) & Nicole Jovita (Nova)
- **Desenvolvimento (DEV):** Danielly Mendes, Rafael Laube, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Formulário P1 | Preencher o formulário do P1 com a consolidação da equipe e entregas | Bernardo Campos |
| [ ] | Apresentação | Criar slides (5 min) e organizar a suíte de testes do front-end | Nicole Jovita |
| [ ] | Documentação | Atualizar o status consolidado pré-P1 no GitHub Pages | Bernardo Campos & Nicole Jovita |
| [ ] | Visualizador | Implementar a função de exibição indentada da AST (`print_ast()`) | Danielly Mendes |
| [ ] | Testes | Elaborar bateria de 5+ testes em C validando erros e geração da AST | Rafael Laube |
| [ ] | Tabela/Ajustes | Adequar a tabela de símbolos para suporte à checagem de escopo inicial | Paulo Nina |

---

### Sprint 7 — Apresentação P1 e ajustes de feedback
- **Período:** 23/09 a 30/09
- **Documentação (DOC):** Nicole Jovita (Veterana) & Danielly Mendes (Nova)
- **Desenvolvimento (DEV):** Bernardo Campos, Rafael Laube, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Registrar relatório de débitos técnicos e feedbacks do professor pós-P1 | Nicole Jovita |
| [ ] | Documentação | Atualizar a página de Problemas e Soluções com o plano de ação | Danielly Mendes |
| [ ] | Documentação | Atualizar o status atual no GitHub Pages | Nicole Jovita & Danielly Mendes |
| [ ] | Apresentação | Realizar a apresentação do P1 e liderar atendimento aos feedbacks | Bernardo Campos |
| [ ] | Refatoração | Ajustar gramática e regras do scanner com base nos apontamentos | Rafael Laube |
| [ ] | Estabilização | Congelar Front-End (Flex + Bison + AST) garantindo ausência de memory leaks | Paulo Nina |

---

### Sprint 8 — Análise semântica integrada à AST
- **Período:** 30/09 a 07/10
- **Documentação (DOC):** Danielly Mendes (Veterana) & Rafael Laube (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Bernardo Campos, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar tabela de compatibilidade de tipos e regras semânticas | Danielly Mendes |
| [ ] | Documentação | Escrever guia de mensagens de erros semânticos no MkDocs | Rafael Laube |
| [ ] | Documentação | Publicar avanço da Sprint 8 no GitHub Pages | Danielly Mendes & Rafael Laube |
| [ ] | Semântica | Implementar travessia na AST (`semantic.c`) para detectar variáveis não declaradas | Nicole Jovita |
| [ ] | Tipagem | Implementar checagem de tipos e coerção implícita simples (`int` → `float`) | Paulo Nina |
| [ ] | Fluxo/Gestão | Validar condições booleanas de `if`/`while` e apoiar regras de escopo | Bernardo Campos |

---

### Sprint 9 — Interpretador (execução direta da AST)
- **Período:** 07/10 a 14/10
- **Documentação (DOC):** Rafael Laube (Veterano) & Paulo Nina (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Danielly Mendes, Bernardo Campos

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar a arquitetura da memória de runtime (`Environment`) em Decisões Técnicas | Rafael Laube |
| [ ] | Documentação | Atualizar o diagrama arquitetural do interpretador no MkDocs | Paulo Nina |
| [ ] | Documentação | Sincronizar atualizações no GitHub Pages | Rafael Laube & Paulo Nina |
| [ ] | Runtime | Desenvolver o módulo `interpreter.c` e a função recursiva `evaluate_expr()` | Nicole Jovita |
| [ ] | Runtime | Implementar a estrutura `Environment` para atualização de variáveis (`NODE_ASSIGN`) | Danielly Mendes |
| [ ] | Execução/Gestão | Implementar a avaliação de nós aritméticos binários em tempo de execução | Bernardo Campos |

---

### Sprint 10 — Otimização na interpretação da AST
- **Período:** 14/10 a 21/10
- **Documentação (DOC):** Paulo Nina (Veterano) & Bernardo Campos (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Danielly Mendes, Rafael Laube

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar regras de *Constant Folding* e simplificação de expressões | Paulo Nina |
| [ ] | Documentação | Elaborar gráficos e relatórios comparativos da redução de nós na AST | Bernardo Campos |
| [ ] | Documentação | Atualizar o progresso da Sprint 10 no GitHub Pages | Paulo Nina & Bernardo Campos |
| [ ] | Otimização | Implementar travessia de *Constant Folding* no módulo `optimizer.c` (ex.: `3 + 5` → `8`) | Nicole Jovita |
| [ ] | Otimização | Implementar eliminação de expressões redundantes (`x + 0`, `x * 1`) | Danielly Mendes |
| [ ] | Otimização | Implementar remoção de código morto em blocos `if (0)` | Rafael Laube |

---

### Sprint 11 — Fluxo de controle e programas completos
- **Período:** 21/10 a 28/10
- **Documentação (DOC):** Bernardo Campos (Veterano) & Nicole Jovita (Nova)
- **Desenvolvimento (DEV):** Danielly Mendes, Rafael Laube, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar em Problemas e Soluções a gestão de pilha em laços profundos | Bernardo Campos |
| [ ] | Documentação | Escrever a documentação dos 10+ programas de teste em C no MkDocs | Nicole Jovita |
| [ ] | Documentação | Publicar status atualizado no GitHub Pages | Bernardo Campos & Nicole Jovita |
| [ ] | Execução | Implementar interpretação de `NODE_IF` executando ramos `then`/`else` | Danielly Mendes |
| [ ] | Execução | Implementar execução de laços `NODE_WHILE` recursivamente | Rafael Laube |
| [ ] | E/S | Integrar comando `printf()` lendo os valores da memória de runtime | Paulo Nina |

---

### Sprint 12 — Consolidação e preparação do P2
- **Período:** 28/10 a 04/11
- **Documentação (DOC):** Nicole Jovita (Veterana) & Danielly Mendes (Nova)
- **Desenvolvimento (DEV):** Bernardo Campos, Rafael Laube, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Formulário P2 | Preencher o formulário do P2 com os avanços da interpretação e otimizações | Nicole Jovita |
| [ ] | Apresentação | Criar slides e roteiro da demonstração ao vivo para o P2 | Danielly Mendes |
| [ ] | Documentação | Atualizar o status consolidado pré-P2 no GitHub Pages | Nicole Jovita & Danielly Mendes |
| [ ] | Qualidade/Gestão | Executar testes de vazamento de memória com *Valgrind* na execução da AST | Bernardo Campos |
| [ ] | Estabilidade | Resolver inconsistências de estado no ambiente de runtime | Rafael Laube |
| [ ] | Testes | Garantir execução estável dos 10+ programas de teste da suíte oficial | Paulo Nina |

---

### Sprint 13 — Apresentação P2 e refinamento do backlog
- **Período:** 04/11 a 11/11
- **Documentação (DOC):** Danielly Mendes (Veterana) & Rafael Laube (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Bernardo Campos, Paulo Nina

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Registrar em Problemas e Soluções os feedbacks da avaliação do P2 | Danielly Mendes |
| [ ] | Documentação | Priorizar e listar melhorias finais para a entrega do projeto no MkDocs | Rafael Laube |
| [ ] | Documentação | Atualizar a situação atual no GitHub Pages | Danielly Mendes & Rafael Laube |
| [ ] | Correções | Resolver bugs de execução apontados durante a demonstração do P2 | Nicole Jovita |
| [ ] | Apresentação | Realizar a apresentação do P2 ao vivo e coordenar backlog de ajustes | Bernardo Campos |
| [ ] | Semântica/Ajustes | Ajustar detalhes finos da análise semântica e escopos | Paulo Nina |

---

### Sprint 14 — Polimento, escopos avançados e exceções
- **Período:** 11/11 a 18/11
- **Documentação (DOC):** Rafael Laube (Veterano) & Paulo Nina (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Danielly Mendes, Bernardo Campos

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Documentar o funcionamento de escopos locais e destruição de variáveis | Rafael Laube |
| [ ] | Documentação | Registrar guia de exceções de runtime no MkDocs | Paulo Nina |
| [ ] | Documentação | Sincronizar as alterações da Sprint 14 no GitHub Pages | Rafael Laube & Paulo Nina |
| [ ] | Exceções | Implementar emissão de mensagens amigáveis de runtime (ex.: divisão por zero) | Nicole Jovita |
| [ ] | Escopo | Garantir destruição correta de variáveis locais ao sair de blocos `{}` | Danielly Mendes |
| [ ] | Refatoração/Gestão | Padronizar código-fonte, remover prints de depuração e organizar arquivos | Bernardo Campos |

---

### Sprint 15 — Code freeze e documentação final
- **Período:** 18/11 a 25/11
- **Documentação (DOC):** Paulo Nina (Veterano) & Bernardo Campos (Novo)
- **Desenvolvimento (DEV):** Nicole Jovita, Danielly Mendes, Rafael Laube

| Status | Frente | Atividade | Responsável |
|:------:|--------|-----------|-------------|
| [ ] | Documentação | Revisar e consolidar as 4 seções principais da documentação | Paulo Nina |
| [ ] | Documentação | Escrever tutorial passo a passo de compilação e execução no `README.md` | Bernardo Campos |
| [ ] | Documentação | Realizar o deploy final e congelar a versão do GitHub Pages | Paulo Nina & Bernardo Campos |
| [ ] | Code Freeze | Executar bateria final de testes de regressão e congelar o código-fonte | Nicole Jovita |
| [ ] | Entrega | Verificar permissões do repositório e gerar arquivo `.zip` se necessário | Danielly Mendes |
| [ ] | Build | Validar compilação limpa do `Makefile` em ambiente zerado | Rafael Laube |

---

### Sprint 16 — Entrevistas presenciais e defesa final
- **Período:** 25/11 a 02/12
- **Alocação:** **Toda a Equipe** (Bernardo Campos, Danielly Mendes, Nicole Jovita, Paulo Nina, Rafael Laube)

| Status | Frente | Atividade | Responsáveis |
|:------:|--------|-----------|--------------|
| [ ] | Alinhamento | Reunião geral: garantir domínio por todos os membros desde o Flex até a execução da AST | Toda a Equipe |
| [ ] | Preparação | Configurar notebooks com o ambiente pronto para execução ao vivo na entrevista | Toda a Equipe |
| [ ] | Defesa | Participar presencialmente da entrevista de defesa do projeto com o professor | Toda a Equipe |
| [ ] | Finalização | Marcar o encerramento do projeto e versão final no GitHub Pages | Nicole Jovita & Bernardo Campos |