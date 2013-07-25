#include <sys/types.h>
#include <unistd.h>

char *testcase_description = "getppid";

void testcase(unsigned long long *iterations)
{
	while (1) {
		getppid();

		(*iterations)++;
	}
}
