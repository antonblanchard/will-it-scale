CFLAGS=-Wall -O2 -g
LDFLAGS=-lhwloc

processes := $(patsubst tests/%.c,%_processes,$(wildcard tests/*.c))
threads := $(patsubst tests/%.c,%_threads,$(wildcard tests/*.c))

all: processes threads

processes: $(processes)

threads: $(threads)

posix_semaphore1_processes_FLAGS=-lpthread

$(processes): %_processes: tests/%.o main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $($@_FLAGS)  main.c $< -o $@

$(threads): %_threads: tests/%.o main.c
	$(CC) -DTHREADS $(CFLAGS) $(LDFLAGS) -lpthread main.c $< -o $@

clean:
	rm -f tests/*.o *_processes *_threads
