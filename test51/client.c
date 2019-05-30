#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>



#define BUF_SIZE    256
#define PROJ_ID     32
#define PATH_NAME   "/tmp"
#define SERVER_MSG  1
#define CLIENT_MSG  2


int main()
{
    /* 自定义缓冲区 */
    struct my_msgbuf {
        long msgtype;
        char ctlstring[BUF_SIZE];
    }msgbufer;
    int qid; /* 消息队列标识符 */
    int msglen;
    key_t  msgkey;
    struct msqid_ds msq_attr;

    /* 获取键值 */
    if( (msgkey = ftok(PATH_NAME,PROJ_ID)) == -1 ) {
        perror("ftok error!");
        exit(1);
    }
    /* 获取消息队列标识符 */
    if( (qid = msgget(msgkey,IPC_CREAT|0660)) == -1 ) {
        perror("msgget error!");
        exit(1);
    }
    while(1) {
        if(msgrcv(qid,&msgbufer,BUF_SIZE,SERVER_MSG,0) == -1) {
            perror("msgrcv error!");
            exit(1);
        }
        printf("Server: %s\n",msgbufer.ctlstring);

        printf("client: ");
        fgets(msgbufer.ctlstring,BUF_SIZE,stdin);
        if(strncmp("exit", msgbufer.ctlstring, 4) == 0) {
            break;
        }
        msgbufer.ctlstring[strlen(msgbufer.ctlstring) - 1] = '\0';
        msgbufer.msgtype = CLIENT_MSG;
        if(msgsnd(qid, &msgbufer, sizeof(msgbufer.ctlstring),0) == -1) {
            perror("msgsnd error!");
            exit(1);
        }
    }
    exit(0);
}


