#define _XOPEN_SOURCE 500
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file pread to different offsets";

void testcase_prepare(unsigned long nr_tasks)
{
	char buf[BUFLEN];
	int fd = mkstemp(tmpfile);
	unsigned long left = getpagesize() * nr_tasks;

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);

	while (left > 0) {
		int n = write(fd, buf, sizeof(buf));
		assert(n > 0);
		left -= n;
	}
	close(fd);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[BUFLEN];
	int fd = open(tmpfile, O_RDONLY);
	unsigned long offset = getpagesize() * nr;

	assert(fd >= 0);

	while (1) {
		assert(pread(fd, buf, BUFLEN, offset) == BUFLEN);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
