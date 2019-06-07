//Client/Server模型的服务器端
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "my_recv.h" //自定义的头文件



#define     SERV_PORT           8083 //服务器的端口号
#define     LISTENQ             12   //链接请求队列的最大长度

#define     INVALID_USERINFO    'n'  //用户信息无效
#define     VALID_USERINFO      'y'  //用户信息有效

#define     USERNAME            0    //接收到的是用户名
#define     PASSWORD            1    //接收到的是密码

struct userinfo {
    char username[32];
    char password[32];
};

struct userinfo users[] = {
        {"linux", "unix"},
        {"4507", "4508"},
        {"clh", "clh"},
        {"xl", "xl"},
        {"yck", "yck"},
        {" ", " "}
};

//
int find_name(const char* name)
{
    int i;
    if(name == NULL) {
        printf("in find name, NULL pointer\n");
        return -2;
    }
    for(i=0; users[i].username[0] != ' '; i++) {
        if(strcmp(users[i].username, name) == 0) {
            return i;
        }
    }
    return -1;
}

//发送数据
void send_data(int conn_fd, const char* string)
{
    if(send(conn_fd, string, strlen(string),0) < 0) {
        my_err("send", __LINE__);
    }
}

int main()
{
    int     sock_fd, conn_fd;
    int     optval;
    int     flag_recv = USERNAME; //接收到的是用户名
    int     ret;
    int     name_num;
    pid_t   pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;
    char    recv_buf[128];
    //创建一个TCP套接字
    if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        my_err("socket", __LINE__);
    }
    //设置套接字可以重新绑定端口
    optval =1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(int)) < 0) {
        my_err("setsockopt", __LINE__);
    }
    //初始化服务器地址结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将套接字绑定到本地端口
    if( bind(sock_fd,(struct  sockaddr*)&serv_addr, sizeof(struct sockaddr_in)) < 0) {
        my_err("bind",__LINE__);
    }
    //将套接字转换成监听套接字
    if( listen(sock_fd, LISTENQ) < 0) {
        my_err("listen", __LINE__);
    }
    cli_len = sizeof(struct sockaddr_in);
    while(1) {
        // 通过accept函数接收客户端的链接请求,并返回链接套接字用于收发数据
        conn_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if(conn_fd < 0) {
            my_err("accept", __LINE__);
        }
        printf("accept a new client , ip is %s\n",inet_ntoa(cli_addr.sin_addr)); 
        //创建一个子进程处理连接请求
        if((pid = fork()) == 0) { //子进程
            while(1) {
                if( (ret = recv(conn_fd, recv_buf, sizeof(recv_buf),0)) < 0 ) {
                    my_err("recv",__LINE__);
                    exit(1);
                }
                recv_buf[ret-1] = '\0';

                if(flag_recv == USERNAME) {
                    name_num = find_name(recv_buf);
                    switch(name_num) {
                        case -1:
                            send_data(conn_fd,"n\n");
                            break;
                        case -2:
                            exit(1);
                            break;
                        default:
                            send_data(conn_fd, "y\n");
                            flag_recv = PASSWORD;
                            break;
                    }
                } else if (flag_recv == PASSWORD) {
                    if(strcmp(users[name_num].password, recv_buf) == 0) {
                        send_data(conn_fd, "y\n");
                        send_data(conn_fd, "Welcome login my tcp server\n");
                        printf("%s login\n",users[name_num].username);
                        break; //跳出while循环
                    } else
                        send_data(conn_fd, "n\n");
                }
            }

            close(conn_fd);
            close(sock_fd);
            exit(0);
        } else {
            close(conn_fd);
        }
    }

    return 0;
}

