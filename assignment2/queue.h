/********************************************************
* ADS HW Assignment 2
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE  1

typedef struct QCarElement QCarElement;
typedef struct QNode QNode;
typedef struct Queue Queue;

struct QCarElement
{
    int enter_time;
};

struct QNode 
{
    QCarElement element;
    QNode *next;
};

struct Queue 
{
    int count;
    QNode *front, *rear;
};

// Queue factory - allocates memory for Queue structure
Queue* qCreate();

// Remove all the nodes from queue and frees the memory
void qDestroy(Queue* queue);

// Pushes element into queue rear
void qPush(Queue* queue, QCarElement element);

// Pops an element from queue front and frees node memory
QCarElement qPop(Queue* queue);

// Checks if queue is empty
int qIsEmpty(Queue* queue);

// Count the amount of nodes remained in the queue
int qCount(Queue* queue);

// Prints the queue into stdout
void qPrint(Queue* queue);

#endif // QUEUE_H
