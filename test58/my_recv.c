//
// Created by yang on 19-6-5.
//
#define MY_RECV_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "my_recv.h"


/* 自定义错误处理函数 */
void my_err(const char* err_string, int line)
{
    fprintf(stderr, "line: %d",line);
    perror(err_string);
    exit(1);
}


/**
 * 函数名: my_recv
 * 描述: 从套接字读取一次数据(以 \n 为结束)
 * param: conn_fd 链接的套接字
 * param: data_buf 数据缓冲区
 * param: len 数据长度
 *
 * return: -1 出错 0 服务器链接已关闭   成功返回读取的字节数
 */
int  my_recv(int conn_fd, char* data_buf, int len)
{
    static char     recv_buf[BUFSIZE]; //自定义缓冲区
    static char*    pread; //指向下一次读取数据的位置、
    static int      len_remain = 0; //自定义缓冲区中剩余的字节数
    int             i;
    //如果自定义缓冲区中没有数据 则从套接字读取数据
    if(len_remain <= 0) {
        if( (len_remain = recv(conn_fd, recv_buf, sizeof(recv_buf),0)) < 0 ) {
            my_err("recv", __LINE__);
        } else if(len_remain == 0) {
            return 0;
        }
        pread = recv_buf;
    }
    for(i=0; *pread != '\n'; i++) {
        if(i > len) {
            return -1;
        }
        data_buf[i] = *pread ++;
        len_remain --;
    }
    //去除结束标志
    len_remain --;
    pread ++;

    return i;
}

