// Experiment C: How much faster do -O0, -O1, -O2, -O3 make the same code?
// The inner loop repeats expressions like (i * N + j) and (x * y), which
// gives CSE (and other optimizations) work to do.
//
// Build and run every level with:   make bench
#include <stdio.h>
#include <time.h>

#define N 1000
#define REPS 100

static double A[N][N];

int main(void) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = (i + j) % 10;

    clock_t start = clock();
    double total = 0.0;
    for (int r = 0; r < REPS; r++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double x = A[i][j];
                double y = r + 1.0;
                // (x * y) is a common subexpression used three times
                total += (x * y) + (x * y) * 0.5 - (x * y) * 0.25;
            }
        }
    }
    double secs = (double)(clock() - start) / CLOCKS_PER_SEC;

    // Printing total stops the compiler from deleting the loop as unused work.
    printf("total = %.1f   time = %.3f s\n", total, secs);
    return 0;
}
