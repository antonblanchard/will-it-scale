#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <assert.h>

char *testcase_description = "eventfd read/write of 8 bytes";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	unsigned long long rbuf;
	unsigned long long wbuf;
	int fd = eventfd(0, 0);
	int ret;

	assert(fd >= 0);

	wbuf = 1;

	while (1) {
		do {
			ret = write(fd, &wbuf, sizeof(wbuf));
		} while (ret != sizeof(wbuf) && errno == EINTR);
		assert(ret == sizeof(wbuf));

		do {
			ret = read(fd, &rbuf, sizeof(rbuf));
		} while (ret != sizeof(rbuf) && errno == EINTR);
		assert(ret == sizeof(wbuf));

		(*iterations)++;
	}
}
