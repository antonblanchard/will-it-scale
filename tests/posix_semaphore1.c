#include <semaphore.h>

char *testcase_description = "POSIX semaphores";

void testcase(unsigned long long *iterations)
{
	sem_t sem;

	sem_init(&sem, 0, 1);

	while (1) {
		sem_wait(&sem);
		sem_post(&sem);
		(*iterations)++;
	}
}
