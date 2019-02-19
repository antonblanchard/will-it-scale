#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>

#define gettid()  syscall(SYS_gettid)
#define FILESIZE (128 * 1024 * 1024)

char *testcase_description = "TLB flush of separated file private mapping";

void testcase(unsigned long *iteration, unsigned long nr)
{
	printf("thread/process number: %lu, pid: %ld\n", nr, gettid());
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
