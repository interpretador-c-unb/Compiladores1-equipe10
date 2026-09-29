# Guia de Uso

Este guia detalha como utilizar o driver principal do interpretador C gerado após a compilação do projeto.

## Execução

O driver principal (`main.c`) funciona através da linha de comandos. Após construir o projeto utilizando o `Makefile` (com o comando `make`), será gerado um executável (ex: `./interpretador`).

A sintaxe de execução requer que o caminho de um ficheiro de código-fonte em C (`arquivo.c`) seja passado como argumento:

```bash
./interpretador caminho/para/o/arquivo.c