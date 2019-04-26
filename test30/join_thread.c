#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void assisthread(void*arg)
{
    printf("I am helping to do some jobs\n");
    sleep(3);
    pthread_exit(0);
}

int main()
{
    pthread_t   thid;
    void*       status;

    pthread_create(&thid,NULL,(void*)assisthread,NULL);
    pthread_join(thid, &status);
    printf("assisthread's exit is caused %ld\n",(long)status);

    return 0;

}

