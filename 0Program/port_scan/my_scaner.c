//端口扫描
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//定义端口区间信息
typedef struct _port_segment {
    struct in_addr  dest_ip; //目标IP
    unsigned short int min_port;
    unsigned short int max_port; //最大端口
}port_segment;

/* 自定义错误处理函数 */
void my_err(const char* str_err, int line)
{
    fprintf(stderr,"line: %d ",line);
    perror(str_err);
    exit(1);
}

/*
 * 扫描某个IP地址上的某个端口函数
 * return 1- error
 *        0  no open
 *        1  open
 */
int do_scan(struct sockaddr_in recv_addr)
{
    int conn_fd;
    int ret;

    //创建你一个套接字
    if( (conn_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
        my_err("socket",__LINE__);
    }

    //向服务器发送链接请求

    if( (ret = connect(conn_fd,(struct sockaddr*)&recv_addr, sizeof(struct sockaddr))) < 0 ) {
        if(errno == ECONNREFUSED) {  // 端口未打开
            close(conn_fd);
            return 0;
        } else {   //其他错误
            close(conn_fd);
            return -1;
        }
    } else if( ret == 0 ) {
        printf("port %d found in %s\n",ntohs(recv_addr.sin_port),inet_ntoa(recv_addr.sin_addr));
        close(conn_fd);
        return 1;
    }
    return -1;
}

/**
 * 执行扫描的线程
 */
void* scaner(void* arg)
{
    unsigned short int i;
    struct sockaddr_in serv_addr;
    port_segment    portinfo;  //端口信息

    //读取端口区间信息
    memcpy(&portinfo,arg, sizeof( struct _port_segment));

    //初始化服务器地址端结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = portinfo.dest_ip.s_addr;

    for(i = portinfo.min_port; i < portinfo.max_port; i++) {
        serv_addr.sin_port = htons(i);
        if( do_scan(serv_addr) < 0 ) {
            continue;
        }
    }
    return NULL;
}

/**
 *
 * @param argc
 * @param argv  -m  最大端口 -a  目标主机的IP地址 -n  最大线程数
 * @return
 */
int main(int argc, char* argv[])
{
    pthread_t*      thread;
    int             max_port;
    int             thread_num;
    int             seg_len;
    struct in_addr  dest_ip;
    int             i;

    //检查参数个数
    if(argc != 7) {
        printf("Usage: [-m] [max_port] [-a] [serv_address] [-n] [thread_number]\n");
        exit(1);
    }

    //解析命令行参数
    for(i=1; i<argc; i++) {
        if( strcmp("-m",argv[i]) == 0 ) {
            max_port = atoi(argv[i+1]); //字符串穿转换数字
            if(max_port < 0 || max_port > 65535) {
                printf("Usage: invalid max dest port\n");
                exit(1);
            }
            continue;
        }
        if(strcmp("-a", argv[i]) == 0) {
            if(inet_aton(argv[i+1],&dest_ip) == 0) {
                printf("Usage: invalid thread_number\n");
                exit(1);
            }
            continue;
        }
        if(strcmp("-n",argv[i]) == 0) {
            thread_num  = atoi(argv[i+1]);
            if(thread_num <= 0) {
                printf("Usage: invalid thread_number\n");
                exit(1);
            }
            continue;
        }
    }
    //如果输入的最大端口号小于线程数 则把线程数改为最大端口号
    if(max_port < thread_num) {
        thread_num = max_port;
    }

    seg_len = max_port / thread_num;
    if(max_port%thread_num != 0) {
        thread_num ++;
    }
    //分配所有线程的内存空间
    thread = (pthread_t*)malloc(thread_num * sizeof(pthread_t));

    //创建线程 根据最大端口号和线程数分配每个线程扫描端口的区间
    for(i=0; i<thread_num; i++) {
        port_segment portinfo;
        portinfo.dest_ip = dest_ip;
        portinfo.min_port = i*seg_len +1;
        if( i == thread_num-1) {
            portinfo.max_port = max_port;
        } else {
            portinfo.max_port = portinfo.min_port + seg_len -1;
        }
        //创建线程
        if( pthread_create(&thread[i],NULL,scaner,(void*)&portinfo) != 0 ) {
            my_err("pthread_create", __LINE__);
        }
        //主进程等待线程结束
        pthread_join(thread[i],NULL);
    }
    return 0;
}


