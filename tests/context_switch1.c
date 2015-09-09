#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define READ 0
#define WRITE 1

char *testcase_description = "Context switch via pipes";

extern void new_task(void *(func)(void *), void *arg);

struct args {
	int fd1[2];
	int fd2[2];
};

static void *child(void *arg)
{
	struct args *a = arg;
	char c;
	int ret;

	while (1) {
		do {
			ret = read(a->fd1[READ], &c, 1);
		} while (ret != 1 && errno == EINTR);
		assert(ret == 1);

		do {
			ret = write(a->fd2[WRITE], &c, 1);
		} while (ret != 1 && errno == EINTR);
		assert(ret == 1);
	}

	return NULL;
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	struct args a;
	char c;
	int ret;

	assert(pipe(a.fd1) == 0);
	assert(pipe(a.fd2) == 0);

	new_task(child, &a);

	while (1) {
		do {
			ret = write(a.fd1[WRITE], &c, 1);
		} while (ret != 1 && errno == EINTR);
		assert(ret == 1);

		do {
			ret = read(a.fd2[READ], &c, 1);
		} while (ret != 1 && errno == EINTR);
		assert(ret == 1);
			
		(*iterations) += 2;
	}
}
