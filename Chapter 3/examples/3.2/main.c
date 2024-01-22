#include <stdio.h>

void multistore(long, long, long *);

int main() {
    long d;
    multistore(2, 3, &d);
    printf("2 * 3 --> %ld\n", d);
    return 0;
}

long mult2(long a, long b) {
    long s = a * b;
    return s;
}