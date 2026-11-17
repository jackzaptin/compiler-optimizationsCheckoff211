// Hook 1: The compiler replaces a loop with a math formula.
// Adds up 0 + 1 + 2 + ... + (n-1). At -O0 this runs n additions.
// clang at -O2 recognizes the pattern and uses n*(n-1)/2 instead: no loop at all.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long sum_to(long n) {
    long total = 0;
    for (long i = 0; i < n; i++)
        total += i;
    return total;
}

int main(int argc, char **argv) {
    long n = argc > 1 ? atol(argv[1]) : 1000000000;   // 1 billion
    clock_t start = clock();
    long result = sum_to(n);
    double secs = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("sum = %ld   time = %.4f s\n", result, secs);
    return 0;
}
