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
#define MEMORYSIZE (1 * 1024 * 1024)

char *testcase_description = "TLB flush of anonymous memory private mapping";

void testcase(unsigned long *iteration, unsigned long nr)
{
	printf("thread/process number: %lu, pid: %ld\n", nr, gettid());
	unsigned long pgsize = getpagesize();
	unsigned long i;

	while (1) {
		char *addr = mmap(NULL, MEMORYSIZE, PROT_READ|PROT_WRITE,
						MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		for (i = 0; i < MEMORYSIZE; i += pgsize) {
			*(addr + i) = 1;
			madvise(addr + i, pgsize, MADV_DONTNEED);
			assert(*(addr + i) == 0);
			(*iteration)++;
		}
		munmap(addr, MEMORYSIZE);
	}
}
