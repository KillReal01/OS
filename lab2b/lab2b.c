#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct{
    int flag;
    char sym;
} targs;

sem_t sem;

void *proc1(void *arg){
    targs* args = (targs*) arg;
    while (args->flag == 0){
        //sem_wait(&sem);//1
        /*while (sem_trywait(&sem) != 0){//2
        	sleep(1);
        }*/

        timespec tp;//3
        clock_gettime(CLOCK_REALTIME, &tp);
        tp.tv_sec += 1;
        while (sem_timedwait(&sem, &tp));
        for (int i = 0; i < 5; i++){
            putchar(args->sym);
            fflush(stdout);
            sleep(1);
        }
        sem_post(&sem);
        sleep(1);
    }
    pthread_exit((void*)1);
}

void *proc2(void *arg){
    targs* args = (targs*) arg;
    while (args->flag == 0){
        //sem_wait(&sem);//1
        /*while (sem_trywait(&sem) != 0){//2
        	sleep(1);
        }*/

        timespec tp;//3
        clock_gettime(CLOCK_REALTIME, &tp);
        tp.tv_sec += 1;
        while (sem_timedwait(&sem, &tp));
        for (int i = 0; i < 5; i++){
            putchar(args->sym);
            fflush(stdout);
            sleep(1);
        }
        sem_post(&sem);
        sleep(1);
    }
    pthread_exit((void*)2);
}
    
 int main()
 {
     pthread_t id1, id2;
     targs arg1, arg2;
     arg1.flag = 0;
     arg2.flag = 0;
     arg1.sym = '1';
     arg2.sym = '2';

     int *exitcode1, *exitcode2;
     
     sem_init(&sem, 0, 1);
     pthread_create(&id1, NULL, proc1, &arg1);
     pthread_create(&id2, NULL, proc2, &arg2);
     
     getchar();
     arg1.flag = 1;
     arg2.flag = 1;
     
     pthread_join(id1, (void**)&exitcode1);
     pthread_join(id2, (void**)&exitcode2);
     sem_destroy(&sem);

     printf("\nexitcode1 = %p\n", exitcode1);
     printf("exitcode2 = %p\n", exitcode2);
     return 0;
 }
