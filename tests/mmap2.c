#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <assert.h>

#define MEMSIZE (128 * 1024 * 1024)

char *testcase_description = "Separate file mmap/munmap of 128MB";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(ftruncate(fd, MEMSIZE) == 0);
	unlink(tmpfile);

	while (1) {
		char *c = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE,
			       MAP_PRIVATE, fd, 0);
		assert(c != MAP_FAILED);
		munmap(c, MEMSIZE);

		(*iterations)++;
	}
}
