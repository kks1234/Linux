#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void)
{
    printf("this is run thread, thread id = %lu\n",pthread_self());
    return;
}



void* thread1(void * arg)
{
    printf("current thread id = %lu\n",pthread_self());
    pthread_once(&once, run);
    printf("thread1 ends\n");
    return NULL;
}


void* thread2(void * arg)
{
    printf("current thread id = %lu\n", pthread_self());
    pthread_once(&once, run);
    printf("thread2 ends\n");
    return NULL;
}



int main()
{
    pthread_t thid1, thid2;

    pthread_create(&thid1,NULL,thread1,NULL);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(3);
    printf("main thread exit\n");
    exit(0);
}
