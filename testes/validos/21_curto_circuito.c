int a = 0 && (1 / 0);
int b = 1 || (1 / 0);
int c = 1 && 1;
int d = 0 || 0;
printf("%d %d %d %d\n", a, b, c, d);