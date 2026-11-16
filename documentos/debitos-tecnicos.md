# Relatório de Débitos Técnicos e Evolução

Este documento registra a avaliação das fases do projeto (P1 e P2), o estado dos débitos técnicos identificados e o alinhamento de cronograma com base nos feedbacks recebidos.

---

## 1. Avaliação e Feedbacks (P1 e P2)

- **Ponto de Controle 1 (P1):** Feedback positivo quanto à organização da equipe, clareza da documentação no GitHub Pages e estrutura de rodízio cruzado por pares.
- **Ponto de Controle 2 (P2):** Apresentação do pipeline completo até o ambiente de runtime e otimizador (*Constant Folding*). Foram mapeadas as entregas finais necessárias para estabilização até a Sprint 15 (*Code Freeze*).

---

## 2. Situação Atual e Débitos Técnicos (Sprint 12)

Todo o desenvolvimento técnico previsto até a **Sprint 11** (`semantic.c`, `interpreter.c`, `optimizer.c`, execução de `if`/`while` e `printf()`) foi concluído e integrado ao repositório. Os entregáveis documentais do P2 (Formulário P2, slides e roteiro) também foram finalizados.

### Diagnóstico de Pendências Ativas:
- **Documentação Técnica Remanescente (Sprints 10 e 11):** Finalização dos artigos de regras de otimização e gráficos de redução de nós (Issue #43) e documentação da gestão de pilha e suíte de 10+ testes (Issue #59).
- **Qualidade e Memória (Sprint 12 — Issue #61):** Execução completa da suíte de testes sob o **Valgrind** para garantia de zero vazamentos de memória (*memory leaks*) e estabilização de estado do ambiente de runtime.
- **Expansão de Recursos da Linguagem (Sprint 13):** Suporte a funções e chamadas, laço `for`, incremento/decremento (`++`/`--`), instrução `return` e atribuição como expressão.

---

## 3. Plano de Quitação (Sprints 12 a 15)

1. **Quitação Documental (Sprints 11/12):** Sincronização do status no GitHub Pages (Issues #60 e #62) e fechamento dos documentos técnicos de otimização e testes.
2. **Estabilização de Memória (Sprint 12):** Testes sistemáticos com Valgrind e ajustes de gerenciamento de escopos locais.
3. **Finalização do Escopo (Sprints 13 e 14):** Implementação de funções, `for`, `return` e tratamento de exceções de runtime antes do *Code Freeze* na Sprint 15.