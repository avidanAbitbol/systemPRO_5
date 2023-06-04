#include "Active_Object.h"
#include <stdlib.h>
#include <pthread.h>
#define ACTIVE_OBJECTS_NUM 4
PActiveObject CreateActiveObject(PQueueFunc func) {
    PActiveObject activeObject = (PActiveObject)malloc(sizeof(ActiveObject));
    if (activeObject == NULL)
    {
        return NULL;
    }
    activeObject->queue = queueCreate();
    if (activeObject->queue == NULL)
    {
        free(activeObject);
        return NULL;
    }
    if (func == NULL)
    {
        queueDestroy(activeObject->queue);
        free(activeObject);
        return NULL;
    }
    activeObject->func = func;
    int ret = pthread_create(&activeObject->thread, NULL, activeObjectRunFunction, activeObject);
    if (ret != 0)
    {
        queueDestroy(activeObject->queue);
        free(activeObject);
        return NULL;
    }
    return activeObject;
}

PQueue getQueue(PActiveObject activeObject) {
    if (activeObject == NULL)
    {
        return NULL;
    }
    return activeObject->queue;
}

void stopActiveObject(PActiveObject activeObject) {
    if (activeObject == NULL)
    {
        return;
    }
    pthread_cancel(activeObject->thread);
    activeObject->func = NULL;
    pthread_join(activeObject->thread, NULL);

    queueDestroy(activeObject->queue);
    free(activeObject);
}

void *activeObjectRunFunction(void *activeObject) {
    if (activeObject == NULL)
    {
        return NULL;
    }
    PActiveObject ao = (PActiveObject)activeObject;
    PQueue queue = ao->queue;
    void *task = NULL;
    if (queue == NULL)
    {
        return NULL;
    }
    while (ao->func && ((task = DEQUEUE(queue, void *))))
    {
        int ret = ao->func(task);
        if (ret == 0)
        {
            break;
        }
    }
    return activeObject;
}

PTask createTask(unsigned int num_of_tasks, unsigned int _data) {
    PTask task = (PTask)malloc(sizeof(Task));
    if (task == NULL)
    {
        return NULL;
    }
    task->num_of_tasks = num_of_tasks;
    task->_data = _data;
    return task;
}
void destroyTask(PTask task) {
    if (task == NULL)
    {
        return;
    }
    free(task);
}

