#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>

typedef struct{
    int flag;
    char sym;
} targs;

pthread_mutex_t mutex;

void *proc1(void *arg){
    printf("\nПоток 1 начал работу\n");
    targs* args = (targs*) arg;
    while (args->flag == 0){
        int ret = pthread_mutex_lock(&mutex);
        if (ret == 0){
            for (int i = 0; i < 5; i++){
        	    putchar(args->sym);
        	    fflush(stdout);
        	    sleep(1);
            }
            pthread_mutex_unlock(&mutex);
        }
        else{
            printf("\nОшибка в 1 потоке:%s\n", strerror(ret));
            fflush(stdout);
        }
        sleep(1);
    }
    printf("\nПоток 1 завершил работу\n");
    pthread_exit((void*)1);
}

void *proc2(void *arg){
    printf("\nПоток 2 начал работу\n");
    targs* args = (targs*) arg;
    while (args->flag == 0){
        int ret = pthread_mutex_lock(&mutex);
        if (ret == 0){
            for (int i = 0; i < 5; i++){
        	    putchar(args->sym);
        	    fflush(stdout);
        	    sleep(1);
            }
            pthread_mutex_unlock(&mutex);
        }
        else{
            printf("\nОшибка во 2 потоке:%s\n", strerror(ret));
            fflush(stdout);
        }
        sleep(1);
    }
    printf("\nПоток 2 завершил работу\n");
    pthread_exit((void*)2);
 }
    
 int main()
 {
     setlocale(LC_ALL, NULL);
     printf("Программа начала работу\n");
     pthread_t id1, id2;
     targs arg1, arg2;
     arg1 = {0, '1'};
     arg2 = {0, '2'};

     int *exitcode1, *exitcode2;
     
     pthread_mutex_init(&mutex, NULL);
     pthread_create(&id1, NULL, proc1, &arg1);
     pthread_create(&id2, NULL, proc2, &arg2);
     
     printf("\nПрограмма ждет нажатия клавиши\n");
     getchar();
     printf("\nКлавиша нажата\n");
     arg1.flag = 1;
     arg2.flag = 1;
     
     pthread_join(id1, (void**)&exitcode1);
     pthread_join(id2, (void**)&exitcode2);
     pthread_mutex_destroy(&mutex);

     printf("\nexitcode1 = %p\n", exitcode1);
     printf("exitcode2 = %p\n", exitcode2);
     printf("\nПрограмма завершила работу\n");
     return 0;
 }
