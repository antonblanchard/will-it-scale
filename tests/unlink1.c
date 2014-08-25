#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

char *testcase_description = "Separate file open/close/unlink";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	close(fd);
	unlink(tmpfile);

	while (1) {
		fd = open(tmpfile, O_RDWR|O_CREAT, 0600);
		assert(fd >= 0);
		close(fd);
		unlink(tmpfile);

		(*iterations)++;
	}

	unlink(tmpfile);
}
