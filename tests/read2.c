#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define BUFLEN 4096
#define FILESIZE (1 * 1024 * 1024)

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file read";

void testcase_prepare(unsigned long nr_tasks)
{
	char buf[FILESIZE];
	int fd = mkstemp(tmpfile);

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	close(fd);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[BUFLEN];
	int fd = open(tmpfile, O_RDONLY);

	assert(fd >= 0);

	while (1) {
		int ret = read(fd, buf, BUFLEN);
		assert(ret >= 0);
		if (ret == 0)
			lseek(fd, 0, SEEK_SET);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
