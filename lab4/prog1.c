#include <stdio.h>
#include <locale.h>
#include <sys/types.h>
#include <unistd.h>

 int main(int argc, char* argv[])
 {
     setlocale(LC_ALL, NULL);
     printf("\nПрограмма 1 начала работу\n");
     printf("ID текущего процесса: %d\n", getpid());
     printf("ID процесса-родителя: %d\n\n", getppid());

     for (int i = 0; i < argc; i++){
        printf("%s\n", argv[i]);
        fflush(stdout);
        sleep(1);
     }
     printf("\nПрограмма 1 завершила работу\n");
     return 9;
 }
