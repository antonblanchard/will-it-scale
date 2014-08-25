#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

char *testcase_description = "Separate file open/close";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	close(fd);

	while (1) {
		int fd = open(tmpfile, O_RDWR);
		assert(fd >= 0);
		close(fd);

		(*iterations)++;
	}

	unlink(tmpfile);
}
