/********************************************************
* ADS HW Assignment 2
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

#include "queue.h"

// Queue factory - allocates memory for Queue structure
Queue* qCreate()
{
    Queue *queue;

    queue = (Queue*)malloc(sizeof(Queue));

    if (NULL == queue) {
        fprintf(stderr, "Insufficient memory for new queue.\n");
        exit(1);
    }

    queue->front = queue->rear = NULL;
    queue->count = 0;

    return queue;
}

// Remove all the nodes from queue and frees the memory
void qDestroy(Queue* queue)
{
    /*
    * First remove each element from the queue (each
    * element is in a dynamically-allocated node.)
    */
    while (FALSE == qIsEmpty(queue))
    {
        qPop(queue);
    }

    /*
    * Reset the front and rear just in case someone
    * tries to use them after the Queue is freed.
    */
    queue->front = queue->rear = NULL;

    /*
    * Now free the structure that holds information
    * about the queue.
    */
    free(queue);
}

// Pushes element into queue rear
void qPush(Queue* queue, QCarElement element)
{
    QNode *newNodeP;

    /* Allocate space for a node in the linked list. */

    newNodeP = (QNode*)malloc(sizeof(QNode));

    if (NULL == newNodeP) {
        fprintf(stderr, "Insufficient memory for new queue element.\n");
        exit(1);  /* Exit program, returning error code. */
    }

    /* Place information in the node */

    newNodeP->element = element;
    newNodeP->next = NULL;

    /*
    * Link the element into the right place in
    * the linked list.
    */

    if (TRUE == qIsEmpty(queue)) 
    {
        queue->front = queue->rear = newNodeP;
    } else {
        queue->rear->next = newNodeP;
        queue->rear = newNodeP;
    }

    queue->count += 1;
}

// Pops an element from queue front and frees node memory
QCarElement qPop(Queue* queue)
{
    QNode *oldNodeP = queue->front;
    QCarElement element = oldNodeP->element;

    /*
    * If queue is not empty then take an
    * element from the front, decrease counter
    * and frees the node memory
    */

    if(0 == qCount(queue))
    {
        fprintf(stderr, "No elements in queue. Abort.\n");
        exit(1);  /* Exit program, returning error code. */
    }

    queue->front = queue->front->next;
    queue->count -= 1;
    
    free(oldNodeP);

    return element;
}

// Checks if queue is empty
int qIsEmpty(Queue* queue)
{
    if(NULL == queue->front)
    {
        return TRUE;
    }

    return FALSE;
}

// Count the amount of nodes remained in the queue
int qCount(Queue* queue)
{
    if(TRUE == qIsEmpty(queue))
    {
        return 0;
    }

    return queue->count;
}

// Prints the queue into stdout
void qPrint(Queue* queue)
{
	QNode *node = NULL;

    /*
    * If tje queue is not empty take front node
    */

    if (TRUE == qIsEmpty(queue))
    {
        return;
    }

    node = queue->front;

    /*
    * Prints all the node remained in queue 
    * one by one
    */

    do
    {
        printf("%d ", node->element.enter_time);
        node = node->next;
    }
    while(NULL != node);

    printf("\n");
}

// queue implementation testing

/*
int main(int argc, char** argv)
{  
    Queue* queue = qCreate();

    QCarElement e1 = { 1 };
    QCarElement e2 = { 2 };
    QCarElement e3 = { 3 };

    QCarElement e;

    qPush(queue, e1);
    qPrint(queue);

    qPush(queue, e2);
    qPrint(queue);

    qPush(queue, e3);
    qPrint(queue);

    e = qPop(queue);
    printf("Popped: %d\n", e.enter_time);
    qPrint(queue);

    e = qPop(queue);
    printf("Popped: %d\n", e.enter_time);
    qPrint(queue);

    e = qPop(queue);
    printf("Popped: %d\n", e.enter_time);
    qPrint(queue);

    qDestroy(queue);
    return 0;
}
*/
