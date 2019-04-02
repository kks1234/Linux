#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    int fd;

    if((fd = open("text.txt",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR)) == -1)
   // if((fd = creat("text.txt",S_IRWXU)) == -1)
    {
        //perror("open");
        printf("open: %s  with errno: %d\n",strerror(errno),errno);
        exit(1);
    }
    else
    {
        printf("creat file success\n");
    }

    close(fd);
    return 0;
}