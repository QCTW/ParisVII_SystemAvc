#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()
int R2 = 0;
int X = 0;
int Y = 0;

void *f1(void *vargp)
{
    Y = 1;
    printf("[1]Before X, Y, R2=%d, %d, %d\n", X, Y, R2);
    R2 = X;
    printf("[1]After  X, Y, R2=%d, %d, %d\n", X, Y, R2);
    return NULL;
}

void *f2(void *vargp)
{
    X = 1;
    printf("[2]Before X, Y, R2=%d, %d, %d\n", X, Y, R2);
    R2 = Y;
    printf("[2]After  X, Y, R2=%d, %d, %d\n", X, Y, R2);
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, f1, NULL);
    pthread_join(tid1, NULL);
    pthread_create(&tid2, NULL, f2, NULL);
    pthread_join(tid2, NULL);
    exit(0);
}
