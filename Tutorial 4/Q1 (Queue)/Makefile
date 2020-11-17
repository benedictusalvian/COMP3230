CC = gcc
AR = ar
CFLAGS = -g -Wall -fopenmp -L.

all: libqueue.a singleThreadTest.out multiThreadTest.out

# Compile queue.c and queue.h
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $^

# Convert queue.o into static library.
libqueue.a: queue.o
	$(AR) rcs $@ $^

# Linking static library
singleThreadTest.out: singleThreadTest.c libqueue.a
	$(CC) $(CFLAGS) -o $@ $^ -lqueue

# Linking static library
multiThreadTest.out: multiThreadTest.c libqueue.a
	$(CC) $(CFLAGS) -o $@ $^ -lqueue

.PHONY: clean

clean:
	rm -f *.o *.out *.so *.a *.gch
