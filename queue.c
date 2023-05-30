#include <pthread.h>
#include <stdlib.h>
#include "queue.h"




Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
    return queue;
}

void enqueue(Queue* queue, void* item) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = item;
    newNode->next = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (queue->tail == NULL) {
        queue->head = queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

void* dequeue(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == NULL) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    QueueNode* temp = queue->head;
    void* item = temp->data;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(temp);
    pthread_mutex_unlock(&queue->mutex);

    return item;
}

void destroyQueue(Queue* queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue);
}
