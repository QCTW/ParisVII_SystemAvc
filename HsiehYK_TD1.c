/**
 * HSIEH Yung-Kun (quincy.tw@gmail.com)
 **/
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int fn(void *arg)
{
    printf("Hello! From the cloned instance. Pid=%d\n", getpid());
    return 0;
}


int main( int argc, const char* argv[] )
{
    void *child_stack = malloc(1024 * 1024);
    if ( child_stack == NULL ) 
    {
        printf("ERROR: Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    int cloned_id = clone(fn, child_stack+(1024*1024), CLONE_NEWUTS|CLONE_NEWIPC|CLONE_NEWUSER|CLONE_NEWPID|CLONE_NEWNET, argv);
    if ( cloned_id < 0 )
    {
        printf("ERROR: Unable to create the child process.\n");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
    free(child_stack);
    printf("INFO: Child process terminated.\n"); 
}
