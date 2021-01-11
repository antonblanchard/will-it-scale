#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

char *testcase_description = "brk unshared increase/decrease of one page";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	unsigned long page_size = getpagesize();
	void *addr = sbrk(page_size) + page_size;

	while (1) {
		addr += page_size;
		assert(brk(addr) == 0);

		addr -= page_size;
		assert(brk(addr) == 0);

		(*iterations) += 2;
	}
}
