#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>




int main()
{
    pid_t pid;
    char *msg;
    int i;
    printf("Process Creation Study\n");
    pid = fork();

    switch(pid)
    {
        case 0:
            msg = "child";
            i = 3;
            //printf("child runing, curpid is %d,parentpid is %d\n",pid,getppid());
            break;
        case -1:
            printf("error\n");
            break;
        default:
            msg = "parent";
            i = 5;
            //printf("child pid is %d,parent pid is %d\n",pid,getpid());
            break;
    }

    while(i > 0)
    {
        puts(msg);

        sleep(1);
	i -- ;
    }

    return 0;
}
