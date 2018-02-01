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

#include <fcntl.h>
#include <errno.h>

/**File copy source:
   https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c **/

int cp(const char *from, const char *to)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0550);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);
        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;
    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}


int fn(void *arg)
{
    printf("Pid of cloned instance: %d Parent's pid: %d\n", getpid(), getppid());
    
    //Set machine name
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

    //Show uname -a
    printf("uname -a: %s %s %s %s %s %s.\n", unameData.sysname, unameData.nodename, unameData.release, unameData.version, unameData.machine, unameData.domainname);
    struct stat st = {0};
    const char vmRootDir[9] = "vm_root";
    if (stat(vmRootDir, &st) == -1) 
    {
        if(mkdir(vmRootDir, 0700) !=0 )
        {
            perror("Calling mkdir('vm_root')");
            exit(EXIT_FAILURE);
        }
    }

    const char vmRootBinDir[13] = "vm_root/bin";
    //Create /bin and copy utility executables under vm_root/bin
    if (stat(vmRootBinDir, &st) == -1)
    {
         if (mkdir(vmRootBinDir, 0700) !=0)
         {
	     perror("Calling mkdir('vm_root/bin')");
             exit(EXIT_FAILURE);
         }
         cp("/bin/bash", "vm_root/bin/bash");
         cp("/bin/ls", "vm_root/bin/ls");
         cp("/bin/cat", "vm_root/bin/cat");
         cp("/bin/cp", "vm_root/bin/cp");
    }

    //Limit directory under ./vm_root
    if (chroot(vmRootDir) ==-1)
    {
	perror("Calling chroot('vm_root')");
        exit(EXIT_FAILURE);
    }
    if (chdir("/") ==-1)
    {
        perror("Calling chdir('vm_root')");
        exit(EXIT_FAILURE);
    }
    
    //Execute ls
    char *arguments[] = { "ls", NULL };
    if (execv("/bin/ls", arguments) == -1) 
    {
	perror("Calling execv('/bin/ls')");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
