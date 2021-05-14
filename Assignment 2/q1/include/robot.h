#ifndef ROBOT_H_GYVTBH6X
#define ROBOT_H_GYVTBH6X

#include "queue.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
typedef struct Task_t {
  // Feel free to add more stuff if needed
  Queue jobQ; 
} Task_t; 

// ===== Don't modify anything below this line =====

//Job ID
#define SKELETON  0
#define ENGINE    1
#define CHASSIS   2
#define BATTERY   3
#define WINDOW    4
#define TIRE      5
#define BODY      6
#define CAR       7 

typedef struct Task_t* Task;
/*! \enum RobotType
 *
 *  Defines robot types
 */
enum RobotType_ { 
  TypeA = 0xabcdef, 
  TypeB = 0xbc1234, 
  TypeC = 0xfedcba 
};
typedef enum RobotType_ RobotType;
// Convert robot type to printale char: A, B, or C
char RobotTypeToChar(RobotType rt);

typedef struct Robot_t {
  /** These variables get initialized when calling createRobot **/
  int id;
  RobotType robotType;
  pthread_t pthread;
  /***********************************/
  // Initialize yourself, only keep a reference here
  Task task; 
} Robot_t; 
typedef struct Robot_t* Robot;

/*! \brief Initial number of robots
 *
 *  Set the number of robots for each type 
 *
 * \param numA max number of typeA robot 
 * \param numB max number of typeB robot
 * \param numC max number of typeC robot
 * \return void
 */
void initNumRobot(int numA, int numB, int numC);

/*! \brief Create Robot 
 *
 *  Create robot with given type and number of work to do.
 *
 * \param rt robot type
 * \return Robot on success. return NULL if max number of certain
 * type of robot has been created.
 */
Robot createRobot(RobotType rt);

/*! \brief Releasing robot
 *
 *  Release robot resources initiated in createRobot.
 *  Memory space of robot will also get freed.
 *
 * \param robot robot to be released
 * \return void
 */
void releaseRobot(Robot robot);

/*! \brief Get number of free robot
 *
 *  Get number of free robot
 *
 * \param rt robot type
 * \return number of free robot
 */
int getNumFreeRobot(RobotType rt);

#endif /* end of include guard: ROBOT_H_GYVTBH6X */
