#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <sys/stat.h>

typedef struct{
    int flag;
    char sym;
} targs;

int filedes[2];

void *proc1(void *arg){
    printf("\nПоток 1 начал работу\n");
    targs* args = (targs*) arg;
    int buf;
    while (args->flag == 0){
        struct stat statbuf;
        int ret = stat("lab3.c", &statbuf);
        buf = statbuf.st_size;
        //ssize_t k = write(filedes[1], &buf, sizeof(buf));
        sleep(1);
    }
    printf("\nПоток 1 завершил работу\n");
    pthread_exit((void*)1);
}

void *proc2(void *arg){
    printf("\nПоток 2 начал работу\n");
    targs* args = (targs*) arg;
    int buf;
    while (args->flag == 0){
        ssize_t k = read(filedes[0], &buf, sizeof(buf));
        printf("size of file %i bytes\n", buf);
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
     int *exitcode1, *exitcode2;

     arg1 = {0, '1'};
     arg2 = {0, '2'};

     pipe(filedes);

     pthread_create(&id1, NULL, proc1, &arg1);
     pthread_create(&id2, NULL, proc2, &arg2);
     
     printf("\nПрограмма ждет нажатия клавиши\n");
     getchar();
     printf("\nКлавиша нажата\n");
     arg1.flag = 1;
     arg2.flag = 1;
     
     pthread_join(id1, (void**)&exitcode1);
     pthread_join(id2, (void**)&exitcode2);
     close(filedes[0]);
     close(filedes[1]);

     printf("\nexitcode1 = %p\n", exitcode1);
     printf("exitcode2 = %p\n", exitcode2);
     printf("\nПрограмма завершила работу\n");

     return 0;
 }
