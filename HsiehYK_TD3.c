#define MFENCE asm volatile("mfence":::"memory")

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

volatile int R2 = 0;
volatile int X = 0;
volatile int Y = 0;

void *f1(void *vargp)
{
    Y = 1;
    R2 = X;
    printf("[1] X, Y, R2: %d, %d, %d\n", X, Y, R2);
    return NULL;
}

void *f2(void *vargp)
{
    X = 1;
    R2 = Y;
    printf("[2] X, Y, R2: %d, %d, %d\n", X, Y, R2);
    return NULL;
}

void *mutf1(void *vargp)
{
    Y = 1;
    R2 = X;
    printf("[1] X, Y, R2: %d, %d, %d\n", X, Y, R2);
    return NULL;
}

void *mutf2(void *vargp)
{
    X = 1;
    R2 = Y;
    printf("[2] X, Y, R2: %d, %d, %d\n", X, Y, R2);
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, f1, NULL);
    pthread_create(&tid2, NULL, f2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    exit(0);
}
