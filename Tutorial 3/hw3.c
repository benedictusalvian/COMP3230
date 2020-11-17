#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _context_t
{
  uint64_t id;
  char *name;
  bool initialized;
} context_t;

context_t *ctx = NULL;
pthread_mutex_t lock;

// singleton
context_t *get_instance()
{
  pthread_mutex_lock(&lock);
  if (ctx == NULL)
  {
    ctx = (context_t *)malloc(sizeof(context_t));
    assert(ctx != NULL);
    ctx->initialized = false;
  }
  pthread_mutex_unlock(&lock);
  return ctx;
}

int id = 0;

void *do_work(void *arg)
{
  context_t *ctx = get_instance();
  pthread_mutex_lock(&lock);
  if (!ctx->initialized)
  {
    ctx->name = (char *)arg;
    ctx->id = ++id;
    ctx->initialized = true;
  }
  pthread_mutex_unlock(&lock);
  printf("name=%s\tid=%ld\n", ctx->name, ctx->id);
  return NULL;
}

int main(int argc, char *argv[])
{
  int rc;
  pthread_t p1, p2;

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
  pthread_mutex_destroy(&lock);
  return 0;
}