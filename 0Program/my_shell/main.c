#include "my_shell.h"





int main(int argc, char *argv[])
{
    int     i;
    int     argcount = 0;
    char    arglist[100][256];
    char    **arg = NULL;
    char    *buf = NULL;

    buf = (char *) malloc(256);
    if(buf == NULL) {
        perror("malloc error");
        exit(-1);
    }

    while(1) {
        /* buf 清零 */
        memset(buf,0,256);
        print_prompt();
        get_input(buf);
        /*　若输入的命令为 exit 或　logout　退出本程序　*/
        if(strcmp(buf,"exit\n") == 0 || strcmp(buf, "logout\n") == 0) {
            break;
        }
        for(i=0; i<100; i++) {
            arglist[i][0] = '\0';
        }
        argcount = 0;
        explain_input(buf,&argcount,arglist);
        do_cmd(argcount,arglist);
    }

    if(buf != NULL) {
        free(buf);
        buf = NULL;
    }
    exit(0);

}

