#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>



int main()
{
    pid_t pid;
    int stat_val = 0;
    int oldpri,newpri;

    printf("nice study !\n");

    pid = fork();
    switch (pid) {
        case 0:
            printf("Child is runing, Curpid is %d, parentpid is %d\n",getpid(),getppid());

            oldpri = getpriority(PRIO_PROCESS, getpid());
            printf("Old priority = %d\n",oldpri);

            newpri  = nice(2);
            printf("New Priorty is %d\n",newpri);

            exit(0);
        case -1:
            perror("Process creation failed!\n");
            break;
        default:
            printf("parent is runing, Childpid is %d,parentpid is %d\n",pid,getpid());
            break ;
    }
    wait(&stat_val);
    exit(0);
}
