#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

char *testcase_description = "Separate file write lock/unlock";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	unlink(tmpfile);

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
