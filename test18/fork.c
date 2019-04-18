#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>




int main()
{
    pid_t pid;

    printf("Process Creation Study\n");
    pid = fork();

    switch(pid)
    {
        case 0:
            printf("child runing, curpid is %d,parentpid is %d\n",pid,getppid());
            break;
        case -1:
            printf("error\n");
            break;
        default:
            printf("child pid is %d,parent pid is %d\n",pid,getpid());
            break;
    }

    return 0;
}