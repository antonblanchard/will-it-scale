#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define FILESIZE (1 * 1024 * 1024)
#define BUFLEN (FILESIZE / 128)

char *testcase_description = "Separate file fallocate";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);
	unsigned long place = 0;
	assert(fd >= 0);
	unlink(tmpfile);

	while (1) {
		int ret = fallocate(fd, 0, place, BUFLEN);
		if (errno != EINTR)
			assert(ret == 0);
		place += BUFLEN;
		if (place >= FILESIZE) {
			ret = ftruncate(fd, 0);
			assert(ret == 0);
			place = 0;
		}
		(*iterations)++;
	}
}

