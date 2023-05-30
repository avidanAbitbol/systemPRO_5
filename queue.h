#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* head;
    QueueNode* tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

Queue* createQueue();
void enqueue(Queue* queue, void* item);
void* dequeue(Queue* queue);
void destroyQueue(Queue* queue);
int isEmpty(Queue* q);

#endif /* QUEUE_H */
