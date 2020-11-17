#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

/* You may need to define struct here */
typedef struct arguments
{
  const float *vec;
  int subArrayLength;
} arguments;

typedef struct returnValue
{
  double sum;
} returnValue;

/*!
 * \brief subroutine function
 *
 * \param arg, input arguments pointer
 * \return void*, return pointer
 */
void *l2_norm(void *arg)
{ /* TODO: Your code here */
  const float *vec;

  arguments *args = (arguments *)arg;
  vec = args->vec;

  double sum = 0.0f;

  for (int i = 0; i < args->subArrayLength; i++)
    sum += vec[i] * vec[i];

  returnValue *rval = malloc(sizeof(returnValue));
  rval->sum = sum;

  return (void *)rval;
}

/*!
 * \brief wrapper function
 *
 * \param vec, input vector array
 * \param len, length of vector
 * \param k, number of threads
 * \return float, l2 norm
 */
float multi_thread_l2_norm(const float *vec, size_t len, int k)
{ /* TODO: your code here */
  pthread_t *threads = malloc(sizeof(pthread_t) * k);
  arguments *args = malloc(sizeof(arguments) * k);

  int subArrayLength = len / k;

  for (int i = 0; i < k; i++)
  {
    args[i].vec = vec + i * subArrayLength;
    args[i].subArrayLength = subArrayLength;
  }

  for (int i = 0; i < k; i++)
    pthread_create(&(threads[i]), NULL, l2_norm, (void *)&args[i]);

  double sum = 0.0f;
  returnValue *rval;

  for (int i = 0; i < k; i++)
  {
    pthread_join(threads[i], (void **)&rval);
    sum += rval->sum;
  }

  free(threads);
  free(args);

  return sqrt(sum);
}

// baseline function
float single_thread_l2_norm(const float *vec, size_t len)
{
  double sum = 0.0f;
  for (int i = 0; i < len; ++i)
  {
    sum += vec[i] * vec[i];
  }
  sum = sqrt(sum);
  return sum;
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

void init_array(float *p, size_t len)
{
  for (int i = 0; i < len; ++i)
  {
    p[i] = (float)rand() / RAND_MAX;
  }
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

  float *vec = gen_array(num);
  init_array(vec, num);

  float your_result, groud_truth;
  // warmup
  single_thread_l2_norm(vec, num);

  {
    struct timespec start, end;
    printf("[Singlethreading]start\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    groud_truth = single_thread_l2_norm(vec, num);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                     (end.tv_nsec - start.tv_nsec) * 1.0e-3;
    printf("[Singlethreading]The elapsed time is %.2f ms.\n",
           delta_us / 1000.0);
  }

  {
    struct timespec start, end;
    printf("[Multithreading]start\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    your_result = multi_thread_l2_norm(vec, num, k);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    float delta_us = (end.tv_sec - start.tv_sec) * 1.0e6 +
                     (end.tv_nsec - start.tv_nsec) * 1.0e-3;
    printf("[Multithreading]The elapsed time is %.2f ms.\n", delta_us / 1000.0);
  }

  if (fabs(your_result - groud_truth) < 1e-6)
  {
    printf("Accepted!\n");
  }
  else
  {
    printf("Wrong Answer!\n");
    printf("your result=%.6f\tground truth=%.6f\n", your_result, groud_truth);
  }

  free(vec);
  return 0;
}