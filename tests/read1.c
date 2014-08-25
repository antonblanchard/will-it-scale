#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096
#define FILESIZE (1 * 1024 * 1024)

char *testcase_description = "Separate file read";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[FILESIZE];
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	unlink(tmpfile);

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));

	while (1) {
		int len = read(fd, buf, BUFLEN);
		assert(len >= 0);
		if (len == 0)
			lseek(fd, 0, SEEK_SET);

		(*iterations)++;
	}
}
