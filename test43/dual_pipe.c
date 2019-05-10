#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/* 子程序读写管道 */
void child_wr_pipe(int w_fd, int r_fd)
{
    char* w_msg = "from child!\n";
    write(w_fd,w_msg,strlen(w_msg)+1);
    
    char r_msg[200];
    read(r_fd,r_msg,200);
    printf("child read: %s",r_msg);
}


/* 子程序读写管道 */
void parent_wr_pipe(int w_fd, int r_fd)
{
    char* w_msg = "from parent!\n";
    write(w_fd,w_msg,strlen(w_msg)+1);

    char r_msg[200];
    read(r_fd,r_msg,200);
    printf("child read: %s",r_msg);
}


int main()
{
    int     fd1[2], fd2[2];
    pid_t   pid;
    int     stat_val;
    
    if(pipe(fd1) || pipe(fd2)) {
        perror("pipe error");
        exit(1);
    }
    
    pid = fork();
    switch (pid) {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            close(fd1[0]);
            close(fd2[1]);
            child_wr_pipe(fd1[1], fd2[0]);
            exit(0);
        default:
            close(fd1[1]);
            close(fd2[0]);
            parent_wr_pipe(fd2[1],fd1[0]);
            wait(&stat_val);
            exit(0);
    }

    return 0;
}

