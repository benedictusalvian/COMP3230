#include "queue.h"

#define CHECK_FRONT                                                            \
  if (queueFront(q) != NULL)                                                   \
    printf("front: %d\n", *queueFront(q));                                     \
  else                                                                         \
    printf("front is NULL\n");                                                 \
  fflush(stdout);

#define CHECK_REAR                                                             \
  if (queueRear(q) != NULL)                                                    \
    printf("rear: %d\n", *queueRear(q));                                       \
  else                                                                         \
    printf("rear  is NULL\n");                                                 \
  fflush(stdout);

#define DEQUEUE_FRONT(ptr)                                                     \
  if (queueDequeueFront(q, ptr) == 0)                                          \
    printf("DQ front: %d\n", *ptr);                                            \
  else                                                                         \
    printf("DQ front: Empty q\n");                                             \
  fflush(stdout);

#define DEQUEUE_REAR(ptr)                                                      \
  if (queueDequeueRear(q, ptr) == 0)                                           \
    printf("DQ rear:  %d\n", *ptr);                                            \
  else                                                                         \
  printf("DQ rear: Empty q\n");                                              \
  fflush(stdout);

#define ENQUEUE(ptr)                                                           \
  if (queueEnqueue(q, ptr) == 0)                                               \
    printf("%d enqueued\n", *ptr);                                             \
  else                                                                         \
    printf("enqueue %d failed\n", *ptr);                                       \
  fflush(stdout);

#define ENQUEUE_NO_PIRNT(ptr)                                                  \
  if (queueEnqueue(q, ptr) != 0)                                               \
    printf("enqueue %d failed\n", *ptr);                                       \
  fflush(stdout);

int main(int argc, char **argv) {
  int element;
  Queue q = queueCreate(4);
  queuePrint(q);
  CHECK_FRONT
  CHECK_REAR
  DEQUEUE_FRONT(&element)
  DEQUEUE_REAR(&element)

  for (int i = 0; i < 3; i++) {
    ENQUEUE(&i)
  }
  queuePrint(q);
  for (int i = 3; i < 7; i++) {
    ENQUEUE(&i)
  }
  queuePrint(q);
  CHECK_FRONT
  CHECK_REAR

  DEQUEUE_FRONT(&element)
  DEQUEUE_FRONT(&element)
  DEQUEUE_REAR(&element)
  queuePrint(q);

  for (int i = 5; i < 8; i++) {
    ENQUEUE(&i)
  }
  queuePrint(q);

  DEQUEUE_FRONT(&element)
  DEQUEUE_REAR(&element)
  queuePrint(q);

  queueDestroy(q);

  int sizeLarge = 1000000;
  printf("\nCreate queue with capacity = %d\n", sizeLarge);
  q = queueCreate(sizeLarge);
  for (int i = 0; i < sizeLarge + 4; i++) {
    ENQUEUE_NO_PIRNT(&i)
  }

  CHECK_FRONT
  CHECK_REAR
  DEQUEUE_FRONT(&element)
  DEQUEUE_FRONT(&element)
  DEQUEUE_REAR(&element)

  for (int i = 5; i < 8; i++) {
    ENQUEUE(&i)
  }
  CHECK_FRONT
  CHECK_REAR

  DEQUEUE_FRONT(&element)
  DEQUEUE_REAR(&element)
  CHECK_FRONT
  CHECK_REAR

  queueDestroy(q);
  return 0;
}
