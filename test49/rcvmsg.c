#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define     BUF_SIZE    256
#define     PROJ_ID     32
#define     PATH_NAME   "."

int main()
{
    /* 用户自定义消息缓冲 */
    struct my_msgbuf {
        long msgtype;
        char ctrlstring[BUF_SIZE];
    }msgbuffer;

    int     qid; /* 消息队列标识符 */
    int     msglen;
    key_t   msgkey;

    /* 获取键值 */
    if( (msgkey = ftok(PATH_NAME,PROJ_ID)) == -1) {
        perror("ftok error!");
        exit(1);
    }
    /* 创建消息队列 */
    if((qid = msgget(msgkey,IPC_CREAT|0660)) == -1) {
        perror("msgget error!");
        exit(1);
    }

    msglen = sizeof(struct my_msgbuf) - 4;
    if(msgrcv(qid,&msgbuffer,msglen,3,0) == -1) {
        perror("msgsnd error!");
        exit(1);
    }
    printf("Get message is: %s",msgbuffer.ctrlstring);
    exit(0);
}

