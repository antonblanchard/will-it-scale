#define _GNU_SOURCE
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN getpagesize()
#define FILESIZE (1 * 1024 * 1024)

char *testcase_description = "Separate file O_DIRECT read";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char *buf;
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkostemp(tmpfile, O_DIRECT);
	char *p;

	buf = aligned_alloc(getpagesize(), FILESIZE);
	memset(buf, 0, FILESIZE);
	assert(fd >= 0);
	assert(write(fd, buf, FILESIZE) == FILESIZE);
	unlink(tmpfile);

	p = malloc(BUFLEN + getpagesize());
	p = (char *)(((unsigned long)p + getpagesize()-1) & ~(getpagesize()-1));

	while (1) {
		int ret = read(fd, p, BUFLEN);
		assert(ret >= 0);
		if (ret == 0)
			lseek(fd, 0, SEEK_SET);

		(*iterations)++;
	}
	free(buf);
}
