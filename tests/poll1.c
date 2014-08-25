#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>

char *testcase_description = "poll of 1 fd";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);

	assert(fd >= 0);
	unlink(tmpfile);

	while (1) {
		struct pollfd pfd[1];

		memset(&pfd, 0, sizeof(pfd));
		pfd[0].fd = fd;
		pfd[0].events = POLLOUT;

		assert(poll(pfd, 1, 0) >= 0);

		(*iterations)++;
	}
}
