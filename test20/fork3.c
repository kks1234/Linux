#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>



int main()
{
    pid_t pid;

    printf("Process Creation Study\n");
    pid = fork();

    switch(pid)
    {
        case 0:
            while(1) {
		printf("A background process, PID:%d, ParentID:%d\n",getpid(),getppid());
		sleep(3);
            }
        case -1:
            printf("error\n");
            break;
        default:
            printf("Im parents, my pid %d\n",getpid());
	    exit(0);
            break;
    }

    return 0;
}
