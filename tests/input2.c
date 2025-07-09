void main() {
    int a = 3, x = 2;
    float b = 1, c, d;

    if (x == 1) {
        a = a - b;
    } else if (x == 2) {
        a = (a - b) * 2;
    } else
        a = a - b;

    while (1) a = b;

    return 0;
}