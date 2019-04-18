#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>




int main(int argc, char * argv[], char ** environ)
{
    pid_t pid;
    int stat_val;
    printf("Exec example!\n");
    pid = fork();

    switch(pid) {
        case -1:
            perror("Process Creation failed\n");
            exit(1);
        case 0:
            printf("Child process is runing\n");
            printf("My pid = %d, parentpid = %d\n",getpid(),getppid());
            printf("uid = %d, gid = %d\n",getuid(),getgid());
            execve("processimage",argv,environ);
            printf("process never go to here!\n");
            exit(0);
        default:
            printf("Parent process is runing\n");
            break;
    }

    wait(&stat_val);
    return 0;
}
