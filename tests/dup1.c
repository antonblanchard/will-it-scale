#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

char *testcase_description = "Separate file dup/close";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);

	while (1) {
		int fd2 = dup(fd);

		assert(fd2 >= 0);
		close(fd2);

		(*iterations)++;
	}

	close(fd);
	unlink(tmpfile);
}
