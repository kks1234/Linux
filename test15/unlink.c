#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>




void my_err(const char * err_str, int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_str);
    exit(1);
}


int main()
{
    int fd;
    char buf[32];

    if((fd = open("text", O_RDWR|O_TRUNC|O_CREAT,S_IRWXU)) == -1) {
        my_err("open",__LINE__);
    }

    if(unlink("text") == -1) {
        my_err("unlink", __LINE__);
    }
    printf("file unlinked\n");

    if(write(fd,"temp",5) == -1) {
        my_err("write", __LINE__);
    }
    if(lseek(fd, 0, SEEK_SET) == -1) {
        my_err("lseek", __LINE__);
    }
    if(read(fd, buf, 5) != 5) {
        my_err("read", __LINE__);
    }

    printf("%s\n",buf);

    return 0;
}


