#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * queue introduction
 *
 * queue is an implementation of int queue. The capacity of a queue is
 * pre-determined and won't be expanded after its creation.
 * Note: this queue is none-thread safe!
 *
 * Elements of struct queue is stored in an integer array.
 * The struct queue has 4 unsigned int
 * 	front:    the index of the first element in the array,
 * 	rear:     the index of the last element in the array,
 * 	size:     the number of elements in the queue,
 * 	capacity: the maximum number of elements the queue can store.
 */

typedef struct queue_t {
	unsigned int front, rear, size, capacity;
	int * array;
} queue_t; 
typedef struct queue_t* Queue;

/*
 * queueCreate - create a queue with given capacity
 * @capacity: the capacity of the queue to be created.
 *
 * Once capacity is set, it can't be changed.
 */ 
Queue queueCreate(unsigned capacity);

/*
 * queueDestroy - destroy the queue and release memory
 * @q: the queue to be destroied
 *
 */ 
void queueDestroy(Queue q);

/*
 * queueIsFull - check if the queue is Full
 * @q: the queue to be checked
 *
 * returns 1 if number of elements reaches its capacity,
 * returns 0 otherwise.
 */ 
int queueIsFull(Queue q);

/*
 * queueIsEmpty - check if the queue is empty
 * @q: the queue to be checked
 *
 * returns 1 if there's no element stored in the queue,
 * returns 0 otherwise.
 */ 
int queueIsEmpty(Queue q);

/*
 * queueEnqueue - append an element to the queue
 * @q: the queue to which the element will be appended
 * @value: the pointer to the element
 *
 * returns 0 if succeed
 * returns -1 if queue is full.
 */ 
int queueEnqueue(Queue q, int* value);

/*
 * queueDequeueFront - pop the first element of the queue and put the element in the container
 * @q: the queue to to be dequeued from the front end
 * @container: the container to store the removed element
 *
 * returns 0 if succeed
 * returns -1 if queue is empty.
 */ 
int queueDequeueFront(Queue q, int* container);

/*
 * queueDequeueRear - pop the last element of the queue and put the element in the container
 * @q: the queue to be dequeued from the rear end
 * @container: the container to store the removed element
 *
 * returns 0 if succeed
 * returns -1 otherwise.
 */ 
int queueDequeueRear(Queue q, int* container);

/*
 * queueFront - get a pointer to the first element in the queue, but won't remove it from the queue
 * @q: the queue
 *
 * returns the pointer to the first element
 * returns NULL if queue is empty.
 */ 
int* queueFront(Queue q);

/*
 * queueRear - get a pointer to the last element in the queue, but won't remove it from the queue
 * @q: the queue
 *
 * returns the pointer to the last element
 * returns NULL if queue is empty.
 */ 
int* queueRear(Queue q);

/*
 * queuePrint - print out the queue
 * @q: the queue to be printed
 *
 */ 
void queuePrint(Queue q);
