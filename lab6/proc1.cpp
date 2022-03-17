#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <locale.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <sys/stat.h>
#include <signal.h>

int flag = 0;
const char name_sem_read[] = "/mysemread";
const char name_sem_write[] = "/mysemwrite";
const char filename[] = "text.txt";

sem_t* sem_read;
sem_t* sem_write;
int shmid;
int* shmaddr;

void *proc(void*){
    printf("\nПоток 1 программы начал работу\n");
    int ret, buf;
    while (flag == 0){
        struct stat statbuf;
        ret = stat("proc1.cpp", &statbuf);
        buf = statbuf.st_size;

        printf("Размер файла: %d\n", buf);
        *shmaddr = buf;

        sem_post(sem_write);
        sem_wait(sem_read);
        sleep(1);
    }
    printf("\nПоток 1 программы завершил работу\n");
    return 0;
}

void sig_handler(int signo){
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, 0);
    sem_post(sem_write);
    sem_post(sem_read);
    _exit(0);
}

 int main()
 {
     setlocale(LC_ALL, NULL);
     signal(SIGINT, sig_handler);
     printf("Программа 1 начала работу\n");
     pthread_t id;
     
     key_t key = ftok(filename, 'a');
     shmid = shmget(key, sizeof(int), IPC_CREAT|0666);
     shmaddr = (int *)shmat(shmid, 0, 0);

     sem_read = sem_open(name_sem_read, O_CREAT, 0644, 1);
     sem_write = sem_open(name_sem_write, O_CREAT, 0644, 1);
     pthread_create(&id, NULL, proc, NULL);

     getchar();
     flag = 1;
     
     pthread_join(id, NULL);
     sem_close(sem_read);
     sem_close(sem_write);
     sem_unlink(name_sem_read);
     sem_unlink(name_sem_write);

     shmdt(shmaddr);
     shmctl(shmid, IPC_RMID, 0);

     printf("\nПрограмма 1 завершила работу\n");
     return 0;
 }
