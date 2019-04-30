#include <stdio.h>
#include <setjmp.h>
#include <signal.h>


jmp_buf env;    //跳转位置信息

void handler_sigrtmin15(int signo)
{
    printf("recv SIGRTMIN+15\n");
    longjmp(env,1); //返回env处
}

void handler_sigrtmax10(int signo)
{
    printf("recv SIGRTMAX-10\n");
    longjmp(env,2); //返回env处
}


int main()
{
    /* 设置返回点 */
    switch(setjmp(env)) {
        case 0:
            break;
        case 1:
            printf("return from SIGRTMIN+15\n");
            break;
        case 2:
            printf("return from SIGRTMAX-10\n");
            break;
        default:
            break;
    }

    /* 安装信号 */
    signal(SIGRTMIN+15, handler_sigrtmin15);
    signal(SIGRTMAX-10, handler_sigrtmax10);

    while(1);
    return 0;
}
