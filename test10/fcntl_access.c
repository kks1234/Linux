#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


/* 自定义错误处理函数 */
void my_err(const char *err_string, int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}

int main()
{
    int ret;
    int access_mode;
    int fd;

    if((fd = open("example",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)) ==-1)
    {
        my_err("open",__LINE__);
    }

    /* 设置文件打开方式 */
    if((ret = fcntl(fd,F_SETFL,O_APPEND)) == -1)
    {
        my_err("fcntl",__LINE__);
    }
    /* 获取文件的打开方式 */
    if((ret = fcntl(fd,F_GETFL,0)) == -1)
    {
        my_err("fcntl",__LINE__);
    }

    access_mode = ret&O_ACCMODE;
    if(access_mode == O_RDONLY) {
        printf("example access mode : read_only ");
    } else if(access_mode == O_WRONLY) {
        printf("example access mode : write only ");
    } else if(access_mode == O_RDWR) {
        printf("example access mode : write and read ");
    }
    if(ret & O_APPEND) {
        printf(", append");
    } else if(ret & O_NONBLOCK) {
        printf(", nonblock");
    } else if(ret & O_SYNC) {
        printf(", sync");
    }
    printf("\n");

    return 0;
}




