#define _GNU_SOURCE
#include <pthread.h>

char *testcase_description = "Contended pthread mutex with TLS update";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
__thread unsigned long val;

void testcase(unsigned long long *iterations, unsigned long nr)
{
	while (1) {
		pthread_mutex_lock(&mutex);
		val++;
		pthread_mutex_unlock(&mutex);

		(*iterations)++;
	}
}
