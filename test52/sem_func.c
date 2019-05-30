//
// Created by yang on 19-5-30.
//
#include "sem_func.h"


/* 信号量创建及初始化 */
int createsem(const char* path_name, int proj_id, int members, int init_val)
{
    key_t       msgkey;
    int         index, sid;
    union semun semopts;

    if( (msgkey = ftok(path_name, proj_id)) == -1 ) {
        perror("ftok error!");
        return -1;
    }
    if( (sid = semget(msgkey, members, IPC_CREAT|0666)) == -1 ) {
        perror("senget error!");
        return -1;
    }

    /* 初始化操作 */
    semopts.val = init_val;
    for(index = 0; index<members; index ++) {
        semctl(sid,index,SETVAL,semopts);
    }
    return sid;
}


/* P操作函数 */
int sem_p(int sem_id, int index)
{
    struct sembuf buf = {0, -1, IPC_NOWAIT};

    if(index < 0) {
        perror("index of array cannot equals a minus value!");
        return -1;
    }
    buf.sem_num = index;
    if(semop(sem_id, &buf, 1) == -1) {
        perror("a wrong operation to semaphore occurred!");
        return -1;
    }

    return 0;
}


/* V操作函数 */
int sem_v(int sem_id, int index)
{
    struct sembuf buf = {0, 1, IPC_NOWAIT};

    if(index < 0) {
        perror("index of operation to semaphore occurred!");
        return -1;
    }
    buf.sem_num = index;
    if(semop(sem_id, &buf, 1) == -1) {
        perror("a wrong operation to semaphore occurred!");
        return -1;
    }

    return 0;
}

/* 获取或设置单个信号 */
int semval_op(int semid, int index, int cmd)
{
    if(index < 0) {
        perror("index cannot be minus!");
        return -1;
    }
    if(cmd == GETVAL || cmd == SETVAL) {
        return (semid, index, cmd, 0);
    }

    printf("function cannot surport cmd %d\n",cmd);
    return -1;
}

/* 获取或设置单个信号 */
int semval_op(int semid, int index, int cmd)
{
    if(index < 0) {
        perror("index cannot be minus!");
        return -1;
    }
    if(cmd == GETVAL || cmd == SETVAL) {
        return semctl(semid, index, cmd, 0);
    }

    printf("function cannot surport cmd %d\n",cmd);
    return -1;
}
