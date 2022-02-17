#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
    int flag;
    char sym;
} targs;

void *proc1(void *arg){
    targs* args = (targs*) arg;
    while (args->flag == 0){
        putchar(args->sym);
        fflush(stdout);
        sleep(1);
    }
    pthread_exit((void*)1);
}

void *proc2(void *arg){
    targs* args = (targs*) arg;
    while (args->flag == 0){
        putchar(args->sym);
        fflush(stdout);
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

     pthread_create(&id1, NULL, proc1, &arg1);
     pthread_create(&id2, NULL, proc2, &arg2);
     getchar();
     arg1.flag = 1;
     arg2.flag = 1;
     pthread_join(id1, (void**)&exitcode1);
     pthread_join(id2, (void**)&exitcode2);

     printf("exitcode1 = %p\n", exitcode1);
     printf("exitcode2 = %p\n", exitcode2);
     return 0;
 }
