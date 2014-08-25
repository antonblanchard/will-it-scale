#include <sched.h>
#include <assert.h>

char *testcase_description = "sched_yield";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	while (1) {
		assert(sched_yield() == 0);

		(*iterations)++;
	}
}
