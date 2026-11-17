// Experiment A: Common Subexpression Elimination (CSE)
// (a * b) appears twice. At -O0 the compiler computes it twice;
// at -O1 and above it computes it once and reuses the result.
//
// Look at the assembly:   gcc -O0 -S cse_basic.c -o cse_basic_O0.s
//                         gcc -O1 -S cse_basic.c -o cse_basic_O1.s
// Then count the "imul" instructions in each file.
#include <stdio.h>

int f(int a, int b, int c) {
    int x = (a * b) + c;   // a*b computed here...
    int y = (a * b) - c;   // ...and the same a*b again
    return x * y;
}

int main(int argc, char **argv) {
    // argc is used so the compiler can't compute the answer at compile time
    printf("%d\n", f(argc + 2, argc + 3, argc));
    return 0;
}
