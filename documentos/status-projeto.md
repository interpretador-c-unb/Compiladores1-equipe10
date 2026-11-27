# Status do Projeto

Visão consolidada do interpretador da linguagem C: entregas por Sprint, resultados dos testes, estado do runtime, débitos técnicos, plano de quitação e registro de problemas.

**Posição atual:** estabilização e integração das entregas finais · **Backlog revisado:** 27/11/2026 · **Marco pendente:** Code Freeze (previsto para 25/11)

Esta revisão consolida a priorização da Sprint 13. Os resultados numéricos da seção 5 são o registro de 25/11, não uma nova execução da suíte. As entregas disponíveis em branches separadas estão identificadas na seção 11; sua existência não significa integração à `main` nem publicação no Pages.

---

## 1. Resumo

| Item | Situação |
|---|---|
| Pipeline (léxico → sintático → semântico → runtime) | ✅ Completo e integrado |
| Otimizador (*Constant Folding*) | ✅ Implementado (Sprint 10) |
| Fluxo de controle (`if`/`else`, `while`) e `printf()` | ✅ Implementado (Sprint 11) |
| Suíte de testes válidos | ⚠️ 19 de 21 executam até o fim |
| Suíte de testes inválidos | ⚠️ 17 de 20 são rejeitados como esperado |
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
| 1 a 7 | <!-- TODO: preencher a partir de planejamento-sprints.md --> | |
| 8 | Análise semântica (`semantic.c`): tipos, variáveis não declaradas e redeclaradas | ✅ Concluída |
| 9 | Interpretador e ambiente de runtime (`interpreter.c`, `Environment`) | ✅ Concluída |
| 10 | Otimização por *Constant Folding* (`optimizer.c`) | ✅ Concluída |
| 11 | Execução de `if`/`else` e `while`, comando `printf()` | ✅ Concluída |
| 12 | Status consolidado, Valgrind e estabilização do runtime | 🔄 Em andamento |
| 13 | Apresentação P2 e priorização do backlog final | 🔄 Priorização documentada; publicação e ajustes pendentes |
| 14 | Finalização do escopo e tratamento de exceções de runtime | ⏳ Planejada |
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

### 5.1 Testes válidos (`testes/validos`)

| Total | Executaram até o fim | Abortaram |
|---|---|---|
| 21 | 19 | 2 |

Abortaram com erro semântico:

| Arquivo | Erro | Observação |
|---|---|---|
| `03_declaracoes.c` | Variável `z` usada sem declaração (linha 11) | `z` é declarada e usada dentro do mesmo bloco; falso erro semântico na versão avaliada |
| `12_escopo_blocos.c` | Variável `y` usada sem declaração (linha 8) | `y` é declarada e usada dentro do mesmo bloco; falso erro semântico na versão avaliada |

Os arquivos `03` e `12` são válidos e devem permanecer nessa categoria. O problema é a perda dos símbolos locais antes da análise semântica. A correção está no commit `ab5ee01`, na branch `fix/sprint12-escopos`; após sua integração, repetir a suíte e atualizar os totais. O teste `15_variavel_fora_de_escopo.c` continua sendo o caso negativo de uso fora do bloco.

### 5.2 Testes inválidos (`testes/invalidos`)

| Total | Rejeitados como esperado | Não rejeitados |
|---|---|---|
| 20 | 17 | 3 |

**Rejeitados corretamente (17):**

- Erros léxicos ou sintáticos (13): `01` a `07`, `10`, `12`, `13`, `14`, `16`, `19`.
- Erros semânticos (4): `08_var_nao_declarada`, `15_variavel_fora_de_escopo`, `17_atribuicao_nao_declarada`, `erro_semantico_tipo`.

**Não rejeitados (3):**

| Arquivo | Comportamento atual | Esperado |
|---|---|---|
| `09_redeclaracao.c` | Emite "variável já declarada", mas a análise semântica conclui sem erros e o programa executa | Abortar com erro semântico |
| `11_comentario_aberto.c` | Emite erro léxico de comentário não terminado, mas segue para análise e execução | Abortar com erro léxico |
| `18_aviso_float_para_int.c` | Emite aviso de perda de precisão e executa | Coerente com um **aviso**, não um erro; o arquivo deveria ser reclassificado |

---

## 6. Estado do runtime

**Funciona:**

