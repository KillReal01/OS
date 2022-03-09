#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <locale.h>

typedef struct{
    int flag;
    char sym;
    sem_t* sem;
} targs;

const char semname[] = "/mysem";
const char filename[] = "text.txt";

void *proc(void *arg){
    printf("\nПоток 2 программы начал работу\n");
    targs* args = (targs*) arg;
    FILE* fp;
    while (args->flag == 0){
        sem_wait(args->sem);
        for (int i = 0; i < 5; i++){
            fp = fopen(filename, "a");
            fputc(args->sym, fp);
            putchar(args->sym);
            fflush(stdout);
            fclose(fp);
            sleep(1);
        }
        sem_post(args->sem);
        sleep(1);
    }
    printf("\nПоток 2 программы завершил работу\n");
    return 0;
}

 int main()
 {
     setlocale(LC_ALL, NULL);
     printf("Программа 2 начала работу\n");
     pthread_t id;
     targs arg;
     arg.flag = 0;
     arg.sym = '2';
     arg.sem = sem_open(semname, O_CREAT, 0644, 1);

     pthread_create(&id, NULL, proc, &arg);
     getchar();
     arg.flag = 1;
     
     pthread_join(id, NULL);
     sem_close(arg.sem);
     sem_unlink(semname);
     
     printf("\nПрограмма 2 завершила работу\n");
     return 0;
 }
