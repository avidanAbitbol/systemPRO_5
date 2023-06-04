
#include <stdlib.h>
#include <pthread.h>
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL)
#define MUTEX_LOCK(mutex) pthread_mutex_lock(mutex)
#define MUTEX_UNLOCK(mutex) pthread_mutex_unlock(mutex)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex)
#define COND_INIT(cond) pthread_cond_init(cond, NULL)
#define COND_WAIT(cond, mutex) pthread_cond_wait(cond, mutex)
#define COND_SIGNAL(cond) pthread_cond_signal(cond)
#define COND_DESTROY(cond) pthread_cond_destroy(cond)
#define ENQUEUE(queue, data) queueEnqueue(queue, (void *)data)
#define DEQUEUE(queue, type) ((type)queueDequeue(queue))

typedef struct _QueueNode {
    void *data;
    struct _QueueNode *next;
} QueueNode, *PQueueNode;

typedef struct _Queue {
    PQueueNode head;
    PQueueNode tail;
    unsigned int size;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Queue, *PQueue;
PQueue queueCreate();

void queueDestroy(PQueue queue);
void queueEnqueue(PQueue queue, void *data);
void *queueDequeue(PQueue queue);
int queueIsEmpty(PQueue queue);
int queueSize(PQueue queue);

