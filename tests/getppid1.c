#include <sys/types.h>
#include <unistd.h>

char *testcase_description = "getppid";

void testcase(unsigned long long *iterations, unsigned long nr)
{
	while (1) {
		getppid();

		(*iterations)++;
	}
}
