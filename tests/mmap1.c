#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#define MEMSIZE (128 * 1024 * 1024)

char *testcase_description = "Anonymous memory mmap/munmap of 128MB";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	while (1) {
		char *c = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE,
			       MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		assert(c != MAP_FAILED);
		munmap(c, MEMSIZE);

		(*iterations)++;
	}
}
