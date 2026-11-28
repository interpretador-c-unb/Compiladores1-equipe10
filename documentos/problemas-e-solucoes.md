Problemas Encontrados e Soluções Adotadas

Registro contínuo dos desafios técnicos enfrentados durante a implementação do interpretador C, suas causas, soluções e formas de validação.

1. Gestão de escopos durante a execução de laços

Problema

Estruturas de repetição podem executar o mesmo bloco várias vezes e também podem conter outros blocos ou laços aninhados. Sem uma entrada e uma saída de escopo equilibradas, variáveis locais poderiam permanecer no ambiente depois do bloco, ser reutilizadas indevidamente em outra iteração ou sobrescrever variáveis de um escopo externo.

O problema aparece especialmente em construções como:

Plain Text


int i = 0;
int j = 0;

while (i < 3) {
    j = 0;
    while (j < 2) {
        j = j + 1;
    }
    i = i + 1;
}



Nesse caso, o laço externo executa um bloco que contém outro laço. O ambiente precisa conservar o escopo externo enquanto o escopo interno estiver ativo e retornar ao nível anterior depois que o bloco terminar.

Solução adotada

O runtime utiliza Environment, definido em src/interpreter.h e implementado em src/interpreter.c. O ambiente mantém:

•
uma lista ligada de variáveis em head;

•
o nível corrente em current_scope;

•
o nível de escopo associado a cada RuntimeVar.

A entrada e a saída de blocos são controladas por:

Plain Text


env_enter_scope(env);
env_exit_scope(env);



Ao avaliar um NODE_BLOCK, o interpretador:

1.
incrementa o nível de escopo;

2.
executa sequencialmente os nós do bloco;

3.
remove as variáveis pertencentes ao nível encerrado;

4.
decrementa o nível de escopo.

O NODE_WHILE reavalia sua condição a cada iteração e executa o corpo por meio de evaluate_ast(). Quando o corpo é um bloco, o NODE_BLOCK faz o pareamento entre env_enter_scope() e env_exit_scope() em cada execução. Dessa forma, a repetição do laço não acumula as variáveis locais das iterações anteriores.

A estrutura representa uma pilha lógica de escopos lexicais. Ela não é uma pilha de chamadas de funções, pois o subconjunto atual do projeto ainda não possui funções ou chamadas de função.

Exemplo de comportamento esperado

Plain Text


int total = 0;
int i = 0;

while (i < 2) {
    int parcial = i + 1;
    total = total + parcial;
    i = i + 1;
}



Durante a execução do corpo, parcial pertence ao escopo interno. Ao final de cada bloco, esse símbolo é removido do ambiente. total e i, declarados no escopo externo, continuam disponíveis para a próxima iteração.

O teste de laços aninhados em testes/validos/14_while_execucao.c valida esse comportamento e termina com:

Plain Text


j (int) = 2
i (int) = 3
cont (int) = 6
n (int) = 0



Limitação conhecida

O ambiente atual localiza variáveis pela lista ligada e pelo nome. A política de runtime deve continuar sendo observada em casos de sombreamento de nomes entre escopos. A destruição de variáveis locais ocorre ao sair do bloco; variáveis do escopo externo permanecem disponíveis enquanto o laço interno estiver ativo.

2. Suíte oficial com mais de dez programas em C

Problema

A execução de if, while e printf() precisa ser validada em mais de um cenário. Um único programa não é suficiente para detectar regressões em ramos condicionais, laços simples, laços aninhados, expressões aritméticas e saída formatada.

Solução adotada

A suíte de entradas válidas está em:

Plain Text


testes/validos/



Atualmente existem 17 programas válidos numerados, além de entradas auxiliares sem par .exp. Os arquivos .exp registram a saída esperada para os casos que participam da comparação automatizada.

Os principais grupos cobertos são:

Grupo
Casos
Cobertura
Declarações e expressões
01 a 03
Declarações, atribuições e expressões aritméticas.
Condicionais
04, 05, 09, 15
if, else, aninhamento e seleção de ramos.
Laços
01, 03, 06, 13, 14
while, corpo sem chaves, laços aninhados e atualização de estado.
Tipos e operadores
07, 08, 11
Precedência, inteiros, floats, resto e operadores relacionais/lógicos.
Literais e comentários
10
Literais e comentários aceitos pelo lexer.
printf()
16
%d, %f, %s, %c, %% e chamadas com diferentes quantidades de argumentos.
Integração
17
while com if, else, aritmética e printf() no mesmo programa.




Os quatro casos mais diretamente relacionados ao runtime da Sprint 11 são:

•
testes/validos/14_while_execucao.c: laços simples e aninhados;

•
testes/validos/15_if_ramos.c: ramo then, else e if aninhado;

•
testes/validos/16_printf.c: saída formatada com diferentes tipos;

•
testes/validos/17_if_while_printf.c: integração de laço, condição e saída.

Como executar

Na raiz do repositório, primeiro compile:

Bash


make -C src rebuild



Depois execute um caso individual:

Bash


./interpreter testes/validos/17_if_while_printf.c



Para comparar automaticamente os casos que possuem arquivo esperado, use o script geral:

Bash


bash testes/executar_testes.sh



Quando o script solicitar a pasta, informe:

Plain Text


testes/validos



A comparação considera a saída padrão do interpretador e verifica se não houve erro em stderr nem código de retorno diferente de zero.

Também é possível validar diretamente os quatro casos principais:

Bash


for caso in 14_while_execucao 15_if_ramos 16_printf 17_if_while_printf; do
    ./interpreter "testes/validos/${caso}.c" > "/tmp/${caso}.out"
    diff -u "testes/validos/${caso}.exp" "/tmp/${caso}.out"
done



Se o diff não produzir saída, a saída observada corresponde ao arquivo esperado.

Resultado esperado

A suíte deve cobrir tanto casos em que o corpo do while não executa quanto casos com várias iterações e aninhamento. O caso 14_while_execucao.c, por exemplo, verifica que o laço interno é executado seis vezes no total e que o ambiente termina com os valores esperados de i, j, cont e n.

O caso 17_if_while_printf.c verifica a interação entre fluxo de controle e saída formatada, incluindo a seleção entre mensagens de número par e ímpar e a soma final.

A suíte documentada nesta seção atende ao requisito de possuir mais de dez programas válidos em C. A criação de novos casos não faz parte desta correção porque os 17 casos já estão versionados no repositório.
