#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

char *testcase_description = "brk increase/decrease of one page";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	void *addr = sbrk(0);
	unsigned long page_size = getpagesize();

	while (1) {
		addr += page_size;
		assert(brk(addr) == 0);

		addr -= page_size;
		assert(brk(addr) == 0);

		(*iterations) += 2;
	}
}
