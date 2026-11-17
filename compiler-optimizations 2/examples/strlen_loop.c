// Hook 2: The same expression, strlen(s), is computed on EVERY loop iteration.
// At -O0 that makes the loop O(n^2): strlen walks the whole string each time.
// At -O2 GCC sees that the loop never changes s, so strlen(s) gives the same
// answer every time. It computes it once, like CSE across loop iterations.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int count_a(const char *s) {
    int count = 0;
    for (size_t i = 0; i < strlen(s); i++)   // strlen called every iteration!
        if (s[i] == 'a') count++;
    return count;
}

int main(void) {
    size_t n = 200000;
    char *s = malloc(n + 1);
    for (size_t i = 0; i < n; i++) s[i] = "abc"[i % 3];
    s[n] = '\0';

    clock_t start = clock();
    int c = count_a(s);
    double secs = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("count = %d   time = %.4f s\n", c, secs);
    free(s);
    return 0;
}
