#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_key_t key;
long tsd = 1;

void* thread2(void* arg)
{
    tsd = 5;
    printf("thread2 %ld is runing\n",pthread_self());
    pthread_setspecific(key,(void *)tsd);
    printf("thread2 %ld returns %ld\n",pthread_self(),(long)pthread_getspecific(key));
}

void* thread1(void* arg)
{
    pthread_t thid2;

    printf("thread1 %ld is runing\n",pthread_self());
    pthread_setspecific(key,(void *)tsd);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(1);
    printf("thread1 %ld returns %ld\n",pthread_self(),(long)pthread_getspecific(key));
}

int main()
{
    pthread_t   thid1;
    printf("main thread begins runing\n");
    pthread_key_create(&key,NULL);
    pthread_create(&thid1,NULL,thread1,NULL);
    sleep(3);
    pthread_key_delete(key);
    printf("main thread exit\n");

    return 0;
}











