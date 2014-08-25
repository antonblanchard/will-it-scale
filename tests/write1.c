#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN 4096
#define FILESIZE (1 * 1024 * 1024)

char *testcase_description = "Separate file write";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char buf[BUFLEN];
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);
	unsigned long size = 0;

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	unlink(tmpfile);

	while (1) {
		int ret = write(fd, buf, BUFLEN);
		assert(ret >= 0);
		size += ret;
		if (size >= FILESIZE) {
			size = 0;
			lseek(fd, 0, SEEK_SET);
		}

		(*iterations)++;
	}
}
