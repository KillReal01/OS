#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <locale.h>
#include <queue>
#include <signal.h>

int flag_g = 0, flag_s = 0, flag_p = 0;
int sock;
struct sockaddr_in addr, from;
std::queue<char*> q_req, q_ans;
pthread_mutex_t mutex_req, mutex_ans;

void handle(int signo){
    printf("Клиент закрылся раньше сервера\n");
    sleep(1);
}

void* getting(void*){
    fcntl(sock, F_SETFL, O_NONBLOCK);
    printf("\nПоток принятия запросов начал работу\n");
    int i = 1;
    char buf[1024];
    socklen_t len;
    while (!flag_g){
        len = sizeof(addr);
        if (recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&addr, &len) < 0){
            perror("recvfrom");
        }
        else{
            printf("Запрос №%i принят\n\n", i);
            pthread_mutex_lock(&mutex_req);
            q_req.push(buf);
            pthread_mutex_unlock(&mutex_req);
            i++;
        }
        sleep(1);
    }
    printf("\nПоток принятия запросов завершил работу\n");
    return 0;
}

void* proccesing(void*){
    printf("\nПоток обработки запросов начал работу\n");
    int val, i = 1;
    char buf[1024];
    while (!flag_s){
        pthread_mutex_lock(&mutex_req);
        if (!q_req.empty()){
            struct stat statbuf;
            stat(q_req.front(), &statbuf);
            q_req.pop();
            pthread_mutex_unlock(&mutex_req);
            
            val = statbuf.st_size;
            std::string msg = std::to_string(val);
            strcpy(buf, msg.c_str());

            pthread_mutex_lock(&mutex_ans);
            q_ans.push(buf);
            pthread_mutex_unlock(&mutex_ans);

            printf("Запрос №%i обработан\nРезультат: %s\n\n", i, buf);
            i++;
        }
        else{
            pthread_mutex_unlock(&mutex_req);
            sleep(1);
        }
    }
    printf("\nПоток обработки запросов завершил работу\n");
    return 0;
}

void* sending(void*){
    printf("\nПоток отправки ответов начал работу\n");
    int i = 1;
    char buf[1024];
    while (!flag_s){
        pthread_mutex_lock(&mutex_ans);
        if (!q_ans.empty()){
            strcpy(buf, q_ans.front());
            q_ans.pop();
            pthread_mutex_unlock(&mutex_ans);
            sendto(sock, buf, 1024, 0, (struct sockaddr *)&addr, sizeof(addr));
            printf("Ответ №%i отправлен\n\n", i);
            i++;
        }
        else{
            pthread_mutex_unlock(&mutex_ans);
            sleep(1);
        }
    }
    printf("\nПоток отправки ответов завершил работу\n");
    return 0;
}

int main()
{
    setlocale(LC_ALL, NULL);
    printf("Сервер начал работу\n");
    signal(SIGPIPE, handle);
    pthread_t id1, id2, id3;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind");
        exit(1);
    }

    pthread_mutex_init(&mutex_req, NULL);
    pthread_mutex_init(&mutex_ans, NULL);

    pthread_create(&id1, NULL, getting, NULL);
    pthread_create(&id2, NULL, proccesing, NULL);
    pthread_create(&id3, NULL, sending, NULL);

    getchar();
    flag_s = 1;
    flag_g = 1;
    flag_p = 1;

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    pthread_join(id3, NULL);

    pthread_mutex_destroy(&mutex_req);
    pthread_mutex_destroy(&mutex_ans);

    printf("\nСервер завершил работу\n");
    return 0;
}
