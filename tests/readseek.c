#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096

char *testcase_description = "Separate file seek+read";

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
		lseek(fd, 0, SEEK_SET);
		assert(read(fd, buf, BUFLEN) == BUFLEN);

		(*iterations)++;
	}
}
