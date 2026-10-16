# Plano de Ação Pós-P1

Planejamento estratégico para a condução da Fase 2 do interpretador da linguagem C, cobrindo o desenvolvimento do Back-End, Execução e Otimizações.

---

## 1. Metas Principais da Fase 2

A segunda metade do projeto focará em transformar a árvore sintática gerada em uma execução funcional:

1. **Análise Semântica Integrada (Sprint 8):**
   - Travessia da AST para verificação de tipos e compatibilidade de operandos.
   - Detecção e emissão de alertas para variáveis não declaradas ou redeclaradas.

2. **Interpretador & Runtime Environment (Sprint 9):**
   - Implementação do ambiente de execução (`Environment`) com gerenciamento dinâmico de variáveis.
   - Avaliação direta de expressões aritméticas e atribuições na AST.

3. **Otimização de Código - Constant Folding (Sprint 10):**
   - Travessia para simplificação de nós em tempo de compilação (ex.: `3 + 5` $\rightarrow$ `8`).

4. **Fluxo de Controle e Entrada/Saída (Sprint 11):**
   - Execução de ramos condicionais (`if/else`) e laços (`while`).
   - Suporte ao comando de saída `printf()`.

---

## 2. Estratégia de Ajuste de Cronograma

Para manter todas as entregas em dia e absorver os atrasos das Sprints 5 e 6:

- **Encaixe Continuo de Tasks:** As frentes de desenvolvimento (DEV) e documentação (DOC) atuarão em ritmo acelerado nas Sprints 7 e 8 para alinhar o repositório com o calendário do plano de curso.
- **Rigor no Code Freeze:** Congelamento do código mantido na Sprint 15 para assegurar estabilidade total antes das entrevistas presenciais e defesa final.