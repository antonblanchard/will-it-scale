#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#define MEMSIZE (128 * 1024 * 1024)

char *testcase_description = "Anonymous memory page fault";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	unsigned long pgsize = getpagesize();

	while (1) {
		unsigned long i;

		char *c = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE,
			       MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		assert(c != MAP_FAILED);

		for (i = 0; i < MEMSIZE; i += pgsize) {
			c[i] = 0;
			(*iterations)++;
		}

		munmap(c, MEMSIZE);
	}
}
