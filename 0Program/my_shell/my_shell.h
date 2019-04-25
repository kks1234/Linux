//
// Created by yang on 19-4-23.
//

#ifndef UNTITLED2_MY_SHEEL_H
#define UNTITLED2_MY_SHEEL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>


#define normal              0 /* 一般的命令 */
#define out_redirect        1 /* 输出重定向 */
#define in_redirect         2 /* 输入重定向 */
#define have_pipe           3 /* 命令中有管道 */




void print_prompt(void);                        /* 打印提示符 */
void get_input(char *);                         /* 得到输入的命令 */
void explain_input(char *, int *, char a[100][256]);  /* 对输入的命令进行解析 */
void do_cmd(int , char a[100][256]);                  /* 执行命令 */
int  find_command(char *);                      /* 查找命令中的可执行程序 */





#endif //UNTITLED2_MY_SHEEL_H
