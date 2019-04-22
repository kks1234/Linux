//
// Created by yang on 19-4-22.
//

#ifndef UNTITLED1_MY_LS_H
#define UNTITLED1_MY_LS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>


#define PARAM_NONE    0     //无参数
#define PARAM_A       1     //-a
#define PARAM_L       2     //-l
#define MAXROWLEN     80    //一行显示的最多字数







void my_error(const char * str_error, int line);
void display_attribute(struct stat buf, char *name);
void display_single(char* name);
void display(int flag, char * pathname);
void display_dir(int flag_param, char * path);





#endif //UNTITLED1_MY_LS_H
