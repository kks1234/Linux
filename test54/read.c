#include "sharemem.h"
#include <string.h>

int main()
{
    int     sem_id, shmid;
    char    *shmaddr;

    if( (shmid = create_shm(".",'m',SHM_SIZE)) == -1 ){
        exit(1);
    }
    if( (shmaddr = shmat(shmid,(char*)0, 0)) == (char*)-1 ) {
        perror("shmat error");
        exit(1);
    }
    if( (sem_id = open_sem(".",'s')) == -1) {
        exit(1);
    }
    
    while(1) {
        printf("Reader: ");
        wait_sem(sem_id, 0);
        sem_p(sem_id, 0);
        printf("%s\n",shmaddr);

        sem_v(sem_id, 0);
        sleep(1);
    }
}

