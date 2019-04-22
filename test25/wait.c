#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>



int main()
{
    pid_t pid;
    char *msg;
    int k;
    int exit_code;

    printf("Study how to get exit code\n");

    pid = fork();
    switch(pid) {
        case 0:
            msg = "Child process is runing";
            k =5;
            exit_code = 37;
            break;
        case -1:
            perror("Process creation failed\n");
            exit(1);
        default:
            exit_code =0;
            break;
    }
    /* 父子进程都会执行以下这段代码
     * 子进程pid值为0 父进程pid值为子进程id */
    if(pid != 0) { //父进程等待子进程结束
        int stat_val;
        pid_t child_pid;

        child_pid = wait(&stat_val);

        printf("Child procee has exited, pid = %d\n",child_pid);
        if(WIFEXITED(stat_val))
            printf("Child exited with code %d\n",WEXITSTATUS(stat_val));
        else
            printf("Child exited abnormally\n");
    } else {
        while(k-- > 0) {
            puts(msg);
            sleep(1);
        }
    }

    exit(exit_code);

}
