#define _GNU_SOURCE
#include <pthread.h>

char *testcase_description = "Contended pthread mutex with local update";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void testcase(unsigned long long *iterations, unsigned long nr)
{
	unsigned long val;

	while (1) {
		pthread_mutex_lock(&mutex);
		val++;
		pthread_mutex_unlock(&mutex);

		(*iterations)++;
	}
}
