#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file open/close";

void testcase_prepare(void)
{
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	close(fd);
}

void testcase(unsigned long long *iterations)
{
	while (1) {
		int fd = open(tmpfile, O_RDWR);
		assert(fd >= 0);
		close(fd);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
