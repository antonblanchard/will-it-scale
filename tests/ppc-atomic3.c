#include <stdint.h>

static inline void atomic_inc(uint64_t *atomic)
{
	uint64_t tmp;

	asm volatile(
		"hwsync\n\t"
		"1:     lwarx   %0,0,%1\n\t"
		"addi	%0,%0,1\n\t"
		"stwcx.  %0,0,%1\n\t"
		"bne-    1b\n\t"
		"hwsync\n\t"
		"2:" : "=&r" (tmp)
		     : "r" (atomic)
		     : "cr0", "memory");
}

char *testcase_description = "Contended atomic with hwsync barriers";

uint64_t atomic = 0;

void testcase(unsigned long long *iterations, unsigned long nr)
{
        while (1) {
		atomic_inc(&atomic);
                (*iterations)++;
        }
}
