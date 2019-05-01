//示例利用信号传递数据,本程序发送数组
//选项　-d  后跟待传送的数据  -s  后跟待发送的信号 -p　后跟目的进程id
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void handler_sigint(int signo, siginfo_t * siginfo, void* pvoid)
{
    printf("recv SIGINT , the data value is:%s\n",(char*)siginfo->_sifields._timer.si_sigval.sival_ptr);
}



int main(int argc, char* argv[])
{
    union sigval    value;
    struct sigaction act;
    int             signum = SIGTERM; //默认发送SIGTERM信号
    pid_t           pid;
    int             i;
    

    /* 赋值act结构 */
    act.sa_sigaction =  handler_sigint;
    act.sa_flags = SA_SIGINFO;
    /* 安装信号处理函数 */
    sigaction(SIGINT,&act,NULL);
    /* 初始化 */
    value.sival_ptr = NULL;

    /* 检查参数的个数 */
    if(argc != 3 && argc != 5 ) {
        printf("./send_data_signo <-d data> <-s signum> \n");
        exit(1);
    }
    /* 从命令行解析出命令编号,PID,以及等待传递的数据 */
    for(i=1; i<argc; i++) {
        if(!strcmp(argv[i],"-d")) {
            value.sival_ptr = argv[i+1];
            continue;
        }
        if(!strcmp(argv[i],"-s")) {
            signum = atoi(argv[i+1]);
            continue;
        }
 
    }
    /* 利用sigqueue函数给pid发送信号signum,并携带数据value */
    if(sigqueue(getpid(),signum,value) < 0) {
        perror("sigqueue");
        exit(1);
    }

    return 0;
}
