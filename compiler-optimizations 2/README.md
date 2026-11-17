# Compiler Optimizations: Common Subexpression Elimination

COMP 211 Checkoff 3. I explored how compilers transform C code to run faster
without changing its behavior. I focused on the GCC optimization levels
(-O0 to -O3) and on one technique, **Common Subexpression Elimination (CSE)**.

## What is CSE?
If the same expression (like `a * b`) is computed more than once and none of
its inputs changed in between, the compiler computes it once, keeps the result
in a register, and reuses it.

## Experiments (in `examples/`)
| File | What it shows |
|---|---|
| `cse_basic.c` | `a * b` is used twice. The number of `imul` instructions drops from 3 at -O0 to 2 at -O1. |
| `cse_aliasing.c` | CSE is **not** allowed when a pointer write might change the value. `restrict` lets the compiler reuse the result. |
| `sum_formula.c` | Adds 0..n-1 in a loop. clang -O2 replaces the loop with the formula n(n-1)/2, so 1 billion additions take ~0 seconds. |
| `strlen_loop.c` | Calls `strlen(s)` on every loop iteration. At -O0 this is O(n²); at -O1+ the compiler calls it once before the loop. |
| `array_sum.c` | Sums a large array. -O3 uses SIMD (vectorized) instructions that add several numbers at once. |
| `benchmark.c` | Times the same loop at -O0, -O1, -O2, -O3, and at -O2 with the CSE passes turned off. |

### How to run
Requires `gcc` and `make` (Linux, macOS, or WSL).
```
cd examples
make asm     # generates .s assembly files and counts instructions
make bench   # builds and times the benchmark at every level
make hook    # the three dramatic speedup examples
make clean
```
You can also paste the `.c` files into [Compiler Explorer](https://godbolt.org)
to see the assembly side by side.

## Results

learncli$ make hook
sum_formula  gcc   -O0  sum = 499999999500000000   time = 2.2245 s
sum_formula  gcc   -O2  sum = 499999999500000000   time = 0.3202 s
sum_formula  gcc   -O3  sum = 499999999500000000   time = 0.3229 s
sum_formula  clang -O0  sum = 499999999500000000   time = 1.0107 s
sum_formula  clang -O2  sum = 499999999500000000   time = 0.0000 s
sum_formula  clang -O3  sum = 499999999500000000   time = 0.0000 s
strlen_loop  gcc   -O0  count = 66667   time = 0.4968 s
strlen_loop  gcc   -O2  count = 66667   time = 0.0001 s
strlen_loop  gcc   -O3  count = 66667   time = 0.0000 s
strlen_loop  clang -O0  count = 66667   time = 0.5025 s
strlen_loop  clang -O2  count = 66667   time = 0.0000 s
strlen_loop  clang -O3  count = 66667   time = 0.0000 s
array_sum    gcc   -O0  total = 1499999700   time = 1.097 s
array_sum    gcc   -O2  total = 1499999700   time = 0.166 s
array_sum    gcc   -O3  total = 1499999700   time = 0.158 s
array_sum    clang -O0  total = 1499999700   time = 0.481 s
array_sum    clang -O2  total = 1499999700   time = 0.060 s
array_sum    clang -O3  total = 1499999700   time = 0.058 s


## Key findings
1. At -O0, GCC does exactly what the code says, including repeated work.
2. CSE is already on at -O1: one `imul` disappears.
3. Aliasing blocks CSE. If two pointers might point to the same memory, the compiler has to reload and recompute.
4. Turning off GCC's CSE passes at -O2 made the benchmark noticeably slower (about 0.07 s to 0.10 s on my test machine), but it was still far faster than -O0. CSE is one of many optimizations that the levels combine.

## Resources
- GCC optimization options: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
- Compiler Explorer: https://godbolt.org
- Wikipedia, Common subexpression elimination: https://en.wikipedia.org/wiki/Common_subexpression_elimination
- My AI conversation: https://claude.ai/share/48e6ce24-5ede-4e4c-ab79-ecafbcd6d11d
