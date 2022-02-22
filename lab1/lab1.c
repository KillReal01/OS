#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>

typedef struct{
    int flag;
    char sym;
} targs;

void *proc1(void *arg){
    printf("\nПоток 1 начал работу\n");
    targs* args = (targs*) arg;
    while (args->flag == 0){
        sleep(1);
        putchar(args->sym);
        fflush(stdout);
    }
    printf("\nПоток 1 завершил работу\n");
    pthread_exit((void*)1);
}

void *proc2(void *arg){
    printf("\nПоток 2 начал работу\n");
    targs* args = (targs*) arg;
    while (args->flag == 0){
        sleep(1);
        putchar(args->sym);
        fflush(stdout);
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
     cpu_set_t cpu1, cpu2;
     int *exitcode1, *exitcode2;
     
     arg1 = {0, '1'};
     arg2 = {0, '2'};

     CPU_ZERO(&cpu1);
     CPU_SET(0, &cpu1);
     pthread_create(&id1, NULL, proc1, &arg1);
     pthread_setaffinity_np(id1, sizeof(cpu_set_t), &cpu1);

     CPU_ZERO(&cpu2);
     CPU_SET(1, &cpu2);
     pthread_create(&id2, NULL, proc2, &arg2);
     pthread_setaffinity_np(id2, sizeof(cpu_set_t), &cpu2);
     
     printf("\nПрограмма ждет нажатия клавиши\n");
     getchar();
     printf("\nКлавиша нажата\n");
     arg1.flag = 1;
     arg2.flag = 1;
     
     pthread_join(id1, (void**)&exitcode1);
     pthread_join(id2, (void**)&exitcode2);
     
     printf("\nexitcode1 = %p\n", exitcode1);
     printf("exitcode2 = %p\n", exitcode2);
     printf("\nПрограмма завершила работу\n");

     return 0;
 }
