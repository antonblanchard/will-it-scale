#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

char *testcase_description = "Separate file open/close in different directories";

#define template 	"/tmp/willitscale.XXXXXX"
static char (*tmpdirs)[sizeof(template)];
static unsigned long local_nr_tasks;

void testcase_prepare(unsigned long nr_tasks)
{
	int i;
	tmpdirs = (char(*)[sizeof(template)])malloc(sizeof(template) * nr_tasks);
	assert(tmpdirs);

	char tmpfile[PATH_MAX];
	int fd;

	for (i = 0; i < nr_tasks; i++) {
		strcpy(tmpdirs[i], template);
		char *tmpdir = tmpdirs[i];
		assert(mkdtemp(tmpdir) != NULL);
		sprintf(tmpfile, "%s/willitscale", tmpdir);
		fd = open(tmpfile, O_RDWR|O_CREAT, 0600);
		assert(fd >= 0);
		close(fd);
	}

	local_nr_tasks = nr_tasks;
}


void testcase(unsigned long long *iterations, unsigned long nr)
{
	int fd;
	char tmpfile[PATH_MAX];
	sprintf(tmpfile, "%s/willitscale", tmpdirs[nr]);

	while (1) {
		fd = open(tmpfile, O_RDWR);
		assert(fd >= 0);
		close(fd);

		(*iterations)++;
	}
}


void testcase_cleanup(void)
{
	int i;
	char tmpfile[PATH_MAX];

	for (i = 0; i < local_nr_tasks; i++) {
		sprintf(tmpfile, "%s/willitscale", tmpdirs[i]);
		unlink(tmpfile);
		rmdir(tmpdirs[i]);
	}
	free(tmpdirs);
}
