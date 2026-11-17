// Hook 3: Doing several additions at once (vectorization).
// At -O3 the compiler uses special CPU instructions (SIMD) that add
// 4 or 8 numbers in a single instruction.
#include <stdio.h>
#include <time.h>

#define N 10000000
#define REPS 50
static int A[N];

int main(void) {
    for (int i = 0; i < N; i++) A[i] = i % 7;
    clock_t start = clock();
    long total = 0;
    for (int r = 0; r < REPS; r++)
        for (int i = 0; i < N; i++)
            total += A[i];
    double secs = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("total = %ld   time = %.3f s\n", total, secs);
    return 0;
}
