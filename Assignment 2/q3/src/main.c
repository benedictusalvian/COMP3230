#include "../include/production.h"
#include "../include/utils.h"
#include "../include/scheduler.h"
#include "../include/robot.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int num_cars;
static int num_spaces;
static int num_typeA;
static int num_typeB;
static int num_typeC;
static int total_num_robots;

pthread_mutex_t lock;

void startProduction()
{
  /* Init robots */
  Robot *robotsA = calloc(num_typeA, sizeof(Robot_t));
  Robot *robotsB = calloc(num_typeB, sizeof(Robot_t));
  Robot *robotsC = calloc(num_typeC, sizeof(Robot_t));
  /* Init robots end*/
  /* Prepare task */
  Task task = calloc(1, sizeof(Task_t));
  task->jobQ = queueCreate(num_cars * 17);

  task->lock = lock;

  for (int k = 0; k < num_cars; k++)
  {
    int i = 6; // BODY
    queueEnqueue(task->jobQ, &i);
    i = 0; // SKELETON
    queueEnqueue(task->jobQ, &i);
    i = 1; // ENGINE
    queueEnqueue(task->jobQ, &i);
    i = 2; // CHASSIS
    queueEnqueue(task->jobQ, &i);
    i = 7; // CAR
    queueEnqueue(task->jobQ, &i);
    i = 3; // BATTERY
    queueEnqueue(task->jobQ, &i);
    i = 4; // WINDOW
    for (int j = 0; j < 7; j++)
      queueEnqueue(task->jobQ, &i);
    i = 5; // TIRE
    for (int j = 0; j < 4; j++)
      queueEnqueue(task->jobQ, &i);
  }
  /* Prepare task end*/

  /* Production start */
  // Create robot, assign task, and start to work
  for (int i = 0; i < num_typeA; ++i)
  {
    robotsA[i] = createRobot(TypeA);
    robotsA[i]->task = task;
    pthread_create(&robotsA[i]->pthread, NULL, simpleRobotRoutine, robotsA[i]);
  }
  // TODO: create typeB and typeC robots
  for (int i = 0; i < num_typeB; ++i)
  {
    robotsB[i] = createRobot(TypeB);
    robotsB[i]->task = task;
    pthread_create(&robotsB[i]->pthread, NULL, simpleRobotRoutine, robotsB[i]);
  }
  for (int i = 0; i < num_typeC; ++i)
  {
    robotsC[i] = createRobot(TypeC);
    robotsC[i]->task = task;
    pthread_create(&robotsC[i]->pthread, NULL, simpleRobotRoutine, robotsC[i]);
  }
  // wait until work done
  for (int i = 0; i < num_typeA; ++i)
  {
    pthread_join(robotsA[i]->pthread, NULL);
  }
  // TODO: join typeB and typeC robot threads
  for (int i = 0; i < num_typeB; ++i)
  {
    pthread_join(robotsB[i]->pthread, NULL);
  }
  for (int i = 0; i < num_typeC; ++i)
  {
    pthread_join(robotsC[i]->pthread, NULL);
  }
  /* Production end */

  /* Release memory */
  for (int i = 0; i < num_typeA; ++i)
    releaseRobot(robotsA[i]);
  for (int i = 0; i < num_typeB; ++i)
    releaseRobot(robotsB[i]);
  for (int i = 0; i < num_typeC; ++i)
    releaseRobot(robotsC[i]);
  queueDestroy(task->jobQ);
  free(task);
  free(robotsA);
  free(robotsB);
  free(robotsC);
}

int main(int argc, char *argv[])
{
  pthread_mutex_init(&lock, NULL);

  if (argc < 6)
  {
    printf("Usage: %s <number of cars> <number of spaces> <number of typeA robot> <number of typeB robot> <number of typeC robot>\n", argv[0]);
    return EXIT_SUCCESS;
  }
  // Change to your name and university ID
  printf("Name: Benedictus Alvian Sofjan\t UID: xxxxxxxxxx\n");

  /* Initialization */
  num_cars = atoi(argv[1]);
  num_spaces = atoi(argv[2]);
  num_typeA = atoi(argv[3]);
  num_typeB = atoi(argv[4]);
  num_typeC = atoi(argv[5]);
  total_num_robots = num_typeA + num_typeB + num_typeC;
  printf("Production goal: %d, num space: %d, num typeA: %d, num typeB: %d, num typeC: %d\n",
         num_cars, num_spaces, num_typeA, num_typeB, num_typeC);
  initProduction(num_cars, num_spaces);
  initNumRobot(num_typeA, num_typeB, num_typeC);
  /* Initialization end */

  double timer = getCurrentTime();
  startProduction();
  timer = getCurrentTime() - timer;

  printf("====Final Report====\n");
  printProductionProgress();
  printf("Production time: %lf\n", timer);

  pthread_mutex_destroy(&lock);
  return 0;
}
