int x = 2;
int y = 5;
if (x < y) {
    if (x > 0){
        x = x + y;
    } else {
        x = x * (-1);
    }
} else {
    y = y + x;
}