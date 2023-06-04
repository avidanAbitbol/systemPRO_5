#include "Active_Object.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define ACTIVE_OBJECTS_NUM 4

PActiveObject *ActiveObjects_Array = NULL;
PTask task_init = NULL;
extern PActiveObject *ActiveObjects_Array;
int check_prime(unsigned int num) {
    if (num <= 2 || num % 2 == 0)
        return 0;
    unsigned int root = (unsigned int)sqrt(num);
    for (unsigned int i = 3; i <= root; i += 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}
int ActiveObjectTask1(void *task) {
    PTask task_init = (PTask)task;
    unsigned int n = task_init->num_of_tasks, seed = task_init->_data;
    srand(seed != 0 ? seed : time(NULL));
    for (unsigned int i = 0; i < n; i++)
    {
        unsigned int num = (rand() % 900000) + 100000;

        PTask task_data = createTask(n, num);

        if (task_data == NULL)
        {
            exit(1);
        }
        ENQUEUE(getQueue(*(ActiveObjects_Array + 1)), task_data);
        usleep(1000);
    }
    return 0;
}

int ActiveObjectTask2(void *task) {
    static unsigned int count = 0;
    PTask task_data = (PTask)task;

    unsigned int iterations = task_data->num_of_tasks, num = task_data->_data;

    fprintf(stdout, "%u\n%s\n", num, check_prime(num) ? "true" : "false");

    destroyTask(task_data);

    task_data = createTask(iterations, (num + 11));

    if (task_data == NULL)
    {
        exit(1);
    }

    ENQUEUE(getQueue(*(ActiveObjects_Array + 2)), task_data);

    return (iterations <= ++count) ? 0 : 1;
}

int ActiveObjectTask3(void *task) {
    static unsigned int count = 0;
    PTask task_data = (PTask)task;

    unsigned int iterations = task_data->num_of_tasks, num = task_data->_data;

    fprintf(stdout, "%u\n%s\n", num, check_prime(num) ? "true" : "false");

    destroyTask(task_data);

    task_data = createTask(iterations, (num - 13));

    if (task_data == NULL)
    {
        exit(1);
    }

    ENQUEUE(getQueue(*(ActiveObjects_Array + 3)), task_data);

    return (iterations <= ++count) ? 0 : 1;
}

int ActiveObjectTask4(void *task) {
    static unsigned int count = 0;
    PTask task_data = (PTask)task;
    unsigned int iterations = task_data->num_of_tasks, num = task_data->_data;
    fprintf(stdout, "%u\n%s\n%u\n", num, check_prime(num) ? "true" : "false", (num + 2));
    destroyTask(task_data);

    return (iterations <= ++count) ? 0 : 1;
}



int main(int argc, char **args) {
    PQueueFunc Functions_Array[ACTIVE_OBJECTS_NUM] = {
            ActiveObjectTask1,
            ActiveObjectTask2,
            ActiveObjectTask3,
            ActiveObjectTask4
    };

    unsigned int n = 0, seed = 0;
    if (argc == 1) {
        return 1;
    } else if (argc == 2) {
        n = atoi(args[1]);
        if (n == 0)
            return 1;
    } else if (argc == 3) {
        n = atoi(args[1]);
        seed = atoi(args[2]);
        if (n <= 0 || seed <= 0)
            return 1;
    } else {
        return 1;
    }
    ActiveObjects_Array = (PActiveObject *) malloc(sizeof(PActiveObject) * ACTIVE_OBJECTS_NUM);
    task_init = createTask(n, seed);
    if (ActiveObjects_Array == NULL || task_init == NULL) {
        if (ActiveObjects_Array != NULL)
            free(ActiveObjects_Array);
        if (task_init != NULL)
            destroyTask(task_init);
        return 1;
    }

    // Create the Active Objects
    for (int i = 0; i < ACTIVE_OBJECTS_NUM; i++) {
        *(ActiveObjects_Array + i) = CreateActiveObject(Functions_Array[i]);
        if (*(ActiveObjects_Array + i) == NULL) {
            for (int j = 0; j < i; j++)
                stopActiveObject(*(ActiveObjects_Array + j));
            free(task_init);
            free(ActiveObjects_Array);

            return 1;
        }
    }
    ENQUEUE(getQueue(*(ActiveObjects_Array)), task_init);
    for (int i = 0; i < ACTIVE_OBJECTS_NUM; i++) {
        int ret = pthread_join((*(ActiveObjects_Array + i))->thread, NULL);

        if (ret != 0) {
            for (int j = 0; j < ACTIVE_OBJECTS_NUM; j++)
                stopActiveObject(*(ActiveObjects_Array + j));
            destroyTask(task_init);
            free(ActiveObjects_Array);
            return 1;
        }
    }
    for (int i = 0; i < ACTIVE_OBJECTS_NUM; i++)
        stopActiveObject(*(ActiveObjects_Array + i));
    destroyTask(task_init);
    free(ActiveObjects_Array);

    return 0;
}


