#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 1000000
long long counter = 0;
sem_t sem01;

void *counting_thread(void *arg)
{
	int offset = *(int *)arg;
	int i;

	sem_wait(&sem01);
	for (i = 0; i < N; i++)
	{
		counter += offset;
	}
	sem_post(&sem01);

	pthread_exit(NULL);
}

int main()
{
	sem_init(&sem01, 0, 1);

	pthread_t th1;
	int offset = 1;
	pthread_create(&th1, NULL, counting_thread, &offset);

	pthread_t th2;
	int offset2 = -1;
	pthread_create(&th2, NULL, counting_thread, &offset2);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	printf("Final counter value: %lld\n", counter);

	sem_destroy(&sem01);
	return 0;
}
