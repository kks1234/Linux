#include "my_ls.h"




int main(int argc, char * argv[])
{
    int i,j,k,num;
    char path[PATH_MAX+1];
    char param[32]; //保存命令行参数  目标文件名和目录名不在此列
    int flag_param = PARAM_NONE;
    struct stat buf;
    /* 命令行参数解析 */
    j = 0;
    num = 0;
    for(i=1; i<argc; i++) {
        if(argv[i][0] == '-') {
            for(k=1; k<strlen(argv[i]); k++,j++) {
                param[j] = argv[i][k];
            }
            num ++;
        }
    }

    /* 只支持参数a和l 如果含有其他参数就报错*/
    for(i=0; i<j; i++) {
        if(param[i] == 'a') {
            flag_param |= PARAM_A;
            continue;
        } else if(param[i] == 'l') {
            flag_param |= PARAM_L;
            continue;
        } else {
            printf("my_ls : invaild option -%c\n",param[i]);
            exit(1);
        }
    }
    param[j] = '\0';


    /* 如果没有输文件名或者目录, 就输出当前目录 */
    if(num+1 == argc) {
        strcpy(path,"./");
        path[2] = '\0';
        display_dir(flag_param,path);
        return 0;
    }

    i = 1;
    do {
        //如果不是目标文件或者目录,解析下一个命令行参数
        if(argv[i][0] == '-') {
            i++;
            continue;
        } else {
            strcpy(path, argv[i]);

            //如果文件不存在则退出程序,并报错
            if(stat(path, &buf) == -1) {
                my_error("stat", __LINE__);
            }

            if(S_ISDIR(buf.st_mode)) { //argv[i]是一个目录
                //如果目录最后不是/  就加上 /
                if(path[strlen(argv[i]) - 1] != '/') {
                    path[strlen(argv[i])] = '/';
                    path[strlen(argv[i])+1] = '\0';
                } else
                    path[strlen(argv[i])] = '\0';

                display_dir(flag_param, path);
                i++;
            } else {
                display(flag_param, path);
                i++;
            }
        }
    }while(i < argc);


    return 0;


}