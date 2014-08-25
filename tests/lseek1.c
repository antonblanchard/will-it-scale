#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define BUFLEN 4096

char *testcase_description = "Separate file lseek";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[BUFLEN];
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	unlink(tmpfile);

	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));

	while (1) {
		lseek(fd, 0, SEEK_SET);

		(*iterations)++;
	}
}
