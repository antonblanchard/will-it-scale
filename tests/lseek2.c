#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file lseek";

void testcase_prepare(unsigned long nr_tasks, unsigned long nr)
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
	int fd = open(tmpfile, O_RDWR);

	assert(fd >= 0);

	while (1) {
		lseek(fd, 0, SEEK_SET);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
