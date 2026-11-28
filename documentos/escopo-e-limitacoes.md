# Escopo Não Implementado e Limitações Conhecidas

Este documento consolida os recursos que ficaram fora da entrega final (*Code Freeze* — Sprint 15) por **decisão técnica da equipe** ou **restrição de prazo**, registrando as justificativas e o comportamento atual do interpretador.

---

## 1. Recursos retirados do escopo (decisão técnica)

Durante a apresentação do Ponto de Controle 2 (P2), foi apontado o risco de prazo e complexidade caso a equipe tentasse implementar todo o backlog de expansão antes do *Code Freeze* (Feedback **F1** em [Status do Projeto](status-projeto.md#11-feedbacks-do-p2-e-priorizacao-do-backlog)). A equipe optou por priorizar a estabilização do núcleo já funcional:

| Recurso | Origem planejada | Motivo de não inclusão | Alternativa no subconjunto atual |
|---|---|---|---|
| **Definição e chamada de funções** | Sprint 13 | Exige tabela de funções, parâmetros, escopo por chamada e pilha de ativação em todas as camadas (gramática, semântica e runtime), com alto risco de regressão antes do prazo. | Escrever comandos diretamente no nível global ou em blocos `{ ... }` (o interpretador não exige `main()`). |
| **Instrução `return`** | Sprint 13 | Depende diretamente do suporte a chamadas de função e pilha de execução. | Não aplicável sem funções. |
| **Laço `for` e operadores `++` / `--`** | Sprint 13 | Mantidos como melhorias opcionais condicionadas à conclusão da estabilização (escopos locais, exceções de runtime e Valgrind); não entraram a tempo do *Code Freeze*. | Usar `while` combinado com atribuição explícita (`i = i + 1;`). |
| **Atribuição como expressão** | Sprint 13 | Baixa prioridade no backlog frente à estabilização; a gramática trata atribuição apenas como comando (`id = expr;`). | Usar atribuições em comandos separados. |
| **Flag `--tokens` no driver** | Suíte `testes/1_lexico/` | O driver `src/main.c` foi focado no pipeline completo (`--optimize` / `-O` e `--print-ast`), sem modo isolado de dump de tokens. | A análise léxica é exercitada integrada ao parser pela suíte `bash testes/executar_testes.sh`. |

---

## 2. Limitações conhecidas por prazo / simplificação

Os comportamentos abaixo permanecem na versão congelada do interpretador e foram documentados para transparência técnica:

### 2.1 Tipos no runtime, otimizador e análise semântica
- **Tipo dinâmico no ambiente de execução:** o valor armazenado em `Environment` acompanha o resultado da última atribuição ou literal avaliado, podendo divergir do tipo declarado originalmente na tabela de símbolos (`data_type`, preservado na AST em `string_value`).
- **Divisão inteira no otimizador (`--optimize`):** `optimize_ast()` em `src/optimizer.c` avalia `/` em ponto flutuante (`double`) mesmo quando ambos os operandos são inteiros, enquanto o runtime sem `--optimize` executa divisão inteira. Por isso, uma expressão como `3 / 2 * 2` resulta em `2` sem `--optimize` e `3` com `--optimize`.
- **Tratamento de `char`:** literais de caractere (`CHARLIT`) são convertidos em `NODE_STRING_LITERAL` pelo parser e armazenados como `VAL_STRING` no runtime (exibidos como `string` no estado final).
- **Cobertura parcial de checagem semântica:** a análise verifica uso de variável não declarada, atribuição a não declarada, redeclaração no mesmo escopo e perda de precisão `float` → `int` em atribuições (`SEM-04`), mas não valida tipos em inicializadores de declaração, condições de `if`/`while` nem regras específicas para `char` e `void`.

### 2.2 Diagnósticos e localização
- **Comentário não terminado (`11_comentario_aberto.c`):** o analisador léxico reporta o erro em `stderr`, mas o erro não é propagado para abortar `yyparse()`, podendo encerrar com código de saída `0`.
- **Linha e coluna nos erros semânticos:** a análise semântica roda após o parsing e lê `yylineno`, o que faz a linha reportada apontar para o final do arquivo e sem informação de coluna.

---

## 3. Referências

- [Status do Projeto](status-projeto.md) — resultados da suíte, pendências e decisões do P2 (`D1`–`D4`).
- [Decisões Técnicas](decisoes-tecnicas.md) — arquitetura da AST, tabela de símbolos e módulos em `src/`.
- [Análise Semântica](analise-semantica.md) — regras implementadas e diagnósticos `SEM-01` a `SEM-04`.
- [Interpretador e Runtime](interpretador.md) — funcionamento do `Environment`, escopos locais e erros de execução.
