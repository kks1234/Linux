//示例利用信号传递数据,本程序发送数组
//选项　-d  后跟待传送的数据  -s  后跟待发送的信号 -p　后跟目的进程id
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    union sigval    value;
    int             signum = SIGTERM; //默认发送SIGTERM信号
    pid_t           pid;
    int             i;

    /* 初始化 */
    value.sival_int = 0;

    /* 检查参数的个数 */
    if(argc != 3 && argc != 5 && argc != 7) {
        printf("./send_data_signo <-d data> <-s signum> [-p][data]\n");
        exit(1);
    }
    /* 从命令行解析出命令编号,PID,以及等待传递的数据 */
    for(i=1; i<argc; i++) {
        if(!strcmp(argv[i],"-d")) {
            value.sival_int = atoi(argv[i+1]);
            continue;
        }
        if(!strcmp(argv[i],"-s")) {
            signum = atoi(argv[i+1]);
            continue;
        }
        if(!strcmp(argv[i],"-p")) {
            pid = atoi(argv[i+1]);
            continue;
        }
    }
    /* 利用sigqueue函数给pid发送信号signum,并携带数据value */
    if(sigqueue(pid,signum,value) < 0) {
        perror("sigqueue");
        exit(1);
    }

    return 0;
}












