# Análise Semântica — Sprint 8

**Estado verificado em 30/10/2026:** o projeto possui verificações iniciais de declaração e escopo durante o parsing. A análise semântica por travessia da AST, prevista na Sprint 8, ainda está pendente. Esta página descreve o comportamento existente, sem tratar funcionalidades planejadas como implementadas.

## O que já está implementado

| Verificação | Implementação atual | Limitação |
| --- | --- | --- |
| Uso de variável não declarada | `lookup_symbol()` nas regras de referência a identificador e de atribuição em `src/interpretador_c.y`. | Emite aviso, mas não interrompe o parser. |
| Redeclaração no mesmo escopo | `insert_symbol()` em `src/symbol_table.c` verifica nome e nível de escopo. | Recusa a inserção duplicada, mas o parser não usa esse retorno para sinalizar falha. |
| Entrada e saída de blocos | O parser chama `enter_scope()` ao abrir `{` e `exit_scope()` ao fechar `}`. | É controle da tabela durante a análise, não um ambiente de execução. |
| Visibilidade e sombreamento | A busca encontra o símbolo visível mais recente; nomes iguais são permitidos em escopos diferentes. | Não substitui a futura validação semântica da AST. |
| Tipo declarado | A tabela guarda `data_type` como texto, por exemplo `int` ou `float`. | Esse registro ainda não é usado para validar operandos ou atribuições. |

Esses recursos são a base já existente no front-end. Não representam a conclusão do módulo `semantic.c`, que ainda não existe no código verificado.

## Guia das mensagens existentes

### Variável usada sem declaração

Formato emitido em `stderr`:

```text
[AVISO SEMÂNTICO] Variável 'nome' usada sem declaração na linha N
```

O aviso aparece quando o identificador não é encontrado na tabela, tanto no lado esquerdo de uma atribuição quanto em uma expressão. Uma variável de bloco que já saiu de escopo também produz esse aviso.

Exemplo:

```c
int x = 2;
x = x + z;
```

`z` não foi declarada. Para corrigir a entrada, declare-a em um escopo visível antes de usá-la, ou corrija o nome caso tenha sido digitado errado.

O número da linha vem de `yylineno` no momento da ação do parser. Não há coluna na mensagem, nem posição de origem armazenada no nó da AST; portanto, isso ainda não atende ao requisito completo de localização dos diagnósticos da Sprint 8.

### Redeclaração no mesmo escopo

Formato emitido em `stderr`:

```text
[ERRO SEMÂNTICO] Variável 'nome' já declarada no escopo N
```

Exemplo:

```c
int x = 9;
int x = 8;
```

As duas declarações estão no escopo global (`0`). Remova a declaração duplicada, use outro nome ou, se a intenção for atualizar a variável, escreva uma atribuição (`x = 8;`). A mensagem atual informa o nível do escopo, mas não linha ou coluna.

### Uso após sair do escopo

```c
int x = 1;
{
    int y = 2;
}
x = y;
```

Ao fechar o bloco, `exit_scope()` remove `y` da tabela. O uso seguinte gera o mesmo aviso de variável sem declaração; não existe uma mensagem específica para “fora de escopo”.

### Resultado da análise

As mensagens acima não alteram o resultado de `yyparse()`. Uma entrada sintaticamente válida pode emitir um diagnóstico semântico e, mesmo assim, imprimir `[SUCESSO]` e terminar com código `0`. O driver atual retorna apenas o resultado da análise sintática.

Ainda estão pendentes a propagação de falha semântica para o driver, os diagnósticos com linha e coluna e a padronização de códigos de erro. Não há códigos numéricos ou catálogo de erros de tipagem implementado para documentar como comportamento atual.

## Compatibilidade de tipos: estado atual

A tabela abaixo registra o estado da implementação; não define uma nova política de tipos para a linguagem.

| Operação ou regra | Estado no código | Pendência |
| --- | --- | --- |
| Atribuição entre valores do mesmo tipo | Não há comparação de tipos. | Implementar validação e documentar a regra aprovada. |
| Conversão implícita `int` → `float` | Prevista no planejamento; não implementada. | Implementar coerção e seus testes. |
| Atribuição `float` → `int` | Não há validação ou conversão específica. | Definir se será permitida e qual será seu comportamento. |
| Operações aritméticas e operandos mistos | O parser constrói nós sem verificar tipos dos operandos. | Definir e implementar tipos permitidos e tipo resultante. |
| Comparações e operadores lógicos | A sintaxe é reconhecida, sem validação de compatibilidade. | Definir e implementar regras dos operandos e resultados. |
| Condições de `if` e `while` | Qualquer expressão aceita pela gramática pode ocupar a condição. | Validar os tipos permitidos nas condições. |
| Uso de `char`, strings e declarações `void` | Há reconhecimento sintático, sem checagem semântica completa. | Definir e validar restrições desses usos. |

Por exemplo, `int x = "texto";` atualmente constrói uma AST sem diagnóstico de incompatibilidade. Isso evidencia a ausência da checagem; não significa que essa atribuição tenha sido aprovada como regra da linguagem.

Na AST atual, os literais numéricos usam `double`, declarações não guardam o tipo declarado e os nós não possuem linha/coluna. A futura implementação deve definir como preservar ou recuperar as informações necessárias para tipagem e diagnósticos. Armazenar números em `double` não equivale a implementar coerção `int` → `float`.

## Pendências da Sprint 8

- [ ] Implementar `semantic.c`/`semantic.h` e a travessia da AST, integrando a fase ao driver.
- [ ] Validar declaração e escopo nessa fase, aproveitando as verificações existentes no parser.
- [ ] Definir a matriz completa de compatibilidade de tipos com a equipe e documentá-la após aprovação.
- [ ] Implementar checagem de tipos e coerção `int` → `float`.
- [ ] Validar condições de `if`/`while` e os operandos de cada operador.
- [ ] Emitir diagnósticos com linha e coluna e fazer falhas semânticas resultarem em falha do programa.
- [ ] Adicionar testes específicos de tipagem, coerção, condições e retorno de falha semântica.
- [ ] Revisar a documentação com Danielly e publicar esta revisão no GitHub Pages.

O planejamento divide a documentação entre Danielly (compatibilidade e regras) e Rafael (guia de mensagens e publicação compartilhada). A issue #37 agrega tipos e erros sob responsabilidade de Danielly, com Rafael como revisor; a #38 atribui a publicação a Rafael, com revisão de Danielly. Esta página reúne o diagnóstico atual, sem declarar concluída a matriz de tipos ou a revisão pelo par.

## Como conferir o comportamento existente

Execute na raiz do projeto:

```bash
make -C src
./interpreter testes/invalidos/08_var_nao_declarada.c
./interpreter testes/invalidos/09_redeclaracao.c
./interpreter testes/invalidos/15_variavel_fora_de_escopo.c
bash testes/executar_testes.sh
```

A suíte existente aceita um diagnóstico em `stderr` como evidência de erro nos casos inválidos, mesmo com código de saída `0`. Sua aprovação não comprova a existência da análise semântica completa.

Para validar a documentação, execute `mkdocs serve` e abra **Análise Semântica** no menu. Consulte também a [tabela de símbolos](tabela-simbolos.md), o [guia de uso](guia-uso.md) e o [planejamento da Sprint 8](planejamento-sprints.md#sprint-8-analise-semantica-integrada-a-ast).
