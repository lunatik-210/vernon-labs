
#include "queue.h"

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

QCarElement qPop(Queue* queue)
{
    QNode *oldNodeP = queue->front;
    QCarElement element = oldNodeP->element;

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

int qIsEmpty(Queue* queue)
{
    if(NULL == queue->front)
    {
        return TRUE;
    }

    return FALSE;
}

int qCount(Queue* queue)
{
    if(TRUE == qIsEmpty(queue))
    {
        return 0;
    }

    return queue->count;
}

void qPrint(Queue* queue)
{
	QNode *node = NULL;

    if (TRUE == qIsEmpty(queue))
    {
        return;
    }

    node = queue->front;

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
