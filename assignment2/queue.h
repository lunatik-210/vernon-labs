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

Queue* qCreate();

void qDestroy(Queue* queue);

void qPush(Queue* queue, QCarElement element);

QCarElement qPop(Queue* queue);

int qIsEmpty(Queue* queue);

int qCount(Queue* queue);

void qPrint(Queue* queue);

#endif // QUEUE_H
