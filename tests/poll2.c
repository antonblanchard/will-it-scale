#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>

#define NR_FILES 128

char *testcase_description = "poll of 128 fds";

void testcase_prepare(unsigned long nr_tasks)
{
	struct rlimit rlim;
	int nr_procs = sysconf(_SC_NPROCESSORS_CONF);
	rlim_t new_lim = (NR_FILES + 10) * nr_procs;

	getrlimit(RLIMIT_NOFILE, &rlim);
	if( rlim.rlim_max < new_lim) {
		rlim.rlim_cur = new_lim;
		rlim.rlim_max = new_lim;
	}
	assert(setrlimit(RLIMIT_NOFILE, &rlim) == 0);
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	int i;
	char tmpfile[PATH_MAX];
	int tmpfiles[NR_FILES];

	for (i = 0; i < NR_FILES; i++) {
		sprintf(tmpfile, "/tmp/willitscale.XXXXXX");
		tmpfiles[i] = mkstemp(tmpfile);
		assert(tmpfiles[i] >= 0);
		unlink(tmpfile);
	}

	while (1) {
		struct pollfd pfd[NR_FILES];

		memset(&pfd, 0, sizeof(pfd));

		for (i = 0; i < NR_FILES; i++) {
			pfd[i].fd = tmpfiles[i];
			pfd[i].events = POLLOUT;
		}

		assert(poll(pfd, NR_FILES, 0) >= 0);

		(*iterations)++;
	}
}
