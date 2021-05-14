#include "../include/scheduler.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

void *simpleRobotRoutine(void *arg)
{

  Robot robot = (Robot)arg;
  Task task = robot->task;
  lock = task->lock; // Obtain Mutex lock.
#ifdef DEBUG
  debug_printf(__func__, "Robot%c[%d] starts...\n",
               RobotTypeToChar(robot->robotType), robot->id);
#endif
  int jobID;
  while (!queueIsEmpty(task->jobQ))
  {
    pthread_mutex_lock(&lock); // Lock Mutex.
    queueDequeueFront(task->jobQ, &jobID);
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d]: working on job %d...\n",
                 RobotTypeToChar(robot->robotType), robot->id, jobID);
#endif
    simpleWork(jobID, robot);
  }
  pthread_exit(NULL);
}

void simpleWork(int jobID, Robot robot)
{
  double timer = getCurrentTime();
  pthread_mutex_unlock(&lock); // Unlock Mutex.
  switch (jobID)
  {

  case SKELETON:
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d] making skeleton...\n",
                 RobotTypeToChar(robot->robotType), robot->id);
    fflush(stdout);
#endif
    makeSkeleton(robot);
    break;
  case ENGINE:
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d] making engine...\n",
                 RobotTypeToChar(robot->robotType), robot->id);
    fflush(stdout);
#endif
    makeEngine(robot);
    break;
  case CHASSIS:
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d] making chassis...\n",
                 RobotTypeToChar(robot->robotType), robot->id);
    fflush(stdout);
#endif
    makeChassis(robot);
    break;
  case BODY:
    // #ifdef DEBUG
    //     debug_printf(__func__, "Robot%c[%d] making body...\n",
    //                  RobotTypeToChar(robot->robotType), robot->id);
    //     fflush(stdout);
    // #endif
    makeBody(robot);
    break;
  case WINDOW:
    makeWindow(robot);
    break;
  case TIRE:
    makeTire(robot);
    break;
  case BATTERY:
    makeBattery(robot);
    break;
  case CAR:
    // #ifdef DEBUG
    //     debug_printf(__func__, "Robot%c[%d] making car...\n",
    //                  RobotTypeToChar(robot->robotType), robot->id);
    //     fflush(stdout);
    // #endif
    makeCar(robot);
    break;
  default:
    err_printf(__func__, __LINE__, "Error!! Robot%c[%d] gets invalid jobID %d\n",
               RobotTypeToChar(robot->robotType), robot->id, jobID);
    break;
  }
  timer = getCurrentTime() - timer;
#ifdef DEBUG
  debug_printf(__func__, "Robot%c[%d] job %d done! Time: %lf\n",
               RobotTypeToChar(robot->robotType), robot->id, jobID, timer);
#endif
}
