int i = 0;
int soma = 0;
while (i < 5) {
    i = i + 1;
    if (i % 2 == 0) {
        printf("par: %d\n", i);
    } else {
        printf("impar: %d\n", i);
    }
    soma = soma + i;
}
printf("soma: %d\n", soma);