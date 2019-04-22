#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>


extern int errno;

int main()
{
    int fd;

    printf("uid study\n");
    printf("Process's uid = %d,euid = %d\n",getuid(),geteuid());

    //strerror函数获取制定错误码的提示信息
    if((fd = open("text1",O_RDWR)) == -1) {
        printf("Open failure, errno is %d : %s\n",errno,strerror(errno));
        exit(1);
    } else {
        printf("Open successfully!\n");
    }
    
    close(fd);
    return 0;
}
