#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define READ 0
#define WRITE 1

char *testcase_description = "unix domain socket read/write";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int fd[2];
	char c;
	int ret;

	socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

	while (1) {
		do {
			ret = write(fd[WRITE], &c, 1);
		} while (ret != 1 && errno == EINTR);

		do {
			ret = read(fd[READ], &c, 1);
		} while (ret != 1 && errno == EINTR);

		(*iterations)++;
	}
}