- Ambiente de execução com variáveis dinâmicas e estado final impresso ao término.
- Avaliação de expressões, precedência e operadores.
- `if`/`else`, `else if`, `while` e laços com `if` interno.
- `printf()` com formatação e múltiplos argumentos.
- Escopos locais por bloco.

**Limitações conhecidas:**

- Variáveis `float` são armazenadas e exibidas como `int` (ex.: `pi (float)` aparece como `pi (int) = 3`).
- Variáveis `char` são tratadas como `string` (ex.: `c (string) = "a"`).
- O tipo declarado não força conversão do valor armazenado no runtime; `float`/`char` não têm suporte completo equivalente ao de C. Literais decimais podem ser classificados como inteiros porque o avaliador consulta `name`, que o construtor do literal numérico não preenche. A decisão da equipe é documentar essas limitações nesta entrega.
- Redeclaração e comentário não terminado não interrompem a execução (ver seção 5.2).
- Validação de vazamentos de memória ainda não concluída (Issue #61).

---

## 7. Débitos técnicos e pendências

Todo o desenvolvimento técnico previsto até a **Sprint 11** (`semantic.c`, `interpreter.c`, `optimizer.c`, execução de `if`/`while` e `printf()`) foi concluído e integrado ao repositório. Os entregáveis documentais do P2 (Formulário P2, slides e roteiro) também foram finalizados.

### Pendências ativas

| Pendência | Origem | Situação |
|---|---|---|
| Artigos de regras de otimização e gráficos de redução de nós | Issue #43 (Sprint 10) | 🔄 Em aberto |
| Documentação da gestão de pilha e suíte de 10+ testes | Issue #59 (Sprint 11) | 🔄 Em aberto |
| Execução completa da suíte sob o **Valgrind** (zero *memory leaks*) e estabilização do runtime | Issue #61 (Sprint 12) | 🔄 Em aberto |
| Sincronização do status no GitHub Pages | Issues #60 e #62 | 🔄 Em andamento (esta página) |
| Redeclaração e comentário não terminado devem abortar a execução | Testes inválidos 09 e 11 | 🔄 A tratar |
| Manter `03` e `12` válidos; separar teste de aviso `18` dos erros obrigatórios | Resultados dos testes | 🔄 Ajustar suíte e expectativas |
| Suporte real a `float` e `char` no runtime | Limitações do runtime | ⏳ Avaliar prioridade |

### Expansão de recursos (Sprint 13)

O planejamento original incluía funções, chamadas, `for`, `++`/`--`, `return` e atribuição como expressão. A equipe decidiu deixar funções, chamadas e `return` para implementação futura. `for`, `++` e `--` são pendências do projeto; as limitações de tipos devem ser documentadas, conforme a seção 11.

---

## 8. Plano de quitação (Sprints 12 a 15)

1. **Quitação documental (Sprints 11/12):** sincronização do status no GitHub Pages (Issues #60 e #62) e fechamento dos documentos técnicos de otimização e testes.
2. **Estabilização de memória (Sprint 12):** testes sistemáticos com Valgrind e ajustes de gerenciamento de escopos locais.
3. **Fechamento para entrega (Sprints 13 a 15):** integrar correções, validar memória e erros, consolidar documentação e repetir a regressão. Recursos novos ficam subordinados aos critérios da seção 11; não há nova data de congelamento aprovada nesta revisão.

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
 
Somente F1 possui registro nesta página. Outros feedbacks devem ser acrescentados pela responsável pelo registro do P2 quando houver evidência; não foram presumidos nesta revisão.

### 11.2 Backlog priorizado e critérios de conclusão

Ordem de trabalho proposta para o fechamento: **validar as correções existentes → implementar as pendências do projeto → validar memória e regressão → fechar documentação**. Os responsáveis abaixo seguem as atribuições do planejamento; são responsáveis pelas frentes, não uma nova atribuição de autoria dos commits.

| Prioridade | Item e responsável | Situação / dependência | Critério de conclusão |
|---|---|---|---|
| Correção implementada | Escopos e redeclaração — Rafael | Corrigidos em `ab5ee01`, na branch `fix/sprint12-escopos`, com 34 verificações aprovadas | Preservar as regressões. A revisão do status de integração será feita posteriormente pelo responsável. |
| P0 | Rejeição de erros léxicos — frente de correções de Nicole | Comentário não terminado ainda permite prosseguir na versão descrita na seção 5 | O teste `11_comentario_aberto.c` retorna falha e não alcança o runtime. |
| P0 | Memória — Bernardo (Valgrind), Rafael (estado/escopos), Danielly (destruição local) | Issue #61; depende da integração das correções de runtime | Relatório reproduzível de Valgrind cobrindo blocos, laços e textos, sem acessos inválidos nem vazamentos atribuíveis ao interpretador. |
| P0 | Suíte e regressão — Paulo; regressão final — Nicole | Saídas esperadas antigas e teste de aviso misturado a erros | Manter `03`/`12` válidos, classificar `18` como aviso e conferir código de saída, diagnóstico e resultado; não aceitar apenas mensagem de erro como prova de rejeição. |
| P1 | Build limpo — Rafael | `c755e1d` disponível em `fix/sprint15-compilacao`; validado nesta branch | Integrar e repetir build sem artefatos anteriores, inclusive `make -j2 rebuild`, com GNU Make 4.3+. |
| P1 | Documentação técnica — Paulo/Bernardo (#43), Bernardo (#59), Rafael (escopos) | Documentação de escopos em `1ecc018`, branch `docs/sprint14-escopos-destruicao`; conferir demais entregas antes de baixar pendências | Documentos coerentes com a versão integrada, exemplos verificáveis e limitações explícitas. |
| P1 | Diagnósticos de runtime — Nicole; guia — Paulo | Validar implementação na versão final; não assumir que branches separadas já foram incorporadas | Divisão por zero e demais falhas cobertas produzem diagnóstico e resultado de execução definido, com testes. |
| P1 | Publicação — Danielly/Rafael (Sprint 13), Paulo/Bernardo (final) | Depende do conteúdo integrado | Build do MkDocs aprovado e publicação da revisão confirmada no Pages. |
| P1 | `for`, `++` e `--` — Paulo | Pendência do projeto; implementação confirmada pela equipe | Implementar parser, semântica e runtime; testar inicialização, condição, atualização, limites e interação com escopos. |
| P1 | Documentar limitações de `float`/`char` | Decisão: documentar as limitações existentes, sem incluir ampliação de tipos nesta tarefa | Explicar representação, conversões ausentes e comportamento dos literais, conforme a seção 6. |
| P3 | Atribuição como expressão — Paulo | Melhoria posterior, sem bloquear a estabilização | Avaliar somente após o fechamento das prioridades anteriores. |
| Trabalho futuro | Funções, chamadas e `return` | Decisão da equipe: não implementar nesta entrega | Manter como evolução futura; não exigir esses recursos para concluir o projeto atual. |

P0 reúne falhas que comprometem a confiabilidade. P1 reúne as pendências da entrega, incluindo `for`, `++` e `--`. P3 reúne melhorias posteriores. Essa classificação organiza o backlog; não atesta conclusão de tarefas de outros integrantes.

### 11.3 Decisões da equipe

Decisões confirmadas por Rafael nesta revisão:

| ID | Decisão | Consequência |
|---|---|---|
| D1 | Não implementar funções nesta entrega; deixar funções, chamadas e `return` como implementação futura | Retirar esses recursos das pendências obrigatórias do projeto atual. |
| D2 | Reconhecer a correção de escopo já implementada na Sprint 12 | Não listar novamente a implementação como trabalho por fazer; manter os testes de regressão. |
| D3 | Implementar `for`, `++` e `--` | Registrar como pendência do projeto, não como melhoria opcional condicionada a nova aprovação. |
| D4 | Documentar as limitações de `float` e `char` | Descrever o comportamento real sem prometer suporte completo aos tipos de C. |
| D5 | Corrigir os testes `03_declaracoes` e `12_escopo_blocos` | Preservar os programas válidos e atualizar suas saídas esperadas para o pipeline com a correção da Sprint 12. |

### 11.4 Checklist desta revisão

- [x] Registrar funções como implementação futura por decisão da equipe.
- [x] Reconhecer a correção de escopo da Sprint 12.
- [x] Listar `for`, `++` e `--` como pendências do projeto.
- [x] Documentar as limitações atuais dos tipos.
- [x] Corrigir as saídas esperadas dos testes `03` e `12` e validá-las com a correção de escopo.

A revisão dos status das demais entregas, da integração e da publicação será feita posteriormente pelo responsável, conforme solicitado. Esta atualização não encerra issues, não publica o Pages e não altera o estado das tarefas dos demais integrantes.
