#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>

char *testcase_description = "Contended pthread mutex with local update";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void testcase(unsigned long long *iterations, unsigned long nr)
{
	// alloca() because plain "unsigned long val" gets optimized away.
	unsigned long *val = alloca(sizeof(unsigned long));
	*(val) = 0;

	while (1) {
		pthread_mutex_lock(&mutex);
		(*val)++;
		pthread_mutex_unlock(&mutex);

		(*iterations)++;
	}
}
