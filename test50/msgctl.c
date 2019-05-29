#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>



#define BUF_SIZE    256
#define PROJ_ID     32
#define PATH_NAME   "."


void getmsgattr(int msgid, struct msqid_ds msginfo);

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
    getmsgattr(qid, msq_attr); /* 输出消息队列属性 */

    /* 发送一消息 */
    msgbufer.msgtype = 2;
    memcpy(msgbufer.ctlstring,"Hello World!",sizeof("Hello World!"));
    msglen = sizeof(struct my_msgbuf) - 4;
    if( msgsnd(qid, &msgbufer, msglen,0) == -1 ) {
        perror("msgsnd error!");
        exit(1);
    }
    getmsgattr(qid, msq_attr); /* 输出消息队列属性 */

    /* 设置属性 */
    msq_attr.msg_perm.uid = 3;
    msq_attr.msg_perm.gid = 2;
    if(msgctl(qid,IPC_SET, &msq_attr) == -1 ){
        perror("msgctl error!");
        exit(1);
    }
    getmsgattr(qid, msq_attr); /* 输出消息队列属性 */
    if( msgctl(qid, IPC_RMID, NULL) == -1 ) {
        perror("dele msg error!");
        exit(1);
    }
    getmsgattr(qid, msq_attr);  /* 输出消息队列属性 */

    exit(0);
}


void getmsgattr(int msgid, struct msqid_ds msginfo)
{
    if(msgctl(msgid,IPC_STAT,&msginfo) == -1) {
        perror("msgctl error!");
        exit(1);
    }
    printf("****information of message queue%d\n",msgid);
    printf("last msgsnd to msg time is %s\n",ctime(&msginfo.msg_stime));
    printf("last msgrcv time from msg is %s\n",ctime(&msginfo.msg_rtime));
    printf("last change msg time is %s\n",ctime(&msginfo.msg_ctime));
    printf("current number of bytes on queue is %ld\n",msginfo.__msg_cbytes);
    printf("number if messages in queue is %ld\n",msginfo.msg_qnum);
    printf("max number of bytes on queue is %ld\n",msginfo.msg_qbytes);
    printf("pid of last msgsnd is %d\n",msginfo.msg_lspid);
    printf("pid of last msgrcv is %d\n",msginfo.msg_lrpid);
    printf("msg uid is %d\n",msginfo.msg_perm.uid);
    printf("msg gid is %d\n",msginfo.msg_perm.gid);
    printf("******information end!**********\n");

}

