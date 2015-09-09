#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define READ 0
#define WRITE 1

char *testcase_description = "pipe read/write";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int fd[2];

	assert(pipe(fd) == 0);

	while (1) {
		char c = 0;
		int ret;

		do {
			ret = write(fd[WRITE], &c, 1);
		} while (ret != 1 && errno == EINTR);

		do {
			ret = read(fd[READ], &c, 1);
		} while (ret != 1 && errno == EINTR);

		(*iterations)++;
	}
}
