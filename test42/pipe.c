#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 读管道 */
void read_from_pipe(int fd)
{
    char message[100];
    read(fd,message,100);
    printf("read from pipe : %s\n",message);
}

/* 写管道 */
void write_to_pipe(int fd)
{
    char* message =  "Hello World!\n";
    write(fd,message,strlen(message)+1);
}


int main()
{
    int fd[2];
    pid_t pid;
    int stat_val;

    if(pipe(fd)) {
        perror("pipe error");
        exit(1);
    }

    pid = fork();
    switch(pid) {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            close(fd[1]);
            read_from_pipe(fd[0]);
            exit(0);
        default:
            close(fd[0]);
//	    sleep(3);
            write_to_pipe(fd[1]);
            wait(&stat_val);
            exit(0);
    }
    return 0;
}
