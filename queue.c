#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

PQueue queueCreate() {
    PQueue queue = (PQueue)malloc(sizeof(Queue));

    if (queue == NULL)
    {
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    MUTEX_INIT(&queue->lock);
    COND_INIT(&queue->cond);
    return queue;
}

void queueDestroy(PQueue queue) {
    if (queue == NULL)
    {
        return;
    }

    MUTEX_LOCK(&queue->lock);

    PQueueNode node = queue->head;

    while (node != NULL)
    {
        PQueueNode next = node->next;
        free(node->data);
        free(node);
        node = next;
    }

    MUTEX_UNLOCK(&queue->lock);
    COND_DESTROY(&queue->cond);
    MUTEX_DESTROY(&queue->lock);

    free(queue);
}

void queueEnqueue(PQueue queue, void *data) {
    if (queue == NULL)
    {
        return;
    }
    PQueueNode node = (PQueueNode)malloc(sizeof(QueueNode));
    if (node == NULL)
    {
        return;
    }
    node->data = data;
    node->next = NULL;
    MUTEX_LOCK(&queue->lock);
    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
        COND_SIGNAL(&queue->cond);
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    MUTEX_UNLOCK(&queue->lock);
}

void *queueDequeue(PQueue queue) {
    if (queue == NULL)
    {
        return NULL;
    }
    MUTEX_LOCK(&queue->lock);
    while (queue->head == NULL)
        COND_WAIT(&queue->cond, &queue->lock);
    PQueueNode node = queue->head;
    void *data = node->data;
    queue->head = node->next;
    if (queue->head == NULL)
        queue->tail = NULL;
    free(node);
    queue->size--;
    MUTEX_UNLOCK(&queue->lock);
    return data;
}

int queueIsEmpty(PQueue queue) {
    if (queue == NULL)
    {
        return -1;
    }
    MUTEX_LOCK(&queue->lock);
    int isEmpty = (queue->size == 0);
    MUTEX_UNLOCK(&queue->lock);

    return isEmpty;
}

int queueSize(PQueue queue) {
		if (queue == NULL)
		{
			return -1;
		}

		MUTEX_LOCK(&queue->lock);
		int size = queue->size;
		MUTEX_UNLOCK(&queue->lock);

		return size;
	}

	void *queuePeek(PQueue queue) {
		if (queue == NULL)
		{
			return NULL;
		}

		MUTEX_LOCK(&queue->lock);

		if (queue->head == NULL)
		{
			MUTEX_UNLOCK(&queue->lock);
			return NULL;
		}

		void *data = queue->head->data;

		MUTEX_UNLOCK(&queue->lock);

		return data;
	}

	void *queuePeekTail(PQueue queue) {
		if (queue == NULL)
		{
			return NULL;
		}

		MUTEX_LOCK(&queue->lock);

		if (queue->tail == NULL)
		{
			MUTEX_UNLOCK(&queue->lock);
			return NULL;
		}

		void *data = queue->tail->data;

		MUTEX_UNLOCK(&queue->lock);

		return data;
	}

	void queuePrint(PQueue queue) {
		if (queue == NULL)
		{
			return;
		}

		MUTEX_LOCK(&queue->lock);
		if (queue->head == NULL)
		{
			MUTEX_UNLOCK(&queue->lock);
			return;
		}

		PQueueNode node = queue->head;

		while (node != NULL)
		{
			node = node->next;
		}
		MUTEX_UNLOCK(&queue->lock);
	}
