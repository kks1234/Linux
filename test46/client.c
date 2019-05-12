#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#define FIFO_READ   "writefifo"
#define FIFO_WRITE  "readfifo"
#define BUF_SIZE    1024


int main()
{
    int     wfd, rfd;
    char    buf[BUF_SIZE];
    int     len;

    umask(0);
    if(mkfifo(FIFO_WRITE, S_IFIFO | 0666) == -1) {
        printf("Can't creat FIFO %s because %s",FIFO_WRITE, strerror(errno));
        exit(1);
    }


    while( (rfd = open(FIFO_READ, O_RDONLY)) == -1) {

        sleep(1);
    }

    if((wfd = open(FIFO_WRITE, O_WRONLY)) == -1) {
        printf("Open FIFO %s error : %s",FIFO_WRITE, strerror(errno));
        exit(1);
    }

 
    while(1) {printf("test1\n");
  	len = read(rfd, buf, BUF_SIZE);
printf("test2\n");
        if(len > 0) {
            buf[len] = '\0';
            printf("Server: %s\n",buf);
	}
        printf("Client: ");
        fgets(buf, BUF_SIZE, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(!strncmp(buf, "quit",4)) {
            close(wfd);
            unlink(FIFO_WRITE);
            close(rfd);
            exit(0);
        }
        write(wfd, buf, strlen(buf) );
    }
}


