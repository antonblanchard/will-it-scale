#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <stdio.h>

#define FILESIZE (128 * 1024 * 1024)

char *testcase_description = "TLB flush of separate file private mapping";

void testcase(unsigned long *iteration, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);
	unsigned long offset = 0;
	unsigned long pgsize = getpagesize();
	assert(fd >= 0);
	assert(ftruncate(fd, FILESIZE) == 0);
	unlink(tmpfile);
	unsigned long i;

	while (1) {
		char *addr = mmap(NULL, FILESIZE, PROT_READ|PROT_WRITE,
						MAP_PRIVATE, fd, offset);
		for (i = 0; i < FILESIZE; i += pgsize) {
			*(addr + i) = 1;
			madvise(addr + i, pgsize, MADV_DONTNEED);
			assert(*(addr + i) == 0);
			(*iteration)++;
		}
		munmap(addr, FILESIZE);
	}
}
