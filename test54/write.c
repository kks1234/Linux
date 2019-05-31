#include "sharemem.h"
#include <string.h>
int main()
{
    int     sem_id, shmid;
    char    *shmaddr;
    char    write_str[SHM_SIZE];
    
    if( (shmid = create_shm(".",'m',SHM_SIZE)) == -1 ){
        exit(1);
    }
    if( (shmaddr = shmat(shmid,(char*)0, 0)) == (char*)-1 ) {
        perror("shmat error");
        exit(1);
    }
    if( (sem_id = create_sem(".",'s',1,1)) ==-1 ) {
        exit(1);
    }
    while(1) {
        wait_sem(sem_id, 0);
        sem_p(sem_id, 0);
	printf("Write: ");
        fgets(write_str, 1024, stdin);
        int len = strlen(write_str) - 1;
        write_str[len] = '\0';
        strcpy(shmaddr, write_str);
      //  sleep(10);
        
        sem_v(sem_id, 0);
        sleep(1);
    }
}

