#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


int val1 = 0;//,val2 = 0;
int main()
{
    pid_t pid;
    int i = 0;
    int val2 = 0; //局部变量有问题
    printf("Process Creation Study\n");
    pid = vfork();
   // pid = fork();

    switch(pid)
    {
        case 0:
            i = 3;
            while(i--) {
                printf("child runing\n");
                val1 ++ ; val2 ++ ;
                sleep(1);
            }
            printf("val1 :%d, val2 : %d\n",val1,val2);
            break;
        case -1:
            perror("error\n");
            break;
        default:
            i = 5;
            while(i--) {
                printf("parent runing\n");
                val1 ++ ; val2 ++ ;
                sleep(1);
            }
            printf("val1 :%d, val2 : %d\n",val1,val2);
            break;
    }

    return 0;
}
