// Experiment B: When CSE is NOT allowed (pointer aliasing)
//
// In no_restrict(), p and q might point to the same int. So the write
// "*q = 0" might change *p, and the compiler must load *p again and
// recompute (*p * 2). It can't reuse the first result.
//
// In with_restrict(), the "restrict" keyword promises p and q never
// point to the same memory, so the compiler can reuse (*p * 2).
//
// Look at the assembly:   gcc -O2 -S cse_aliasing.c -o cse_aliasing_O2.s
// no_restrict loads from (%rdi) twice; with_restrict loads it once.
#include <stdio.h>

int no_restrict(int *p, int *q) {
    int x = *p * 2;
    *q = 0;            // might overwrite *p
    int y = *p * 2;    // must reload *p and recompute
    return x + y;
}

int with_restrict(int *restrict p, int *restrict q) {
    int x = *p * 2;
    *q = 0;            // promised not to touch *p
    int y = *p * 2;    // compiler can reuse x
    return x + y;
}

int main(void) {
    int a = 5, b = 7;
    printf("different pointers: %d %d\n", no_restrict(&a, &b), with_restrict(&a, &b));

    // Same pointer passed twice: no_restrict handles this correctly.
    // Doing this with with_restrict would break the restrict promise
    // (undefined behavior), which is why the compiler can't assume it by default.
    int c = 5;
    printf("same pointer:       %d\n", no_restrict(&c, &c));
    return 0;
}
