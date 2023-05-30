#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "ActiveObject.h"
#include "pipeline_st.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isPrime(unsigned int num) {
    if (num < 2 || (num > 2 && num % 2 == 0))
        return 0;

    unsigned int sqrtNum = (unsigned int)sqrt(num);
    for (unsigned int i = 3; i <= sqrtNum; i += 2) {
        if (num % i == 0)
            return 0;
    }

    return 1;
}

typedef struct {
    ActiveObject* nextAO;
    int* numberPtr;
} TaskData;

void generateNumbers(void* arg) {
    TaskData* data = (TaskData*)arg;
    int N = *(data->numberPtr);
    srand(time(NULL));

    TaskData* nextData;  // Move the declaration outside the loop

    for (int i = 0; i < N; i++) {
        int* numberPtr = (int*)malloc(sizeof(int));
        *numberPtr = rand() % 1000000;
        printf("%d\n", *numberPtr);
        usleep(1000); // Delay 1 millisecond

        nextData = (TaskData*)malloc(sizeof(TaskData));  // Assign a new value inside the loop
        nextData->nextAO = data->nextAO;
        nextData->numberPtr = numberPtr;

        enqueueTask(data->nextAO, nextData);
    }

    // Free the allocated memory for the current task data
    free(data);
}
void processNumber(void* arg) {
    TaskData* data = (TaskData*)arg;
    int number = *(data->numberPtr);
    printf("%s\n", isPrime(number) ? "true" : "false");
    number += 11;

    TaskData* nextData = (TaskData*)malloc(sizeof(TaskData));
    nextData->nextAO = data->nextAO;
    nextData->numberPtr = data->numberPtr;

    enqueueTask(data->nextAO, nextData);
}

void subtractAndPrint(void* arg) {
    TaskData* data = (TaskData*)arg;
    int number = *(data->numberPtr);
    printf("%s\n", isPrime(number) ? "true" : "false");
    number -= 13;

    TaskData* nextData = (TaskData*)malloc(sizeof(TaskData));
    nextData->nextAO = data->nextAO;
    nextData->numberPtr = data->numberPtr;

    enqueueTask(data->nextAO, nextData);
}

void addAndPrint(void* arg) {
    TaskData* data = (TaskData*)arg;
    int number = *(data->numberPtr);
    printf("%d\n", number);
    number += 2;
    printf("%d\n", number);

    TaskData* firstData = (TaskData*)getQueue(data->nextAO)->head->data;
    if (number == *(firstData->numberPtr)) {
        printf("Last number matches the first number.\n");
    } else {
        printf("Last number does not match the first number.\n");
    }

    free(firstData->numberPtr);
    free(firstData);
}

void waitForTasks(ActiveObject* ao) {
    pthread_mutex_lock(&ao->mutex);
    while (ao->tasks > 0 && !ao->stop) {
        pthread_cond_wait(&ao->cond, &ao->mutex);
    }
    pthread_cond_broadcast(&ao->cond);  // Wake up all threads
    pthread_mutex_unlock(&ao->mutex);
}

void pipeline_st(int N, unsigned int seed) {
    ActiveObject* AO1 = createActiveObject();
    AO1->func = generateNumbers;

    ActiveObject* AO2 = createActiveObject();
    AO2->func = processNumber;

    ActiveObject* AO3 = createActiveObject();
    AO3->func = subtractAndPrint;

    ActiveObject* AO4 = createActiveObject();
    AO4->func = addAndPrint;

    createActiveThread(AO1);
    createActiveThread(AO2);
    createActiveThread(AO3);
    createActiveThread(AO4);

    int* NPtr = (int*)malloc(sizeof(int));
    *NPtr = N;

    TaskData* data = (TaskData*)malloc(sizeof(TaskData));
    data->nextAO = AO2;
    data->numberPtr = NPtr;

    enqueueTask(AO1, data);

    // Wait for tasks to complete
    while (1) {
        usleep(1000); // Delay 1 millisecond

        if (AO4->tasks == 0) {
            break;  // All tasks completed
        }
    }

    AO1->stop = 1;
    AO2->stop = 1;
    AO3->stop = 1;
    AO4->stop = 1;

    stopActiveObject(AO1);
    stopActiveObject(AO2);
    stopActiveObject(AO3);
    stopActiveObject(AO4);

    destroyActiveObject(AO1);
    destroyActiveObject(AO2);
    destroyActiveObject(AO3);
    destroyActiveObject(AO4);
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Usage: %s N [seed]\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    unsigned int seed = (argc == 3) ? atoi(argv[2]) : 0;
    srand(seed);
    pipeline_st(N, seed);

    return 0;
}
