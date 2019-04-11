#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <values.h>


void my_err(const char * err_str, int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_str);
    exit(1);
}

int main(int argc, char * argv[])
{
    char buf[PATH_MAX +1 ];
    if(argc < 2) {
        my_err("argc", __LINE__);
    }
    if(chdir(argv[1]) == -1) {
        my_err("chdir", __LINE__);
    }

    if(getcwd(buf, 512) == NULL) {
        my_err("getcwd", __LINE__);
    }

    printf("%s\n",buf);

    return 0;

}

