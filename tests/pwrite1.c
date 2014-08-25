#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096

char *testcase_description = "Separate file pwrite";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[BUFLEN];
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	unlink(tmpfile);

	while (1) {
		assert(pwrite(fd, buf, BUFLEN, 0) == BUFLEN);

		(*iterations)++;
	}
}
