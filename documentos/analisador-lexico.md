# Analisador Léxico

## Tratamento de Comentários em Bloco

Para o reconhecimento e descarte de comentários multilinha (`/* ... */`), utiliza-se o recurso de **condições de início exclusivas** do Flex (`%x`).

### Por que utilizar Estado Exclusivo (`%x`)?
Ao declarar `%x COMMENT`, o Flex garante que, enquanto o analisador estiver no estado `COMMENT`, **apenas** as regras prefixadas com `<COMMENT>` serão avaliadas. Regras globais (como identificadores, palavras reservadas e numéricos) ficam temporariamente desativadas, prevenindo falsos reconhecimentos dentro do bloco de comentário.

### Funcionamento e Regras
1. **Entrada no Estado:** Ao encontrar a sequência inicial `/*`, o analisador executa `BEGIN(COMMENT)`.
2. **Saída do Estado:** Ao encontrar `*/`, o analisador executa `BEGIN(INITIAL)`, retornando ao fluxo normal.
3. **Contagem de Linhas:** Quebras de linha (`\n`) ocorridas dentro do comentário incrementam o contador de linhas do analisador (`yylineno`), garantindo o rastreamento correto de mensagens de erro posteriores.
4. **Descarte de Caracteres:** Quaisquer outros caracteres dentro do comentário são ignorados.

### Tratamento de Exceções (Erros Léxicos)
* **Comentário Não Finalizado (`<<EOF>>`):** Caso o arquivo chegue ao fim (`<<EOF>>`) antes que o comentário seja fechado com `*/`, uma exceção/erro léxico é disparada informando a linha onde o arquivo encerrou prematuramente.

### Especificação no Flex (`interpretador_c.l`)

```flex
%x COMMENT

%%

"/*"              { BEGIN(COMMENT); }
<COMMENT>"*/"     { BEGIN(INITIAL); }
<COMMENT>\n       { yylineno++; }
<COMMENT>.        { /* Ignora conteúdo do comentário */ }
<COMMENT><<EOF>>  { 
                    printf("Erro Léxico: Comentário multilinha não finalizado (linha %d)\n", yylineno);
                    BEGIN(INITIAL);
                  }