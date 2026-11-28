# Status do Projeto

Visão consolidada do interpretador da linguagem C: entregas por Sprint, resultados dos testes, estado do runtime, débitos técnicos, plano de quitação e registro de problemas.

**Posição atual:** Sprint 14 integrada · **Última atualização:** 27/11/2026 · **Próximo marco:** validação do Code Freeze (Sprint 15)

---

## 1. Resumo

| Item | Situação |
|---|---|
| Pipeline (léxico → sintático → semântico → runtime) | ✅ Completo e integrado |
| Otimizador (*Constant Folding*) | ✅ Implementado (Sprint 10) |
| Fluxo de controle (`if`/`else`, `while`) e `printf()` | ✅ Implementado (Sprint 11) |
| Suíte de testes válidos | ✅ 25 de 25 passam pelos critérios da suíte |
| Suíte de testes inválidos | ⚠️ 26 de 26 passam pelos critérios da suíte, mas 2 retornam 0 |
| Documentação técnica | 🔄 Em fechamento (Issues #43 e #59) |
| Validação de memória (Valgrind) | 🔄 Pendente (Issue #61) |

---

## 2. Avaliações P1 e P2

- **Ponto de Controle 1 (P1):** feedback positivo quanto à organização da equipe, à clareza da documentação no GitHub Pages e à estrutura de rodízio cruzado por pares.
- **Ponto de Controle 2 (P2):** apresentação do pipeline completo até o ambiente de runtime e o otimizador (*Constant Folding*). Foram mapeadas as entregas finais necessárias para a estabilização até a Sprint 15 (*Code Freeze*).

---

## 3. Resultados por Sprint

| Sprint | Entrega principal | Situação |
|---|---|---|
| 1 a 7 | Ambiente, léxico, parser, AST, tabela de símbolos e entrega do P1 | ✅ Concluídas |
| 8 | Análise semântica (`semantic.c`): tipos, variáveis não declaradas e redeclaradas | ✅ Concluída |
| 9 | Interpretador e ambiente de runtime (`interpreter.c`, `Environment`) | ✅ Concluída |
| 10 | Otimização por *Constant Folding* (`optimizer.c`) | ✅ Concluída |
| 11 | Execução de `if`/`else` e `while`, comando `printf()` | ✅ Concluída |
| 12 | Status consolidado, Valgrind e estabilização do runtime | 🔄 Em andamento |
| 13 | Funções e chamadas, `for`, `++`/`--`, `return`, atribuição como expressão | ⏳ Planejada |
| 14 | Escopos locais, curto-circuito e divisão por zero | ✅ Código integrado pelo PR #80 |
| 15 | *Code Freeze* | ⏳ Planejada |

Detalhamento por Sprint: [Planejamento das Sprints](planejamento-sprints.md).

---

## 4. Funcionalidades disponíveis

- Declarações de variáveis `int`, `float` e `char`, com inicialização.
- Expressões aritméticas com precedência e operadores (cobertos por `07_precedencia.c` e `11_operadores.c`).
- Atribuições e literais, incluindo strings e comentários.
- Controle de fluxo: `if`, `else`, `else if`, `if` aninhado e `while`.
- `printf()` com `%d`, `%s`, `%c`, `%%`, com e sem argumentos.
- Escopo de blocos com tabela de símbolos por nível.
- Análise semântica com erros e avisos (ex.: conversão `float` → `int`).
- Otimização por *Constant Folding*.

---

## 5. Resultados dos testes

Validação na `main` após o PR #80 (`f18edb5`), com compilação limpa e comparação das saídas `.exp` quando presentes.

| Grupo | Resultado |
|---|---|
| Válidos | 25 de 25 sem erro |
| Inválidos | 26 de 26 pelos critérios da suíte. 24 retornaram código 1 e 2 retornaram 0 com diagnóstico |
| Léxicos | 0 de 30 na suíte de tokens. O driver não oferece `--tokens` |

O teste `11_comentario_aberto.c` ainda emite erro léxico e termina com código 0. O teste `18_aviso_float_para_int.c` emite um aviso, por isso o código 0 é esperado e o caso deve ser reclassificado. A suíte aceita diagnósticos em ambos os casos.

---

## 6. Estado do runtime

**Funciona:**

- Ambiente de execução com variáveis dinâmicas e estado final impresso ao término.
- Avaliação de expressões, precedência e operadores.
- `if`/`else`, `else if`, `while` e laços com `if` interno.
- `printf()` com formatação e múltiplos argumentos.
- Escopos locais por bloco, com sombreamento e remoção das variáveis locais ao sair.
- Curto-circuito de `&&` e `||`. Divisão inteira, divisão de ponto flutuante e resto por zero geram erro de runtime e código de saída 1.
- Redeclaração no mesmo escopo é rejeitada na análise semântica.

**Limitações conhecidas:**

- Literais decimais são reconhecidos como `float`. O tipo exibido no runtime acompanha o valor armazenado e pode divergir do tipo declarado.
- Variáveis `char` são tratadas como `string` (ex.: `c (string) = "a"`).
- Comentário não terminado ainda pode retornar sucesso (ver seção 5).
- Validação de vazamentos de memória ainda não concluída (Issue #61).

Para o detalhamento completo do que foi retirado do escopo ou mantido como limitação conhecida, consulte [Escopo e Limitações](escopo-e-limitacoes.md).

---

## 7. Débitos técnicos e pendências

A `main` inclui o pipeline apresentado no P2 e as correções da Sprint 14 para escopos locais, curto-circuito e operações com divisor zero. A validação de memória e a consolidação da documentação seguem pendentes.

### Pendências ativas

| Pendência | Origem | Situação |
|---|---|---|
| Artigos de regras de otimização e gráficos de redução de nós | Issue #43 (Sprint 10) | 🔄 Em aberto |
| Documentação da gestão de pilha e suíte de 10+ testes | Issue #59 (Sprint 11) | 🔄 Em aberto |
| Execução completa da suíte sob o **Valgrind** (zero *memory leaks*) e estabilização do runtime | Issue #61 (Sprint 12) | 🔄 Em aberto |
| Sincronização do status no GitHub Pages | Issues #60 e #62 | 🔄 Em andamento (esta página) |
| Propagar erro de comentário não terminado para o código de saída | Teste inválido 11 | 🔄 A tratar |
| Reclassificar `18_aviso_float_para_int.c` | Resultados dos testes | 🔄 A decidir |
| Suporte real a `float` e `char` no runtime | Limitações do runtime | ⏳ Avaliar prioridade |

### Expansão de recursos (Sprint 13)

Funções e chamadas, laço `for`, incremento e decremento (`++`/`--`), instrução `return` e atribuição como expressão.

---

## 8. Plano de quitação (Sprints 12 a 15)

1. **Quitação documental (Sprints 11/12):** sincronização do status no GitHub Pages (Issues #60 e #62) e fechamento dos documentos técnicos de otimização e testes.
2. **Estabilização de memória (Sprint 12):** testes sistemáticos com Valgrind e ajustes de gerenciamento de escopos locais.
3. **Fechamento (Sprints 14 e 15):** escopos locais e tratamento de divisão e resto por zero já estão integrados. Para o *Code Freeze*, restam a validação de memória, a revisão dos diagnósticos restantes e a documentação. Funções, chamadas e `return` ficam como trabalho futuro.

---

## 9. Plano pós-P1 (referência)

Planejamento original da Fase 2 do interpretador, cobrindo Back-End, execução e otimizações.

| Sprint | Meta |
|---|---|
| 8 | **Análise semântica integrada:** travessia da AST para verificação de tipos e compatibilidade de operandos; alertas para variáveis não declaradas ou redeclaradas |
| 9 | **Interpretador e runtime:** ambiente de execução (`Environment`) com gerenciamento dinâmico de variáveis; avaliação direta de expressões aritméticas e atribuições na AST |
| 10 | **Constant Folding:** simplificação de nós em tempo de compilação (ex.: `3 + 5` → `8`) |
| 11 | **Fluxo de controle e saída:** execução de `if`/`else` e `while`; comando `printf()` |

**Estratégia de ajuste de cronograma:** para absorver os atrasos das Sprints 5 e 6, as frentes de desenvolvimento (DEV) e documentação (DOC) atuaram em ritmo acelerado nas Sprints 7 e 8, alinhando o repositório ao calendário do plano de curso. O *Code Freeze* foi mantido na Sprint 15 para assegurar estabilidade total antes das entrevistas presenciais e da defesa final.

---

## 10. Problemas encontrados e soluções adotadas

Registro contínuo dos desafios técnicos enfrentados pela equipe e das soluções aplicadas.

| Problema | Solução |
|---|---|
| Comandos encadeados não eram executados corretamente | Adição de uma função auxiliar que percorre a lista de comandos, executando-os em sequência |
| Incompatibilidade do tratamento de escopo com a gramática BNF | 🔄 Em alinhamento pela equipe (ver seção 7) |

Problemas ainda **em aberto** estão na seção 7.

---
 
## 11. Feedbacks do P2 e priorização do backlog
 
Consolidação dos feedbacks recebidos na apresentação do P2 e priorização das melhorias finais do interpretador, considerando impacto, urgência, esforço e relevância para a entrega final (*Code Freeze* na Sprint 15).
 
### 11.1 Feedbacks recebidos
 
| # | Feedback | Tipo | Área afetada | Impacto |
|---|---|---|---|---|
| F1 | Cobrir funções (definição e chamada) seria muito complexo para o tempo restante do projeto; sugerida a redução do escopo | Risco de escopo e cronograma | Gramática, parser, análise semântica e runtime | Alto |
 
 
### 11.2 Backlog priorizado
 
Esforço e urgência são estimativas da equipe e podem ser ajustados.
 
| Prioridade | Item | Origem | Impacto | Urgência | Esforço |
|---|---|---|---|---|---|
| Alta | Validação completa com Valgrind (zero *memory leaks*) | Issue #61 | Alto | Alta | Médio |
| Alta | Retornar falha após erro de comentário não terminado | Teste inválido 11 | Alto | Alta | Baixo |
| Alta | Alinhar o escopo com a BNF e reclassificar `18_aviso_float_para_int.c` | Seções 5 e 10 | Alto | Alta | Baixo a médio |
| Alta | Fechar a documentação técnica pendente | Issues #43 e #59 | Médio | Alta | Baixo |
| Média | Laço `for` e incremento/decremento (`++`/`--`) | Sprint 13 original | Médio | Média | Médio |
| Baixa | Suporte real a `float` e `char` no runtime (ou documentar como limitação) | Limitações do runtime | Médio | Baixa | Médio |
| Baixa | Atribuição como expressão | Sprint 13 original | Baixo | Baixa | Médio |
| Fora do escopo | Funções, chamadas e `return` | Feedback F1 | Alto | — | Alto |
 
### 11.3 Decisões e justificativas
 
| # | Decisão | Justificativa | Status |
|---|---|---|---|
| D1 | Retirar funções, chamadas e `return` do escopo da entrega final e registrá-los como trabalho futuro | Atende ao feedback F1. Funções exigem escopo por função, pilha de chamadas e mudanças simultâneas na gramática, no parser, na análise semântica e no runtime. O risco de desestabilizar o que já funciona é incompatível com o prazo até o *Code Freeze* | Proposta |
| D2 | Priorizar a estabilização (Valgrind, rejeição correta de erros, escopo e BNF) antes de qualquer novo recurso | A defesa final avalia o que existe e funciona. Corrigir bugs conhecidos tem mais retorno que ampliar a linguagem | Proposta |
| D3 | Manter `for` e `++`/`--` como melhorias opcionais, condicionadas à conclusão dos itens de prioridade alta | Reaproveitam estruturas já existentes (`while`, atribuição), com esforço e risco menores que funções | Proposta |
| D4 | Decidir, até a Sprint 14, entre implementar `float`/`char` no runtime ou documentá-los como limitação conhecida | Evita investir esforço sem impacto claro na demonstração final | Pendente |
 
### 11.4 Impacto no cronograma
 
Com o escopo reduzido, as Sprints 13 a 15 ficam assim:
 
- **Sprint 13:** estabilização (itens de prioridade alta) e, se houver folga, `for` e `++`/`--`.
- **Sprint 14:** escopos locais e tratamento de divisão e resto por zero integrados. O status da sprint ainda precisa ser publicado no GitHub Pages.
- **Sprint 15:** *Code Freeze*.