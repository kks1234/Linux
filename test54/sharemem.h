#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>



#define SHM_SIZE    1024

union semun{
    int     val;
    struct semid_ds *buf;
    unsigned short *array;
};


/* 创建信号量函数 */

int create_sem(const char* pathname, int proj_id, int members, int init_val)
{
    key_t  key;
    int    index, sid;
    union semun semopts;

    if( (key = ftok(pathname, proj_id)) == -1 ) {
        perror("ftok error");
        return -1;
    }
    if((sid = semget(key,members,IPC_CREAT|0666)) == -1 ) {
        perror("semget error");
        return -1;
    }

    /* 初始化操作 */
    semopts.val = init_val;
    for(index = 0; index < members; index++) {
        semctl(sid, index, SETVAL, semopts);
    }
    return sid;
}

/* 打开信号量函数 */
int open_sem(const char* pathname, int proj_id)
{
    key_t key;
    int sid;

    if( (key = ftok(pathname, proj_id)) == -1 ) {
        perror("ftok error");
        return -1;
    }
    if( (sid = semget(key,0,IPC_CREAT|0666))  == -1 ) {
        perror("semget error");
        return -1;
    }

    return sid;
}


/* P操作函数 */
int sem_p(int sem_id, int index)
{
    struct sembuf buf = {0, -1, IPC_NOWAIT};
    if(index < 0 ) {
        perror("index error");
        return -1;
    }
    buf.sem_num = index;
    if(semop(sem_id, &buf, 1) == -1){
        perror("semop error");
        return -1;
    }
    return 0;
}


/* V操作函数 */
int sem_v(int sem_id, int index)
{
    struct sembuf buf = {0, 1, IPC_NOWAIT};
    if(index < 0) {
        perror("index error");
        return -1;
    }
    buf.sem_num = index;
    if(semop(sem_id, &buf, 1) == -1) {
        perror("semop error");
        return -1;
    }
    return 0;
}


/* 删除信号集函数 */
int sem_delete(int sem_id)
{
    return (semctl(sem_id,0,IPC_RMID));
}

/* 等待信号为1 */
int wait_sem(int sem_id, int index)
{
    while(semctl(sem_id, index, GETVAL,0) == 0) {
        sleep(0);
    }
    return 1;
}

/* 创建共享内存函数 */
int create_shm(const char* pathname, int proj_id, size_t size)
{
    key_t  shmkey;
    int    sid;

    /* 获取键值 */
    if( (shmkey = ftok(pathname, proj_id)) == -1 ) {
        perror("ftok error");
        return -1;
    }
    if( (sid = shmget(shmkey,size, IPC_CREAT|0666)) == -1) {
        perror("shmget error");
        return -1;
    }
    return sid;
}



