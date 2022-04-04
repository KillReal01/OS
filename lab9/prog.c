#include <stdio.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <stdlib.h>


#define STACK_SIZE (1024*1024)

static int childFunc(void*){
    printf("Дочерний процесс начал работу\n\n");
    printf("ID текущего процесса: %d\n", getpid());
    printf("ID процесса-родителя: %d\n", getppid());
    printf("\nДочерний процесс завершил работу\n\n");
    return 0;
}

int main()
 {
     setlocale(LC_ALL, NULL);
     printf("Родительский процесс начал работу\n\n");

     char* stack;
     char* stackTop;
     stack = (char*)malloc(STACK_SIZE);
     stackTop = stack + STACK_SIZE;

     pid_t child_pid = clone(childFunc, stackTop, CLONE_NEWPID | SIGCHLD, NULL);
     if (child_pid == -1){
         perror("clone");
         exit(0);
     }
    
     printf("ID текущего процесса: %d\n", getpid());
     printf("ID дочернего процесса: %d\n", child_pid);
     printf("ID процесса-родителя: %d\n\n", getppid());

     int status;
     waitpid(child_pid, &status, 0);

     printf("ID текущего процесса: %d\n", getpid());
     printf("ID дочернего процесса: %d\n", child_pid);
     printf("ID процесса-родителя: %d\n", getppid());
 
     printf("\nРодительский процесс завершил работу\n");
     return 0;
 }
