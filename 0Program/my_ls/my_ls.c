//
// Created by yang on 19-4-22.
//
#include "my_ls.h"

int g_leave_len = MAXROWLEN;    //一行剩余的长度
int g_maxlen;                    //存放某目录下最长目录的长度


/* 错误处理函数 */
void my_error(const char * str_error, int line)
{
    fprintf(stderr,"line : %d",__LINE__);
    perror(str_error);
    exit(1);
}


/**S_ISLNK(st_mode)：是否是一个连接.
 * S_ISREG(st_mode)：是否是一个常规文件.
 * S_ISDIR(st_mode)：是否是一个目录
 * S_ISCHR(st_mode)：是否是一个字符设备.
 * S_ISBLK(st_mode)：是否是一个块设备
 * S_ISFIFO(st_mode)：是否 是一个FIFO文件.
 * S_ISSOCK(st_mode)：是否是一个SOCKET文件
 **/
void display_attribute(struct stat buf, char *name)
{
    char    buf_time[32];
    struct passwd   *psd;   //从该结构体中获取文件所有者的用户名
    struct group    *grp;   //从该结构体中获取文件所属组的组名

    /* 获取并打印文件类型 */
    if(S_ISLNK(buf.st_mode)) {
        printf("l");
    } else if(S_ISREG(buf.st_mode)) {
        printf("-");
    } else if(S_ISDIR(buf.st_mode)) {
        printf("d");
    } else if(S_ISCHR(buf.st_mode)) {
        printf("c");
    } else if(S_ISBLK(buf.st_mode)) {
        printf("b");
    } else if(S_ISFIFO(buf.st_mode)) {
        printf("f");
    } else if(S_ISSOCK(buf.st_mode)) {
        printf("s");
    }

    /* 获取并打印文件所有者的权限 */
    if(buf.st_mode & S_IRUSR) {
        printf("r");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IWUSR) {
        printf("w");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IXUSR) {
        printf("x");
    } else {
        printf("-");
    }

    /* 获取并打印其他用户对该文件的操作权限 */
    if(buf.st_mode & S_IRGRP) {
        printf("r");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IWGRP) {
        printf("w");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IXGRP) {
        printf("x");
    } else {
        printf("-");
    }

    /* 打印其他用户对该文件的操作权限 */
    if(buf.st_mode & S_IROTH) {
        printf("r");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IWOTH) {
        printf("w");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IXOTH) {
        printf("x");
    } else {
        printf("-");
    }

    printf("   ");

    /* 根据uid和gid获取文件所有者用户名和组名 */
    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%ld ",buf.st_nlink);
    printf("%-8s",psd->pw_name);
    printf("%-8s",grp->gr_name);

    printf("%6ld",buf.st_size);  //打印文件大小
    strcpy(buf_time, ctime((time_t *)&(buf.st_mtim)));
    buf_time[strlen(buf_time) - 1] = '\0';
    printf("  %s",buf_time);    //打印文件时间信息

}

/* 在没有使用-l选项时,打印一个文件名,打印时上下对其 */
void display_single(char* name)
{
    int i,len;

    //如果本行不足以打印一个文件名则换行
    if(g_leave_len < g_maxlen) {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }

    len = strlen(name);
    len = g_maxlen - len;
    printf("%-s",name);

    for(i = 0; i<len; i++) {
        printf(" ");
    }
    printf("  ");
    /* 2 : 两个空格 */
    g_leave_len -= (g_maxlen +2);
}



/**
 * 根据命令行参数和完整的路径名显示目标文件
 * 参数： flag 命令行参数
 *       pathname 包含了文件的路径名
 **/
void display(int flag, char * pathname)
{
    int             i,j;
    struct stat     buf;
    char            name[NAME_MAX + 1];

    /* 从路径中解析出文件名 */
    for(i=0, j=0; i<strlen(pathname); i++)
    {
        if(pathname[i] == '/') {
            j = 0;
            continue;
        }
        name[j++] = pathname[i];
    }
    name[j] = '\0';

    /* 用lstat而不是stat方便解析链接文件 */
    if(lstat(pathname,&buf) == -1) {
        my_error("lstat",__LINE__);
    }

    switch(flag) {
        case PARAM_NONE:    //没有-l -a
            if(name[0] != '.') {
                display_single(name);
            }
	    break;
        case PARAM_A:       //-a
            display_single(name);
            break;
        case PARAM_L:
            if(name[0] != '.') {
                display_attribute(buf,name);
                printf("  %-s\n",name);
            }
            break;
        case PARAM_A + PARAM_L:
            display_attribute(buf,name);
            printf("  %-s\n",name);
            break;
        default:
            break;
    }
}

void display_dir(int flag_param, char * path)
{
    DIR                 *dir;
    struct dirent       *ptr;
    int                 count = 0;
    char                filename[256][PATH_MAX+1],temp[PATH_MAX+1];

    //获取该文件下的文件总数和最长文件名
    dir = opendir(path);
    if(dir == NULL) {
        my_error("DIR", __LINE__);
    }
    while((ptr = readdir(dir)) != NULL) {
        if(g_maxlen < strlen(ptr->d_name))
            g_maxlen = strlen(ptr->d_name);
        count ++ ;
    }
    closedir(dir);

    if(count > 256)
        my_error("too many files under this dir",__LINE__);

    int i,j,len = strlen(path);
    //获取文件下所有的文件名
    dir = opendir(path);
    for(i=0; i<count; i++) {
        ptr = readdir(dir);
        if(ptr == NULL) {
            my_error("readdir",__LINE__);
        }
        strncpy(filename[i],path,len);
        filename[i][len] = '\0';
        strcat(filename[i], ptr->d_name);
        filename[i][len + strlen(ptr->d_name)] = '\0';
    }

    //使用冒泡排序对文件进行排序
    for(i=0; i<count-1; i++)
        for(j=0; j<count-1-i; j++) {
            if(strcmp(filename[j],filename[j+1]) > 0) {
                strcpy(temp, (const char*)filename[j+1]);
                temp[strlen(filename[j+1])] = '\0';
                strcpy(filename[j+1], filename[j]);
                filename[j+1][strlen(filename[j])] = '\0';
                strcpy(filename[j],temp);
                filename[j][strlen(temp)] = '\0';
            }
        }
    for(i=0; i<count; i++) {
        display(flag_param, filename[i]);
    }

    closedir(dir);

    //如果命令行中没有-l选项 ,打印一个换行符
    if( (flag_param & PARAM_L) == 0) {
        printf("\n");
    }
}

























