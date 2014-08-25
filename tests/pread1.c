#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUFLEN 4096

char *testcase_description = "Separate file pread";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[BUFLEN];
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	unlink(tmpfile);

	while (1) {
		assert(pread(fd, buf, BUFLEN, 0) == BUFLEN);

		(*iterations)++;
	}
}
