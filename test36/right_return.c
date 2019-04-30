#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

#define ENV_UNSAVE      0
#define ENV_SAVE        1

int     flag_saveenv =  ENV_UNSAVE;
sigjmp_buf env;    //跳转位置信息

void handler_sigrtmin15(int signo)
{
    if(flag_saveenv == ENV_UNSAVE) {
        return;
    }
    printf("recv SIGRTMIN+15\n");
    sleep(10);
    printf("in handler_sigrtmin15,after sleep\n");
    siglongjmp(env,1); //返回env处
}



int main()
{
    /* 设置返回点 */
    switch(sigsetjmp(env,1)) {
        case 0:
            printf("return 0\n");
            flag_saveenv = ENV_SAVE;
            break;
        case 1:
            printf("return from SIGMIN+15\n");
            break;
        case 2:
            printf("return from SIGMAX-15\n");
            break;
        default:
            break;
    }

    /* 安装信号 */
    signal(SIGRTMIN+15, handler_sigrtmin15);


    while(1);
    return 0;
}
