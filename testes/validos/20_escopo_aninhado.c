int a = 1;
{
    int b = 2;
    {
        int c = a + b;
        printf("%d\n", c);
    }
    printf("%d\n", b);
}
printf("%d\n", a);