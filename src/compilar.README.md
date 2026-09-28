## Passos para compilar e executar

Na raiz do repositório:

```bash
make
./interpreter testes/exemplo.c
make clean
```

O `make` da raiz chama o `Makefile` de `src/`. Esse Makefile gera o analisador com o Bison e o Flex e produz o executável `interpreter`.

O programa de teste fica em `testes/exemplo.c`.
