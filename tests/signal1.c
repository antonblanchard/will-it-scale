#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int junk)
{
	/* Do Nothing */
}

char *testcase_description = "signal delivery";

void testcase(unsigned long long *iterations)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	sigaction(SIGUSR1, &act, NULL);

	while (1) {
		raise(SIGUSR1);

		(*iterations)++;
	}
}
