//
// Created by yang on 19-5-30.
//

#ifndef _SEM_FUNC_H
#define _SEM_FUNC_H

#include <stdio.h>
//#include <linux/sem.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>


union semun {
   int val;

};

int createsem(const char* path_name, int proj_id, int members, int init_val);
int sem_p(int sem_id, int index);
int sem_v(int sem_id, int index);
int semval_op(int semid, int index, int cmd);


#endif //UNTITLED3_SEM_FUNC_H

