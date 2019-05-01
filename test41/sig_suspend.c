#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


/* 自定义的处理处理函数 */
void my_err(const char * err_string, int line) {
    fprintf(stderr,"line : %d",__LINE__);
    perror(err_string);
    exit(1);
}

/* SIGINT　的处理函数 */
void handler_sigint(int signo) {
    printf("recv SIGINT\n");
}

int main()
{
    sigset_t    newmask, oldmask, zeromask;     //定义信号集

    /* 安装信号处理函数 */
    if( signal(SIGINT, handler_sigint) == SIG_ERR ) {
        my_err("signal",__LINE__);
    }


    /* 初始化信号集newmask并将SIGINT添加进去 */
    sigemptyset(&newmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask, SIGINT);

    /* 屏蔽信号SIGINT */
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        my_err("sigprocmask",__LINE__);
    } else {
        printf("SIGINT blocked\n");
    }

    /* 临界区 */

    /* 使用sigsuspend取消所有信号的屏蔽并等待信号触发 */
    if(sigsuspend(&zeromask) != -1) {
        my_err("sigsuspend",__LINE__);
    } else {
        printf("recv a signo, return from sigsuspend\n");
    }

    /*------------------------------
     * 如果使用sigprocmask加上pause可能会出现错误
     *  if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
     *    my_err("sigprocmask",__LINE__);
     *  }
     *  pause();
     */


    /* 解除对SIGINT的屏蔽 */
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        my_err("sigprocmask",__LINE__);
    }

    while(1);
    return 0;
}











