//
// Created by yang on 19-4-23.
//
#include "my_shell.h"



/* 打印提示符 */
void print_prompt(void)
{
    printf("mysheel$$ ");
}


/* 得到输入的命令 */
void get_input(char * buf)
{
    int len = 0;
    int ch;

    ch = getchar();
    while(len < 256 && ch != '\n') {
        buf[len++] = ch;
        ch = getchar();
    }

    if(len == 256) {
        printf("it's too long command \n");
        exit(-1);  //输入命令过长则退出程序
    }
    buf[len] = '\n';
    len ++;
    buf[len] = '\0';
}

/* 对输入buf中的命令进行解析,将结果放入arglist中,命令以回车符号\n结束*/
/* 例如 ls -l /tmp   则 arglist[0] = ls arglist[1] = -l arglist[2] = /tmp */
void explain_input(char *buf, int *argcount, char arglist[100][256])
{
    char    *p = buf;
    char    *q = buf;
    int  number= 0;

    while(1) {
        if(p[0] == '\n')
            break;

        if(p[0] == ' ')
            p++;
        else {
            q = p;
            number = 0;
            while ( (q[0] != ' ') && (q[0] != '\n') ) {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount +1;
            p = q;
        }
    }
}

/* 执行命令 */
void do_cmd(int argcount, char arglist[100][256])
{
    int     flag = 0;           /* 用于判断命令中的 < > | 的格式或者格式*/
    int     how  = 0;           /* 用于指示命令中是否有>,<,| */
    int     background = 0;     /* 标识命令中是否有后滩运行符 */
    int     status;
    int     i;
    int     fd;
    char*   arg[argcount+1];
    char*   argnext[argcount+1];
    char*   file;
    pid_t   pid;

    /* 将命令取出 */
    for(i=0; i<argcount; i++) {
        arg[i] = (char *)arglist[i];
    }
    arg[argcount] = NULL;

    /* 查看命令是否有后台运行符 */
     for(i=0; i<argcount; i++) {
         if(strncmp(arg[i],"&",1) == 0) {
             if(i == argcount-1) {
                 background = 1;
                 arg[argcount-1] = NULL;
                 break;
             } else {
                 printf("wrong command\n");
                 return;
             }
         }
     }

     for(i=0; arg[i]!=NULL; i++) {
         if(strcmp(arg[i],">") == 0) {
             flag ++;
             how = out_redirect;
             if(arg[i+1] == NULL)
                 flag ++;
         }
         if(strcmp(arg[i],"<") == 0) {
             flag ++;
             how = in_redirect;
             if(i == 0)
                 flag ++;
         }
         if(strcmp(arg[i],"|") == 0) {
             flag ++;
             how = have_pipe;
             if(arg[i+1] == NULL)
                 flag ++;
             if(i == 0)
                 flag ++;
         }
     }

     /* flag > 1 说明： 命令中有多个 < > |  或者 命令格式不对*/
     if(flag > 1) {
         printf("wrong command\n");
         return;
     }

     /* 19-4-25-00:28 */
    if(how == out_redirect) {  /* 命令只含有一个输出重定向符号 */
        for(i=0; arg[i] != NULL; i++) {
            if(strcmp(arg[i],">") == 0) {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if(how == in_redirect) {  /* 命令只含有一个输入重定向 */
        for(i=0; arg[i] != NULL; i++) {
            if(strcmp(arg[i],"<") == 0) {
                file = arg[i-1];
                arg[i] = NULL;
            }
        }
    }

    if(how == have_pipe) {
        for(i=0; arg[i] != NULL; i++) {
            if(strcmp(arg[i],"|") == 0) {
                arg[i] = NULL;
                int j;
                for(j=i+1; arg[j] != NULL; j++) {
                    argnext[j-i-1] = arg[j];
                }
                argnext[j-i-1] = arg[j];
                break;
            }
        }
    }

    if( (pid = fork()) < 0) {
        printf("fork error\n");
        return;
    }

    switch (how) {
        case 0:
            /* pid 为零说明是子进程 */
            /* 输入的命令不含 > < | */
            if(pid == 0) {
                if( !find_command(arg[0]) ) {
                    printf("%s : command not founn",arg[0]);
                    exit(0);
                }
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 1:
            /* 含有　>  */
            if(pid == 0 ) {
                if(!find_command(arg[0])) {
                    printf("%s : command not found",arg[0]);
                    exit(0);
                }
                fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
                dup2(fd,1);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 2:
            /* 输入的含有重定向　< */
            if(pid == 0) {
                if(!find_command(arg[0])) {
                    printf("%s : command not found",arg[0]);
                    exit(0);
                }
                fd = open(file, O_RDONLY);
                dup2(fd,0);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 3:
            /* 输入的命令中含有管道 */
            if(pid == 0) {
                int pid2;
                int status2;
                int fd2;

                if( (pid2 = fork()) < 0) {
                    printf("fork2 error\n");
                    exit(1);
                }
                else if(pid2 == 0) {
                    if(!find_command(arg[0])) {
                        printf("%s : command not found",arg[0]);
                        exit(0);
                    }
                    fd2 = open("/tmp/youdonotknowfile",
                                    O_WRONLY|O_CREAT|O_TRUNC,0644);
                    dup2(fd2,1);
                    execvp(arg[0], arg);
                    exit(0);
                }

                if(waitpid(pid2,&status2,0) == -1) {
                    printf("wait for child process error\n");
                }

                if(!find_command(argnext[0])) {
                    printf("%s : command not found",argnext[0]);
                    exit(0);
                }

                fd2 = open("/tmp/youdonotknowfile",O_RDONLY);
                dup2(fd2,0);
                execvp(argnext[0],argnext);

                if(remove("/tmp/youdonotknowfile"))
                    printf("remove error \n");
                exit(0);
            }
            break;
        default:
            break;
    }

    /* 若命令中含有& 后台执行,父进程返回,不等待子进程结束 */
    if(background == 1) {
        printf("[process id %d]",pid);
        return;
    }
    /* 父进程等待子进程结束 */
    if(waitpid(pid,&status,0) == -1) {
        printf("wait for child process error\n");
    }
}

/* 查找命令中的可执行程序 */
int  find_command(char *command)
{
    DIR*            dp;
    struct dirent*  dirp;
    char*           path[] = {"./","/bin","/usr/bin",NULL};

    /* 使用当前目录下的程序可以运行,如命令"./fork" 可以正确的结束执行 */
    if(strncmp(command,"./",2) == 0) {
        command = command + 2;
    }
    /* 分别在当前目录,/bin,/usr/bin 目录查找要执行的程序 */
    int i = 0;
    while(path[i] != NULL) {
        if( (dp = opendir(path[i]) ) == NULL) {
            printf("can't open /bin \n");
        }
        while( (dirp = readdir(dp)) != NULL) {
            if(strcmp(dirp->d_name, command) == 0) {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}















