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
#include <sys/utsname.h>
#include <sys/stat.h>

int fn(void *arg)
{
    printf("Pid of cloned instance: %d Parent's pid: %d\n", getpid(), getppid());
    const char newHostName[9] = "cloned-vm";
    size_t n = sizeof(newHostName);
    if(sethostname("cloned-vm", n) != 0)
    {
        perror("Calling sethostname('cloned-vm')");
        exit(EXIT_FAILURE);
    }
    struct utsname unameData;
    if (uname(&unameData) != 0)
    {
        perror("Calling utsname");
        exit(EXIT_FAILURE);
    }
    printf("uname -a: %s %s %s %s %s %s.\n", unameData.sysname, unameData.nodename, unameData.release, unameData.version, unameData.machine, unameData.domainname);
    struct stat st = {0};
    const char vmRootDir[9] = "./vm_root";
    if (stat(vmRootDir, &st) == -1) 
    {
        if(mkdir(vmRootDir, 0700) !=0 )
        {
            perror("Calling mkdir('./vm_root')");
            exit(EXIT_FAILURE);
        }
    }
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
