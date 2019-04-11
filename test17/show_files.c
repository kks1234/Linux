#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>


void my_err(const char * err_str, int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_str);
    exit(1);
}


int my_readdir(const char * path)
{
    DIR * dir;
    struct dirent * ptr;
    int i = 0;
    if((dir = opendir(path)) == NULL) {
        my_err("opendir", __LINE__);
    }
    while(++i,(ptr = readdir(dir)) != NULL) {
        printf("file%d:%s\n",i,ptr->d_name);
    }

    closedir(dir);
    return 0;
}


int main(int argc, char * argv[])
{
    if(argc < 2) {
        my_err("argc", __LINE__);
    }

    my_readdir(argv[1]);

    return 0;


}