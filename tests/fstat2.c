#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file fstat";

void testcase_prepare(unsigned long nr_tasks)
{
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	close(fd);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	struct stat sb;
	int fd = open(tmpfile, O_RDONLY);

	assert(fd>= 0);

	while (1) {
		int error = fstat(fd, &sb);
		assert(error == 0);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
