#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <locale.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>

int flag = 0;
mqd_t mq;
char name_queue[] = "/myqueue";

void *proc(void*){
    printf("\nПоток 2 программы начал работу\n");
    int ret;
    char buf[2048];
    while (flag == 0){
        memset(buf, 0x00, sizeof(buf));
        ret = mq_receive(mq, buf, 2048, 0);
        if (ret >= 0){
            printf("Размер файла: %s\n", buf);
        }
        else if (ret == -1){
            perror("mq_receive");
            sleep(1);
            continue;
        }
    }
    printf("\nПоток 2 программы завершил работу\n");
    return 0;
}

 int main()
 {
     setlocale(LC_ALL, NULL);
     printf("Программа 2 начала работу\n");
     pthread_t id;
     mq_attr attr;
     attr.mq_maxmsg = 15;
     attr.mq_msgsize = 1024;

     mq = mq_open(name_queue, O_CREAT|O_RDONLY|O_NONBLOCK, 0644, &attr);

     printf("size: %ld\n", attr.mq_maxmsg);

     pthread_create(&id, NULL, proc, NULL);
     
     getchar();
     flag = 1;

     pthread_join(id, NULL);
     mq_close(mq);
     mq_unlink(name_queue);

     printf("\nПрограмма 2 завершила работу\n");
     return 0;
 }