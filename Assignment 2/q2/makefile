CC = gcc
AR = ar
CFLAGS = -Wall -L../lib -pthread -fopenmp -Wl,--no-as-needed -ldl
RM_TMP = rm -f *.o include/*.gch

all: tesla_factory.out 
	$(RM_TMP)

debug: CFLAGS += -DDEBUG -ggdb  
debug: tesla_factory_debug.out
	$(RM_TMP)

queue.o: include/queue.h src/queue.c
	$(CC) $(CFLAGS) -c $^ 

scheduler.o: include/robot.h include/scheduler.h src/scheduler.c
	$(CC) $(CFLAGS) -c $^ 

tesla_factory_debug.out: include/robot.h src/main.c scheduler.o queue.o
	$(CC) $(CFLAGS) -o $@ $^ -lTeslaFactoryDebug

tesla_factory.out: include/robot.h src/main.c scheduler.o queue.o
	$(CC) $(CFLAGS) -o $@ $^ -lTeslaFactory

.PHONY: clean

clean:
	rm -f *.out; $(RM_TMP)
