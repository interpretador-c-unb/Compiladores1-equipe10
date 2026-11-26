Relatório de Valgrind — Sprint 12

Objetivo

Executar os programas válidos do interpretador com Valgrind para verificar vazamentos, acessos inválidos e uso de valores não inicializados durante a execução da AST.

Procedimento

Na raiz do repositório, executar:

Bash


chmod +x testes/valgrind/run_valgrind.sh
./testes/valgrind/run_valgrind.sh



O script recompila o interpretador com informações de depuração e executa todos os arquivos testes/validos/*.c com:

Plain Text


--leak-check=full
--show-leak-kinds=all
--track-origins=yes
--errors-for-leak-kinds=definite,indirect



Os blocos classificados apenas como still reachable não fazem o comando falhar. O script falha para vazamentos definitely lost ou indirectly lost, além de erros do Memcheck.

Problema encontrado e correção

Na execução inicial, o caso testes/validos/16_printf.c apresentou 6 bytes definitely lost em duas alocações. A origem estava em valores temporários de strings produzidos pela avaliação da AST e duplicados pela tabela de variáveis, sem liberação após a declaração ou atribuição. O caso 11_operadores.c também revelou uma decisão condicional dependente de valor não inicializado nos operadores unários ! e -.

A correção foi feita em src/interpreter.c com a função free_value, liberando valores string temporários depois de serem copiados para o ambiente. A mesma rotina também é usada para liberar valores temporários de condições e operandos da execução, sem alterar a propriedade das strings armazenadas na AST ou no ambiente. Os operadores unários passaram a produzir explicitamente um Value inteiro ou real antes da liberação do operando.

Critério de aprovação

Considera-se a verificação aprovada quando run_valgrind.sh termina com código 0 e os relatórios não apresentam definitely lost, indirectly lost, acessos inválidos ou uso de valores não inicializados.