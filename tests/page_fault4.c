#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#define MEMSIZE (128 * 1024 * 1024)

char *testcase_description = "Separate file private mapping page read fault";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);
	unsigned long pgsize = getpagesize();

	assert(fd >= 0);
	assert(ftruncate(fd, MEMSIZE) == 0);
	unlink(tmpfile);

	while (1) {
		unsigned long i;
		char cc __attribute__((unused));

		char *c = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE,
			       MAP_PRIVATE, fd, 0);
		assert(c != MAP_FAILED);

		for (i = 0; i < MEMSIZE; i += pgsize) {
			cc = *(volatile char *)(c + i);
			(*iterations)++;
		}

		munmap(c, MEMSIZE);
	}
}
