#include <semaphore.h>
#include <assert.h>
#include <pthread.h>
#include "queue.h"

static sem_t threadCnt;
static int concurrentThreadLimit = 256;
static int limited_pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
    void *(*start_routine)(void *), void *arg) {
  sem_wait(&threadCnt);
  return pthread_create(thread, attr, start_routine, arg);
}

void *enqueueThread(void *arg) {
  Queue q = (Queue)arg;
  int i = 1;
  if (queueEnqueue(q, &i) == -1)
    printf("[!!!]enqueueThread failed!\n");
  sem_post(&threadCnt);
  pthread_exit(NULL);
}

void *dequeueRearThread(void *arg) {
  Queue q = (Queue)arg;
  int i;
  if (queueDequeueRear(q, &i) == -1)
    printf("[!!!]dequeueRearThread failed!\n");
  sem_post(&threadCnt);
  pthread_exit(NULL);
}

void *dequeueFrontThread(void *arg) {
  Queue q = (Queue)arg;
  int i;
  if (queueDequeueFront(q, &i) == -1)
    printf("[!!!]dequeueFrontThread failed!\n");
  sem_post(&threadCnt);
  pthread_exit(NULL);
}

void test1(int num_pthreads) {
  int i, rc, tmp;
  Queue q = queueCreate(num_pthreads);
  pthread_t *threads = malloc(num_pthreads * sizeof(pthread_t));
  num_pthreads = num_pthreads;
  printf("-------Run %s-------\n", __func__);
  printf("====Enqueuing...");
  for (i = 0; i < num_pthreads; i++) {
    if ((rc = limited_pthread_create(&threads[i], NULL, enqueueThread, (void *)q))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  for (i = 0; i < num_pthreads; i++)
    pthread_join(threads[i], NULL);
  int sum = 0;
  for (i = 0; i < num_pthreads; i++)
    sum += q->array[i];
  if (queueIsFull(q) && q->size == num_pthreads && sum)
    printf(" Pass.\n");
  else {
    printf(" Fail! \nQueue is Full: %d, size = %d\n", queueIsFull(q), q->size);
    printf("Queue sum = %d, num_pthreads = %d\n", sum, num_pthreads);
  }
  assert(queueIsFull(q) && q->size == num_pthreads && sum);

  int queueSize = q->size;
  printf("====Dequeuing Rear...");
  for (i = 0; i < queueSize; i++) {
    if ((rc = limited_pthread_create(&threads[i], NULL, dequeueRearThread, (void *)q))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  for (i = 0; i < queueSize; i++)
    pthread_join(threads[i], NULL);
  if (queueIsEmpty(q))
    printf(" Pass.\n");
  else
    printf(" Fail!\nQueue is Empty: %d, size = %d\n", queueIsEmpty(q), q->size);
  assert(queueIsEmpty(q));
  while (queueDequeueRear(q, &tmp) == 0) {
  }

  printf("====Dequeuing Front...");
  for (i = 0; i < num_pthreads; i++)
    queueEnqueue(q, &num_pthreads);
  for (i = 0; i < num_pthreads; i++) {
    if ((rc = limited_pthread_create(&threads[i], NULL, dequeueFrontThread, (void *)q))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  for (i = 0; i < num_pthreads; i++)
    pthread_join(threads[i], NULL);
  if (queueIsEmpty(q))
    printf(" Pass.\n");
  else
    printf(" Fail!\nQueue is Empty: %d, size = %d\n", queueIsEmpty(q), q->size);
  assert(queueIsEmpty(q));
  while (queueDequeueRear(q, &tmp) == 0) {
  }
  queueDestroy(q);
  free(threads);
}

void *cntEnqueueThread(void *arg) {
  Queue q = (Queue)arg;
  srand(time(NULL));
  int e_num = rand() % 10 + 1;
  int i = 1;
  for (int kI = 0; kI < e_num; ++kI) {
    assert(queueEnqueue(q, &i) == 0);
  }
  sem_post(&threadCnt);
  pthread_exit(NULL);
}

void *cntDequeueRearThread(void *arg) {
  Queue q = (Queue)arg;
  int i;
  assert(queueDequeueRear(q, &i) == 0);
  sem_post(&threadCnt);
  pthread_exit(NULL);
}

void *cntDequeueFrontThread(void *arg) {
  Queue q = (Queue)arg;
  int i;
  assert(queueDequeueFront(q, &i) == 0);
  sem_post(&threadCnt);
  pthread_exit(NULL);
}

void test2(int num_pthreads) {
  printf("-------Run %s-------\n", __func__);
  int rc;
  int limThreadCnt;
  Queue q = queueCreate(num_pthreads * 10);
  pthread_t *enqueueThreads = malloc(num_pthreads * sizeof(pthread_t));
  for (int i = 0; i < num_pthreads; i++) {
    if ((rc = limited_pthread_create(&enqueueThreads[i], NULL, cntEnqueueThread, (void *)q))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < num_pthreads; i++) { 
    pthread_join(enqueueThreads[i], NULL);
  } 
  sem_getvalue(&threadCnt, &limThreadCnt);
  assert(limThreadCnt == concurrentThreadLimit);
  int enqueueCntNum = q->size;
  int numDQFront = rand() % enqueueCntNum + 1;
  int numDQRear = enqueueCntNum - numDQFront;
  assert(enqueueCntNum == q->size);
  pthread_t * DQFrontThreads = malloc(numDQFront * sizeof(pthread_t));
  pthread_t * DQRearThreads = malloc(numDQRear * sizeof(pthread_t));
  for (int i = 0; i < numDQFront; i++) {
    if ((rc = limited_pthread_create(&DQFrontThreads[i], NULL, cntDequeueFrontThread, (void *)q))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < numDQRear; i++) {
    if ((rc = limited_pthread_create(&DQRearThreads[i], NULL, cntDequeueRearThread, (void *)q))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  sem_getvalue(&threadCnt, &limThreadCnt);
  assert(limThreadCnt <= concurrentThreadLimit);
  for (int i = 0; i < numDQFront; i++) { 
    pthread_join(DQFrontThreads[i], NULL);
  } 
  for (int i = 0; i < numDQRear; i++) { 
    pthread_join(DQRearThreads[i], NULL);
  } 
  assert(queueIsEmpty(q));
  sem_getvalue(&threadCnt, &limThreadCnt);
  assert(limThreadCnt == concurrentThreadLimit);

  free(DQRearThreads);
  free(DQFrontThreads);

  queueDestroy(q);
  free(enqueueThreads);
  printf("%s PASS.\n", __func__);
}

int main(int argc, char **argv) {
  sem_init(&threadCnt, 0, concurrentThreadLimit);
  srand(time(NULL));

  for (int i = 0; i < 10; ++i) test1(rand() % 10001 + 20000); 

  for (int i = 0; i < 10; ++i) test2(rand() % 1001 + 2000); 

  return 0;
}
