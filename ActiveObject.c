#include <pthread.h>
#include <stdlib.h>
#include "queue.h"
#include "ActiveObject.h"


int isEmpty(Queue* q) {
    if (q->head == NULL) {
        return 1;
    }
    return 0;
}
void* activeThread(void* arg) {
    ActiveObject* active = (ActiveObject*)arg;
    Queue* queue = active->queue;
    void (*func)(void*) = active->func;

    void* task;
    while ((task = dequeue(queue))) {
        if (func != NULL) {
            func(task);
        }
    }

    return NULL;
}

void enqueueTask(ActiveObject* ao, void* arg) {
    pthread_mutex_lock(&ao->mutex);

    if (ao->stop) {
        pthread_mutex_unlock(&ao->mutex);
        return;
    }

    enqueue(ao->queue, arg);
    ao->tasks++;
    pthread_cond_signal(&ao->cond);
    pthread_mutex_unlock(&ao->mutex);
}

void* threadFunc(void* arg) {
    ActiveObject* active = (ActiveObject*)arg;
    Queue* queue = active->queue;
    void (*func)(void*) = active->func;

    void* task;
    while (1) {
        pthread_mutex_lock(&active->mutex);
        while (isEmpty(queue) && !active->stopInProgress) {
            pthread_cond_wait(&active->cond, &active->mutex);
        }
        if (active->stopInProgress) {
            pthread_mutex_unlock(&active->mutex);
            break;  // Exit the loop and terminate the thread
        }
        task = dequeue(queue);
        pthread_cond_signal(&active->cond);
        pthread_mutex_unlock(&active->mutex);
        if (task == NULL) {
            break;  // Exit the loop and terminate the thread
        }
        active->tasks--;
        func(task);
    }

    return NULL;
}

ActiveObject* createActiveObject() {
    ActiveObject* active = (ActiveObject*)malloc(sizeof(ActiveObject));
    active->queue = createQueue();
    active->func = NULL;
    active->stop = 0;
    return active;
}

void createActiveThread(ActiveObject* active) {
    pthread_create(&active->thread, NULL, threadFunc, active);
}

Queue* getQueue(ActiveObject* active) {
    return active->queue;
}

void stopActiveObject(ActiveObject* active) {
    pthread_mutex_lock(&active->mutex);
    active->stopInProgress = 1;  // Set the flag to indicate stop is in progress
    enqueue(active->queue, NULL);
    pthread_cond_signal(&active->cond);
    pthread_mutex_unlock(&active->mutex);
}

void destroyActiveObject(ActiveObject* active) {
    destroyQueue(active->queue);
    free(active);
}
