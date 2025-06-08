#include <stdio.h>
#include "defer.h"

void open(int x) {
    printf("opening %d\n", x);
}

void close(int x) {
    printf("closing %d\n", x);
}

int main() {
    open(1);
    defer { close(1); }

    if (true) {
        printf("uh oh!\n");
        return 1;
    }

    open(2);
    defer { close(2); }

    return 0;
}
