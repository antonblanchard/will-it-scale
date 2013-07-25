#define _XOPEN_SOURCE 500
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file pread";

void testcase_prepare(void)
{
	char buf[BUFLEN];
	int fd = mkstemp(tmpfile);

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	close(fd);
}

void testcase(unsigned long long *iterations)
{
	char buf[BUFLEN];
	int fd = open(tmpfile, O_RDONLY);

	assert(fd >= 0);

	while (1) {
		assert(pread(fd, buf, BUFLEN, 0) == BUFLEN);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
