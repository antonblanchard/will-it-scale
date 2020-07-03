static inline unsigned int spin_trylock(unsigned int *lock)
{
	unsigned int tmp, token;

	token = 1;
	asm volatile(
		"1:     lwarx   %0,0,%2,1\n\t"
		"cmpwi   0,%0,0\n\t"
		"bne-    2f\n\t"
		"stwcx.  %1,0,%2\n\t"
		"bne-    1b\n\t"
		"lwsync\n"
		"2:"     : "=&r" (tmp)
		: "r" (token), "r" (lock)
			: "cr0", "memory");

	return tmp;
}

static void inline spin_lock(unsigned int *lock)
{
	while (spin_trylock(lock))
		;
}

static void inline spin_unlock(unsigned int *lock)
{
	asm volatile("lwsync":::"memory");
	*lock = 0;
}

char *testcase_description = "Uncontended lwarx/stwcx. spinlock with lwsync barriers";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	unsigned int lock = 0;
	// alloca() because plain "unsigned long val" gets optimized away.
	unsigned long *val = alloca(sizeof(unsigned long));
	*val = 0;

	while (1) {
		spin_lock(&lock);
		(*val)++;
		spin_unlock(&lock);

		(*iterations)++;
	}
}
