#include <assert.h>
#include <pthread.h>

char *testcase_description = "Thread creation and teardown";

static void *worker(void *arg)
{
	return (NULL);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	pthread_t thread;
	int error;

	while (1) {
		error = pthread_create(&thread, NULL, worker, NULL);
		assert(error == 0);
		error = pthread_join(thread, NULL);
		assert(error == 0);
		(*iterations)++;
	}
}
