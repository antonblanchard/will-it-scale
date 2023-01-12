#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

char *testcase_description = "Separate file access";

#define template 	"/tmp/willitscale.XXXXXX"
static char (*tmpfiles)[sizeof(template)];
static unsigned long local_nr_tasks;

void testcase_prepare(unsigned long nr_tasks)
{
	int i;
	tmpfiles = (char(*)[sizeof(template)])malloc(sizeof(template) * nr_tasks);
	assert(tmpfiles);

	for (i = 0; i < nr_tasks; i++) {
		strcpy(tmpfiles[i], template);
		char *tmpfile = tmpfiles[i];
		int fd = mkstemp(tmpfile);

		assert(fd >= 0);
		close(fd);
	}

	local_nr_tasks = nr_tasks;
}

void testcase(unsigned long long *iterations, unsigned long nr)
{
	char *tmpfile = tmpfiles[nr];

	while (1) {
		int error = access(tmpfile, R_OK);
		assert(error == 0);

		(*iterations)++;
	}
}

void testcase_cleanup(void)
{
	int i;
	for (i = 0; i < local_nr_tasks; i++) {
		unlink(tmpfiles[i]);
	}
	free(tmpfiles);
}
