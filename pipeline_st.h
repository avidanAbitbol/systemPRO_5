#ifndef PIPELINE_ST_H
#define PIPELINE_ST_H
#include "ActiveObject.h"
#include "queue.h"
void pipeline_st(int N, unsigned int seed);
int isPrime(unsigned int num);
void generateNumbers(void* arg);
void processNumber(void* arg);
void subtractAndPrint(void* arg);
void addAndPrint(void* arg);
void waitForTasks(ActiveObject* ao);
#endif /* PIPELINE_ST_H */
