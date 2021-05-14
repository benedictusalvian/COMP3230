#ifndef PRODUCTION_H_M0DMQT7R
#define PRODUCTION_H_M0DMQT7R

/*============Don't modify this file==============*/

#include <unistd.h>
#include "robot.h"

/*! \brief Initialize the production settings
 *
 *  Initialize the production settings. This function should only
 *  be called once. Error will occur otherwise.
 *
 * \param numCars number of cars to make
 * \param numStorage number of storage space for production
 * \return void
 */
void initProduction(int numCars, int numStorage);

/*! \brief Query the production progress
 *
 *  Query the production progress of certain item
 *
 * \return number of queried items
 */
int getNumFreeSpace();
int getNumProducedSkeleton();
int getNumProducedEngine();
int getNumProducedChassis();
int getNumProducedBody();
int getNumProducedWindow();
int getNumProducedTire();
int getNumProducedBattery();
int getNumProducedCar();

/*! \brief Print the production progress
 *
 *  Print number of free spaces and each produced item
 *
 * \return void
 */
void printProductionProgress();

/*! \brief Make an item
 *
 *  Make an item with a given robot of certain type.
 *  Each make function(except makeCare) will always wait for a 
 *  storage space first.
 *
 *  When making a body or a car, there's no particular order of 
 *  acquiring dependent parts.
 *
 * \param robot robot to make the item
 * \return void
 */
void makeSkeleton(Robot robot);
void makeEngine(Robot robot);
void makeChassis(Robot robot);
void makeBody(Robot robot);
void makeWindow(Robot robot);
void makeTire(Robot robot);
void makeBattery(Robot robot);
void makeCar(Robot robot); // makeCar doesn't require storage space

/*! \brief Try to make an item
 *
 *  Try to make an item with a given robot of certain type.
 *
 *  Each try make function will try to acquire a storage space first.
 *  If there's no free space, these functions will return a non zero value.
 *
 *  Once a space acquired, tryMakeBody will keep waiting until all 
 *  dependent parts are acquired 
 *
 * \param robot robot to make the item
 * \return 0 on success, errors on failing to acquire storage space
 */
int tryMakeSkeleton(Robot robot);
int tryMakeEngine(Robot robot);
int tryMakeChassis(Robot robot);
int tryMakeBody(Robot robot);
int tryMakeWindow(Robot robot);
int tryMakeTire(Robot robot);
int tryMakeBattery(Robot robot);

/*! \brief Try to make an item given waiting time limit
 *
 *  Try to make an item with waiting time and a robot of certain type.
 *
 *  Each try make function will try to acquire a storage space for 
 *  given time limit. If there's no free space when timeout, 
 *  these functions will return a non zero value.
 *
 *  Once space acquired, timedTryMakeBody will keep waiting until all
 *  dependent parts are acquired
 *
 * \param robot robot to make the item
 * \return 0 on success, errors on failing to acquire storage space
 */
int timedTryMakeSkeleton(int waitTime, Robot robot);
int timedTryMakeEngine(int waitTime, Robot robot);
int timedTryMakeChassis(int waitTime, Robot robot);
int timedTryMakeBody(int waitTime, Robot robot);
int timedTryMakeWindow(int waitTime, Robot robot);
int timedTryMakeTire(int waitTime, Robot robot);
int timedTryMakeBattery(int waitTime, Robot robot);

#endif /* end of include guard: PRODUCTION_H_M0DMQT7R */
