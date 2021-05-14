#ifndef SCHEDULER_H_3PFXQEN0
#define SCHEDULER_H_3PFXQEN0

#include "../include/production.h"
#include "../include/robot.h"
#include "../include/utils.h"

// simpleWork() and simpleRobotRoutine() are simple examples, 
// you can create your own scheduling functions
void simpleWork(int jobID, Robot robot); 
void* simpleRobotRoutine(void* arg);


#endif /* end of include guard: SCHEDULER_H_3PFXQEN0 */
