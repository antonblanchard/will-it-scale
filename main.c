/*
 * Copyright (C) 2010 Anton Blanchard <anton@au.ibm.com>, IBM
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <hwloc.h>
#include <sys/types.h>
#include <signal.h>
#include <poll.h>

#define MAX_TASKS 1024
#define CACHELINE_SIZE 128
#define WARMUP_ITERATIONS 5

extern char *testcase_description;
extern void __attribute__((weak)) testcase_prepare(unsigned long nr_tasks) { }
extern void __attribute__((weak)) testcase_cleanup(void) { }
extern void *testcase(unsigned long long *iterations, unsigned long nr);

static char *initialise_shared_area(unsigned long size)
{
	char template[] = "/tmp/shared_area_XXXXXX";
	int fd;
	char *m;
	int page_size = getpagesize();

	/* Align to page boundary */
	size = (size + page_size-1) & ~(page_size-1);

	fd = mkstemp(template);
	if (fd < 0) {
		perror("mkstemp");
		exit(1);
	}

	if (ftruncate(fd, size) == -1) {
		perror("ftruncate");
		unlink(template);
		exit(1);
	}

	m = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (m == MAP_FAILED) {
		perror("mmap");
		unlink(template);
		exit(1);
	}

	memset(m, 0, size);

	unlink(template);

	return m;
}

static void usage(char *command)
{
	printf("Usage: %s [options]\n\n", command);
	printf("\t-s iterations\tNumber of iterations to run\n");
	printf("\t-t tasks\tNumber of threads or processes to run\n");
	printf("\t-m\t\tAffinitize tasks on SMT threads (default cores)\n");
	exit(1);
}

struct args
{
	void *(*func)(unsigned long long *iterations, unsigned long nr);
	unsigned long long *arg1;
	unsigned long arg2;
	int poll_fd;
	hwloc_topology_t topology;
	hwloc_cpuset_t cpuset;
};

static void *testcase_trampoline(void *p)
{
	struct args *args = p;
	struct pollfd pfd = { args->poll_fd, POLLIN, 0 };

	poll(&pfd, 1, -1);

	return args->func(args->arg1, args->arg2);
}

#ifdef THREADS

#include <pthread.h>

void new_task(void *(func)(void *), void *arg)
{
	pthread_t tid;

	pthread_create(&tid, NULL, func, arg);
}

static void *thread_pre_trampoline(void *p)
{
	struct args *args;

	args = (struct args *)p;
	if (hwloc_set_thread_cpubind(args->topology, pthread_self(),
	    args->cpuset, 0)) {
		perror("hwloc_set_thread_cpubind");
		exit(1);
	}
	hwloc_topology_destroy(args->topology);
	hwloc_bitmap_free(args->cpuset);
	return testcase_trampoline(args);
}

void new_task_affinity(struct args *args)
{
	pthread_attr_t attr;
	pthread_t tid;

	pthread_attr_init(&attr);

	pthread_create(&tid, &attr, thread_pre_trampoline, args);

	pthread_attr_destroy(&attr);
}

/* All threads will die when we exit */
static void kill_tasks(void)
{
}

#else
#include <signal.h>

static int pids[MAX_TASKS];
static int nr_pids;

/* Watchdog used to make sure all children exit when the parent does */
static int parent_pid;
static void watchdog(int junk)
{
	if (kill(parent_pid, 0) == -1)
		exit(0);

	alarm(1);
}

void new_task(void *(func)(void *), void *arg)
{
	int pid;

	parent_pid = getpid();

	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	}

	if (!pid) {
		struct sigaction sa;

		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = watchdog;
		sa.sa_flags = SA_RESTART;
		sigaction(SIGALRM, &sa, NULL);
		alarm(1);

		func(arg);
	}

	pids[nr_pids++] = pid;
}

