#define _GNU_SOURCE
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define BUFLEN getpagesize()
#define FILESIZE (1 * 1024 * 1024)

static char tmpfile[] = "/tmp/willitscale.XXXXXX";

char *testcase_description = "Same file O_DIRECT read";

void testcase_prepare(void)
{
	int fd = mkstemp(tmpfile);
	char buf[FILESIZE];

	memset(buf, 0, sizeof(buf));
	assert(fd >= 0);
	assert(write(fd, buf, sizeof(buf)) == sizeof(buf));
	close(fd);
}

void testcase(unsigned long long *iterations)
{
	int fd = open(tmpfile, O_DIRECT|O_RDONLY);
	char *p;

	p = malloc(BUFLEN + getpagesize());
	p = (char *)(((unsigned long)p + getpagesize()-1) & ~(getpagesize()-1));

	while (1) {
		int ret = read(fd, p, BUFLEN);
		assert(ret >= 0);
		if (ret == 0)
			lseek(fd, 0, SEEK_SET);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	unlink(tmpfile);
}
