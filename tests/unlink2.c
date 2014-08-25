#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <assert.h>

char *testcase_description = "Separate file open/close in different directories";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpdir[] = "/tmp/willitscale.XXXXXX";
	char tmpfile[PATH_MAX];

	assert(mkdtemp(tmpdir) != NULL);
	sprintf(tmpfile, "%s/willitscale", tmpdir);

	while (1) {
		int fd = open(tmpfile, O_RDWR|O_CREAT, 0600);
		assert(fd >= 0);
		close(fd);
		unlink(tmpfile);

		(*iterations)++;
	}
}
