#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <locale.h>
#include <signal.h>


int flag_s = 0, flag_g = 0;
int sock;
struct sockaddr_in addr, from;

void handle(int signo){
    printf("Сервер закрылся раньше клиента\n");
    sleep(1);
}

void *sending(void*){
    printf("\nПоток передачи запросов начал работу\n");
    int i = 1;
    while(!flag_s){
        char msg[] = "client.cpp";
        sendto(sock, msg, sizeof(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
        printf("Запрос №%i: найти размер файла (%s) передан\n\n", i, msg);
        i++;
        sleep(1);
    }
    printf("\nПоток передачи запросов завершил работу\n");
    return 0;
}

void *getting(void*){
    fcntl(sock, F_SETFL, O_NONBLOCK);
    printf("\nПоток приема ответов начал работу\n");
    int i = 1;
    char buf[1024];
    while(!flag_s){
        socklen_t len = sizeof(from);
        if (recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &len) < 0){
            perror("recvfrom");
        }
        else{
            printf("Ответ на запрос №%i принят: %s\n\n", i, buf);
            i++;
        }
        sleep(1);
    }
    printf("\nПоток приема ответов завершил работу\n");
    return 0;
}

int main()
{
    setlocale(LC_ALL, NULL);
    printf("Клиент начал работу\n");
    signal(SIGPIPE, handle);

    pthread_t id1, id2;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    pthread_create(&id1, NULL, &sending, NULL);
    pthread_create(&id2, NULL, &getting, NULL);

    getchar();
    flag_s = 1;
    flag_g = 1;

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    socklen_t len = sizeof(addr);
    getsockname(sock, (struct sockaddr *)&addr, &len);
    printf("\nclient port: %d\n", ntohs(addr.sin_port));

    close(sock);
    printf("\nКлиент завершил работу\n");
    return 0;
}
