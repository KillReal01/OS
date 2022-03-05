#include <stdio.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
 {
     setlocale(LC_ALL, NULL);
     printf("Программа 2 начала работу\n");
     pid_t rv;
     pid_t pid = fork();
     int status;
     char *const argv[] = {"hello", "Kirill", NULL};

     if (pid == 0){
         execv("prog1", argv);
     }
     else if (pid > 0){
         printf("ID текущего процесса: %d\n", getpid());
         printf("ID дочернего процесса: %d\n", pid);
         printf("ID процесса-родителя: %d\n\n", getppid());
         while (rv = waitpid(pid, &status, WNOHANG) == 0){
             printf("Ждем\n");
             usleep(500000);
         }
         if (rv == -1){
             printf("Ошибка waitpid\n");
         }
         else{
             printf("Код завершения дочернего процесса: %d\n", status);
         }
     }
     else{
         perror("fork");
     }
     printf("\nПрограмма 2 завершила работу\n");
     return 0;
 }
