#ifndef ACTIVE_H
#define ACTIVE_H

#include "queue.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Queue* queue;
    void (*func)(void*);
    int stopInProgress;
    int tasks;
    bool stop;  // Declare this field
} ActiveObject;

ActiveObject* createActiveObject();
void createActiveThread(ActiveObject* active);
void enqueueTask(ActiveObject* active, void* task);
Queue* getQueue(ActiveObject* active);
void stopActiveObject(ActiveObject* active);
void destroyActiveObject(ActiveObject* active);
void* threadFunc(void* arg);
void enqueueTask(ActiveObject* ao, void* arg);
#endif /* ACTIVE_H */
