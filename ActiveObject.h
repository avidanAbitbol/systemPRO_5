#include "queue.h"

typedef struct _Task {
    unsigned int num_of_tasks;
    unsigned int _data;
} Task, *PTask;

PTask createTask(unsigned int num_of_tasks, unsigned int _data);
void destroyTask(PTask task);
typedef int (*PQueueFunc)(void *);
typedef struct _ActiveObject {
    pthread_t thread;
    PQueue queue;
    PQueueFunc func;
} ActiveObject, *PActiveObject;
PActiveObject CreateActiveObject(PQueueFunc func);
PQueue getQueue(PActiveObject activeObject);
void stopActiveObject(PActiveObject activeObject);
void *activeObjectRunFunction(void *activeObject);

