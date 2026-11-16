int x = 1;
int r = 0;
int z = 0;
if (x > 3) { r = 1; } else { r = 2; }
if (x > 3) { r = r + 10; }
if (x < 5) {
    if (x > 5) { r = r + 50; } else { r = r + 100; }
} else {
    r = r + 7;
}
if (z) { r = r + 1; }
x = x + r;