void new_task_affinity(struct args *args)
{
	hwloc_cpuset_t old_set;
	int pid;


	old_set = hwloc_bitmap_alloc();
	if (old_set == NULL) {
		perror("hwloc_bitmap_alloc");
		exit(1);
	}
	if (hwloc_get_cpubind(args->topology, old_set,
	    HWLOC_CPUBIND_PROCESS) < 0) {
		perror("hwloc_get_cpubind");
		exit(1);
	}
	if (hwloc_set_cpubind(args->topology, args->cpuset,
	    HWLOC_CPUBIND_PROCESS) < 0) {
		perror("hwloc_set_cpubind");
		exit(1);
	}

	parent_pid = getpid();

	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	}

	if (!pid) {
		struct sigaction sa;

		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = watchdog;
		sa.sa_flags = SA_RESTART;
		sigaction(SIGALRM, &sa, NULL);
		alarm(1);

		testcase_trampoline(args);
	}

	if (hwloc_set_cpubind(args->topology, old_set,
	    HWLOC_CPUBIND_PROCESS) < 0) {
		perror("hwloc_set_cpubind");
		exit(1);
	}

	pids[nr_pids++] = pid;
	hwloc_topology_destroy(args->topology);
	hwloc_bitmap_free(args->cpuset);
	hwloc_bitmap_free(old_set);

}


static void kill_tasks(void)
{
	int i;

	for (i = 0; i < nr_pids; i++)
		kill(pids[i], SIGTERM);
}
#endif

int main(int argc, char *argv[])
{
	int opt_tasks = 1;
	int opt_iterations = 0;
	int iterations = 0;
	int i, n;
	char *m;
	static unsigned long long *results[MAX_TASKS];
	hwloc_topology_t topology;
	unsigned long long prev[MAX_TASKS] = {0, };
	unsigned long long total = 0;
	int fd[2];
	bool smt_affinity = false;

	while (1) {
		signed char c = getopt(argc, argv, "mt:s:h");
		if (c < 0)
			break;

		switch (c) {
			case 'm':
				smt_affinity = true;
				break;

			case 't':
				opt_tasks = atoi(optarg);
				if (opt_tasks > MAX_TASKS) {
					printf("tasks cannot exceed %d\n",
					       MAX_TASKS);
					exit(1);
				}
				break;

			case 's':
				opt_iterations = atoi(optarg);
				break;

			default:
				usage(argv[0]);
		}
	}

	if (optind < argc)
		usage(argv[0]);

	m = initialise_shared_area(opt_tasks * CACHELINE_SIZE);
	for (i = 0; i < opt_tasks; i++)
		results[i] = (unsigned long long *)&m[i * CACHELINE_SIZE];

	if (pipe(fd) == -1) {
		perror("pipe");
		exit(1);
	}

	testcase_prepare(opt_tasks);

	hwloc_topology_init(&topology);
	hwloc_topology_load(topology);

	n = hwloc_get_nbobjs_by_type(topology,
			smt_affinity ? HWLOC_OBJ_PU : HWLOC_OBJ_CORE);
	for (i = 0; i < opt_tasks; i++) {
		hwloc_obj_t obj;
		struct args *args;

		args = malloc(sizeof(struct args));
		if (!args) {
			perror("malloc");
			exit(1);
		}
		args->func = testcase;
		args->arg1 = results[i];
		args->arg2 = i;
		args->poll_fd = fd[0];
		obj = hwloc_get_obj_by_type(topology,
				smt_affinity ? HWLOC_OBJ_PU : HWLOC_OBJ_CORE,
				i % n);
		if ((args->cpuset = hwloc_bitmap_dup(obj->cpuset)) == NULL) {
			perror("hwloc_bitmap_dup");
			exit(1);
		}
		if (hwloc_topology_dup(&args->topology, topology)) {
			perror("hwloc_topology_dup");
			exit(1);
		}
		new_task_affinity(args);
	}

	if (write(fd[1], &i, 1) != 1) {
		perror("write");
		exit(1);
	}

	hwloc_topology_destroy(topology);

	printf("testcase:%s\n", testcase_description);

	printf("warmup\n");

	while (1) {
		unsigned long long sum = 0, min = -1ULL, max = 0;

		sleep(1);

		for (i = 0; i < opt_tasks; i++) {
			unsigned long long val = *(results[i]);
			unsigned long long diff = val - prev[i];

			if (diff < min)
				min = diff;

			if (diff > max)
				max = diff;

			sum += diff;
			prev[i] = val;
		}

		printf("min:%llu max:%llu total:%llu\n", min, max, sum);

		if (iterations == WARMUP_ITERATIONS)
			printf("measurement\n");

		if (iterations++ > WARMUP_ITERATIONS)
			total += sum;

		if (opt_iterations &&
		    (iterations > (opt_iterations + WARMUP_ITERATIONS))) {
			printf("average:%llu\n", total / opt_iterations);
			testcase_cleanup();
			kill_tasks();
			exit(0);
		}
	}
}
