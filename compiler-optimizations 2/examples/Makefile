CC = gcc
LEVELS = O0 O1 O2 O3

all: asm bench

# Experiment A + B: generate assembly at each level
asm:
	@for L in $(LEVELS); do \
	  $(CC) -$$L -S cse_basic.c    -o cse_basic_$$L.s; \
	  $(CC) -$$L -S cse_aliasing.c -o cse_aliasing_$$L.s; \
	done
	@echo "imul count in f() for cse_basic at each level:"
	@for L in $(LEVELS); do \
	  printf "  -$$L: "; awk '/^f:/,/ret/' cse_basic_$$L.s | grep -c imul; \
	done
	@echo "loads of *p (%rdi) in no_restrict vs with_restrict at -O2:"
	@printf "  no_restrict:   "; awk '/^no_restrict:/,/ret/'   cse_aliasing_O2.s | grep -c '(%rdi)'
	@printf "  with_restrict: "; awk '/^with_restrict:/,/ret/' cse_aliasing_O2.s | grep -c '(%rdi)'

# Experiment C: time the benchmark at each level
bench:
	@for L in $(LEVELS); do \
	  $(CC) -$$L benchmark.c -o bench_$$L; \
	  printf -- "-$$L  "; ./bench_$$L; \
	done
	@echo "With GCC's CSE passes turned off at -O2:"
	@$(CC) -O2 -fno-gcse -fno-tree-fre -fno-tree-pre benchmark.c -o bench_O2_nocse
	@printf -- "-O2 no CSE  "; ./bench_O2_nocse

clean:
	rm -f *.s bench_* hook_bin

# Hook examples: big speedups from a single flag (uses gcc and clang if installed)
hook:
	@for f in sum_formula strlen_loop array_sum; do \
	  for cc in gcc clang; do command -v $$cc >/dev/null || continue; \
	    for L in O0 O2 O3; do \
	      $$cc -$$L $$f.c -o hook_bin; \
	      printf "%-12s %-5s -%s  " $$f $$cc $$L; ./hook_bin; \
	    done; done; done; rm -f hook_bin
