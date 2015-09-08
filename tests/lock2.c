#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file write lock/unlock";

void testcase_prepare(unsigned long nr_tasks)
{
	assert(mkstemp(tmpfile) != -1);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int fd = open(tmpfile, O_RDWR);

	assert(fd >= 0);

	while (1) {
		struct flock lck;

		lck.l_type = F_WRLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = 0;
		lck.l_len = 1;
		assert(fcntl(fd, F_SETLKW, &lck) == 0);

		lck.l_type = F_UNLCK;
		lck.l_whence = SEEK_SET;
		lck.l_start = 0;
		lck.l_len = 1;
		assert(fcntl(fd, F_SETLKW, &lck) == 0);

		(*iterations) += 2;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
