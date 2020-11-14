/*!
 * \file hw3d.c
 * \author Yuchen Zhong (izhongyuchen@gmail.com)
 * \brief debug version of hw3.c
 * \date 2020-11-05
 *
 */
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_barrier_t barrier;

typedef struct _context_t
{
  uint64_t id;
  char *name;
  bool initialized;
} context_t;

context_t *ctx = NULL;

// singleton
context_t *get_instance()
{
  if (ctx == NULL)
  {
    ctx = (context_t *)malloc(sizeof(context_t));
    assert(ctx != NULL);
    ctx->initialized = false;
  }
  return ctx;
}

int id = 0;

void *do_work(void *arg)
{
  pthread_barrier_wait(&barrier);
  context_t *ctx = get_instance();
  if (!ctx->initialized)
  {
    ctx->name = (char *)arg;
    ctx->id = ++id;
    ctx->initialized = true;
  }
  printf("name=%s\tid=%ld\n", ctx->name, ctx->id);
  return NULL;
}

int main(int argc, char *argv[])
{
  int rc;
  pthread_t p1, p2;
  pthread_barrier_init(&barrier, NULL, 2);

  rc = pthread_create(&p1, NULL, do_work, "A");
  assert(rc == 0);
  rc = pthread_create(&p2, NULL, do_work, "B");
  assert(rc == 0);

  rc = pthread_join(p1, NULL);
  assert(rc == 0);
  rc = pthread_join(p2, NULL);
  assert(rc == 0);

  if (ctx)
  {
    free(ctx);
  }
  return 0;
}