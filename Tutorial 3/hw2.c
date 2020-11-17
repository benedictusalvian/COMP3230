#define _GNU_SOURCE
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <unistd.h>

/* You may need to define struct here */
typedef struct arguments
{
  float *src;
  float *dest;
  int subArrayLength;
} arguments;

/*!
 * \brief subroutine function
 *
 * \param arg, input arguments pointer
 * \return void*, return pointer
 */
void *mt_memcpy(void *arg)
{ /* TODO: Your code here */
  arguments *args = (arguments *)arg;

  for (int i = 0; i < args->subArrayLength; i++)
    args->dest[i] = args->src[i];

  return NULL;
}

/*!
 * \brief wrapper function
 *
 * \param dst, destination pointer
 * \param src, source pointer
 * \param size, copy bytes
 * \param k, number of threads
 */
void multi_thread_memcpy(void *dst, const void *src, size_t size, int k)
{ /* TODO: your code here */
  pthread_t *threads = malloc(sizeof(pthread_t) * k);
  arguments *args = malloc(sizeof(arguments) * k);

  int subArrayLength = (size / sizeof(int)) / k;

  for (int i = 0; i < k; i++)
  {
    args[i].src = (float *)src + i * subArrayLength;
    args[i].dest = (float *)dst + i * subArrayLength;
    args[i].subArrayLength = subArrayLength;
  }

  for (int i = 0; i < k; i++)
    pthread_create(&threads[i], NULL, mt_memcpy, (void *)&args[i]);

  for (int i = 0; i < k; i++)
    pthread_join(threads[i], NULL);

  free(threads);
  free(args);
}

/*!
 * \brief bind new threads to the same NUMA node as the main
 * thread and then do multithreading memcy
 *
 * \param dst, destination pointer
 * \param src, source pointer
 * \param size, copy bytes
 * \param k, number of threads
 */
void multi_thread_memcpy_with_affinity(void *dst, const void *src, size_t size, int k)
{ /* TODO: your code here */
  pthread_t ph[k];
  int rc;
  cpu_set_t cpu_set[k];
  pthread_attr_t attr[k];
  for (int i = 0; i < k; ++i)
  {
    rc = pthread_attr_init(&attr[i]);
    assert(rc == 0);
  }

  int cpu;
  rc = syscall(SYS_getcpu, &cpu, NULL, NULL);
  assert(rc == 0);
  printf("Main thread runs on CPU %d\n", cpu);
  int start = cpu % 2;

  printf("Set affinity mask to include CPUs (%d, %d, %d, ...  %s)\n", start,
         start + 2, start + 4, start ? "2n+1" : "2n");
  size_t nprocs = get_nprocs();
  int i = 0, j = start;
  while (i < k)
  {
    if (j == cpu)
    {
      j += 2;
      continue;
    }
    if (j >= nprocs)
    {
      j = start;
    }

    CPU_ZERO(&cpu_set[i]);
    CPU_SET(j, &cpu_set[i]);
    pthread_attr_setaffinity_np(&attr[i], sizeof(cpu_set_t), &cpu_set[i]);
    ++i;
    j += 2;
  }

  arguments args[k];
  int subArrayLength = (size / sizeof(int)) / k;

  for (int i = 0; i < k; i++)
  {
    args[i].dest = (float *)dst + i * subArrayLength;
    args[i].src = (float *)src + i * subArrayLength;
    args[i].subArrayLength = subArrayLength;
  }

  for (int i = 0; i < k; i++)
    pthread_create(&ph[i], &attr[i], mt_memcpy, (void *)&args[i]);

  for (int i = 0; i < k; i++)
    pthread_join(ph[i], NULL);
}

void single_thread_memcpy(void *dst, const void *src, size_t size)
{
  float *in = (float *)src;
  float *out = (float *)dst;

  for (size_t i = 0; i < size / 4; ++i)
  {
    out[i] = in[i];
  }
  if (size % 4)
  {
    memcpy(out + size / 4, in + size / 4, size % 4);
  }
}

float *gen_array(size_t len)
{
  float *p = (float *)malloc(len * sizeof(float));
  if (!p)
  {
    printf("failed to allocate %ld bytes memory!", len * sizeof(float));
    exit(1);
  }
  return p;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr,
            "Error: The program accepts exact 2 intergers.\n The first is the "
            "vector size and the second is the number of threads.\n");
    exit(1);
  }
  const int num = atoi(argv[1]);
  const int k = atoi(argv[2]);
  if (num < 0 || k < 1)
  {
    fprintf(stderr, "Error: invalid arguments.\n");
    exit(1);
  }

  printf("Vector size=%d\tthreads num=%d.\n", num, k);

  float *dst = gen_array(num);
  float *src = gen_array(num);

  // warmup
  memcpy(dst, src, num * sizeof(float));
  {
    struct timespec start, end;
    printf("[C library: memcpy]start\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    memcpy(dst, src, num * sizeof(k));

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                     (end.tv_nsec - start.tv_nsec) * 1.0e-3;
    printf("[C library: memcpy]The throughput is %.2f Gbps.\n",
           num * sizeof(float) * 8 / (delta_us * 1000.0));
  }

  {
    struct timespec start, end;
    printf("[Singlethreading]start\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    single_thread_memcpy(dst, src, num * sizeof(k));

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                     (end.tv_nsec - start.tv_nsec) * 1.0e-3;
    printf("[Singlethreading]The throughput is %.2f Gbps.\n",
           num * sizeof(float) * 8 / (delta_us * 1000.0));
  }

  {
    struct timespec start, end;
    printf("[Multithreading]start\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    multi_thread_memcpy(dst, src, num * sizeof(k), k);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                     (end.tv_nsec - start.tv_nsec) * 1.0e-3;
    printf("[Multithreading]The throughput is %.2f Gbps.\n",
           num * sizeof(float) * 8 / (delta_us * 1000.0));
  }

  {
    struct timespec start, end;
    printf("[Multithreading with affinity]start\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    multi_thread_memcpy_with_affinity(dst, src, num * sizeof(k), k);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                     (end.tv_nsec - start.tv_nsec) * 1.0e-3;
    printf("[Multithreading with affinity]The throughput is %.2f Gbps.\n",
           num * sizeof(float) * 8 / (delta_us * 1000.0));
  }

  free(dst);
  free(src);
  return 0;
}
