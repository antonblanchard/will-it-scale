#define _GNU_SOURCE
#include <pthread.h>

char *testcase_description = "Contended pthread mutex";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void testcase(unsigned long long *iterations, unsigned long nr)
{
	while (1) {
		pthread_mutex_lock(&mutex);
		pthread_mutex_unlock(&mutex);

		(*iterations)++;
	}
}
