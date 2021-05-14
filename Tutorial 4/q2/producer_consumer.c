#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void time_printf(const char *funcName, const char *format, ...);

int setTimespec(struct timespec *ts, int sec, long nanoSec)
{
  int ret;
  if ((ret = clock_gettime(CLOCK_REALTIME, ts)) != 0)
  {
    /* handle error */
    time_printf(__func__, "Init clock failed\n");
  }
  else
  {
    ts->tv_sec += sec;
    ts->tv_nsec += nanoSec;
  }
  return ret;
}

int chefWaitTime = 5;
static struct timespec duty_ts;
static sem_t consumer;
static sem_t bread;
static sem_t space;

void *Chef(void *arg)
{
  int s;
  setTimespec(&duty_ts, chefWaitTime, 0);
  while (1)
  {
    s = sem_timedwait(&consumer, &duty_ts);
    if (s != 0)
    {
      if (errno == ETIMEDOUT)
      {
        time_printf(__func__, "No Consumer shows up in %d sec. Chef is off duty.\n", chefWaitTime);
        pthread_exit(NULL);
      }
      else
        perror("sem_timedwait");
    }
    time_printf(__func__, "requesting storage space...\n");
    if (sem_trywait(&space) == 0)
    {
      time_printf(__func__, "making bread...\n");
      sleep(2);
      sem_post(&bread);
      time_printf(__func__, "one bread made\n");
    }
  }
}

void *Consumer(void *arg)
{
  sem_post(&consumer);
  setTimespec(&duty_ts, chefWaitTime, 0);
  struct timespec wait_ts;
  int s;
  int waitTime = rand() % 4 + 2;
  time_printf(__func__, "requesting a piece of bread\n");
  setTimespec(&wait_ts, waitTime, 0);
  s = sem_timedwait(&bread, &wait_ts);
  if (s == -1)
  {
    if (errno == ETIMEDOUT)
    {
      time_printf(__func__, "waiting for %d sec and gives up.\n", waitTime);
      pthread_exit(NULL);
    }
    else
      perror("sem_timedwait");
  }
  time_printf(__func__, "hot bread got.\n");
  sem_post(&space);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  int num_space = 2;
  int num_consumer = 10;
  sem_init(&bread, 0, 0);
  sem_init(&space, 0, num_space);
  sem_init(&consumer, 0, 0);

  pthread_t chef;
  pthread_t consumers[num_consumer];

  int ret;
  if ((ret = pthread_create(&chef, NULL, Chef, NULL)) != 0)
  {
    printf("line: %d, pthread_create failed!\n", __LINE__);
    exit(-1);
  }

  for (int i = 0; i < num_consumer; ++i)
  {
    sleep(rand() % 2);
    if ((ret = pthread_create(&consumers[i], NULL, Consumer, NULL)) != 0)
    {
      printf("line: %d, pthread_create failed!\n", __LINE__);
      exit(-1);
    }
  }

  pthread_join(chef, NULL);

  for (int i = 0; i < num_consumer; ++i)
  {
    pthread_join(consumers[i], NULL);
  }

  int breadLeft;
  sem_getvalue(&bread, &breadLeft);
  if (breadLeft)
    printf("%d pieces of bread wasted!\n", breadLeft);
  else
    printf("No bread wasted!\n");

  return 0;
}

#define PRINT_BLUE               \
  fprintf(stderr, "\033[0;34m"); \
  printf("\033[0;34m");
#define PRINT_YELLOW_B           \
  fprintf(stderr, "\033[1;33m"); \
  printf("\033[1;33m");
#define PRINT_DEFAULT         \
  fprintf(stderr, "\033[0m"); \
  printf("\033[0m");
#define PRINT_CYAN               \
  fprintf(stderr, "\033[0;36m"); \
  printf("\033[0;36m");
static pthread_mutex_t print_mx;
static int print_mx_inited = 0;
static void initMX()
{
  pthread_mutex_init(&print_mx, NULL);
  print_mx_inited = 1;
}
// thread safe customized printf function
void time_printf(const char *funcName, const char *format, ...)
{
  if (!print_mx_inited)
    initMX();
  pthread_mutex_lock(&print_mx);
  PRINT_BLUE;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
  PRINT_YELLOW_B;
  printf("%s ", funcName);
  PRINT_CYAN;
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  fflush(stdout);
  PRINT_DEFAULT;
  pthread_mutex_unlock(&print_mx);
}
