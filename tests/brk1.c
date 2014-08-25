#include <sys/types.h>
#include <unistd.h>

char *testcase_description = "brk increase/decrease of one page";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	void *addr = sbrk(0);
	unsigned long page_size = getpagesize();

	while (1) {
		addr += page_size;
		brk(addr);

		addr -= page_size;
		brk(addr);

		(*iterations) += 2;
	}
}
