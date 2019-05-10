#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    int     n;
    char    buf[1024];

    while(1) {
        if( (n = read(STDIN_FILENO, buf, 1024)) > 0) {
            buf[n] = '\0';
            printf("ctrlprocess receive: %s\n",buf);

            if(!strcmp(buf, "exit")) {
                exit(0);
            }
            if(!strcmp(buf, "getpid")) {
                printf("My pid:%d\n",getpid());
                sleep(3);
                exit(0);
            }
        }
    }
}




