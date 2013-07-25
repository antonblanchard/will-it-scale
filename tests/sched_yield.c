#include <sched.h>
#include <assert.h>

char *testcase_description = "sched_yield";

void testcase(unsigned long long *iterations)
{
	while (1) {
		assert(sched_yield() == 0);

		(*iterations)++;
	}
}
