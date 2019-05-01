//示例利用信号传递数据,本程序接收数组
#include <stdio.h>
#include <signal.h>


void handler_sigint(int signo, siginfo_t * siginfo, void* pvoid)
{
    printf("recv SIGINT , the data value is:%d\n",siginfo->_sifields._timer.si_sigval.sival_int);
}


int main()
{
    struct sigaction act;

    /* 赋值act结构 */
    act.sa_sigaction =  handler_sigint;
    act.sa_flags = SA_SIGINFO;
    /* 安装信号处理函数 */
    sigaction(SIGINT,&act,NULL);

    while (1);
    return 0;
}












