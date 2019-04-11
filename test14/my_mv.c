#include <stdio.h>
#include <stdlib.h>


void my_err(const char * err_str, int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_str);
    exit(1);
}


int main(int argc, char * argv[])
{
    if(argc < 3) {
        my_err("argc", __LINE__);
    }

    if(rename(argv[1], argv[2]) == -1) {
        my_err("rename", __LINE__);
    }

    return 0;
}