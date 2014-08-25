#define _GNU_SOURCE
#include <pthread.h>

char *testcase_description = "Uncontended pthread mutex";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	while (1) {
		pthread_mutex_lock(&mutex);
		pthread_mutex_unlock(&mutex);

		(*iterations)++;
	}
}
