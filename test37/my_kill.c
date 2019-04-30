#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>



int main(int argc, char* argv[])
{
    int     i,j;
    int     signum = SIGTERM; //默认发送SIGTERM
    pid_t   pid;

    if(argc!=2 && argc!=4) {
        printf("Usage: ./my_kill <-s signum> [PID]\n");
        exit(EXIT_SUCCESS);
    }

    /* 从命令行参数中解析出信号编号 */
    for(i=1; i<argc; i++) {
        if(!strcmp(argv[i],"-s")) {
            signum = atoi(argv[i+1]);
            break;
        }
    }

    /* 解析出进程号 */
    if(argc == 2) {
        pid = atoi(argv[1]);
    } else {
        for(j=1; j<argc; j++) {
            if(j!=i && j!=i+1) {
                pid = atoi(argv[j]);
                break;
            }
        }
    }

    if(kill(pid, signum) < 0) {
        perror("kill");
        exit(EXIT_FAILURE);
    }
    return 0;
}
