#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <fcntl.h>
#include <ucontext.h>
#include <queue>
#include <malloc.h>

#define SIZE 16384

ucontext_t *uctx_func1, *uctx_func2, *uctx_disp, uctx_main, *uctx_tmp;
std::queue<ucontext_t*> readyList;

void func1(void){
    while(1){
        printf("Выполнение func1\n");
        sleep(1);
        swapcontext(uctx_func1, uctx_disp);
    }
}

void func2(void){
    while(1){
        printf("Выполнение func2\n");
        int ch = getchar();
        sleep(1);
        if (ch == 10){
            swapcontext(uctx_func2, &uctx_main);
        }
        else{
            swapcontext(uctx_func2, uctx_disp);
        }
    }
}

void dispatcher(void){
    ucontext_t uctx_old;
    while(1){
        printf("\nДиспетчер передает управление...\n\n");
        readyList.push(uctx_tmp);
        uctx_old = *uctx_tmp;
        uctx_tmp = readyList.front();
        readyList.pop();
        swapcontext(&uctx_old, uctx_tmp);
    }
}

void create_uctx(ucontext_t* a, char * stack, void (*f)()){
     getcontext(a);
     a->uc_stack.ss_sp = stack;
     a->uc_stack.ss_size = sizeof(stack);
     makecontext(a, f, 0);
}

int main()
 {
     setlocale(LC_ALL, NULL);
     printf("Программа начала работу\n\n");
     int flags = fcntl(STDIN_FILENO, F_GETFL);
     fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

     char *func1_stack, *func2_stack, *disp_stack;
     func1_stack = (char*)malloc(SIZE);
     func2_stack = (char*)malloc(SIZE);
     disp_stack = (char*)malloc(SIZE);

     uctx_func1 = (ucontext_t*)malloc(sizeof(ucontext_t));
     uctx_func2 = (ucontext_t*)malloc(sizeof(ucontext_t));
     uctx_disp = (ucontext_t*)malloc(sizeof(ucontext_t));

     create_uctx(uctx_func1, func1_stack, func1);
     create_uctx(uctx_func2, func2_stack, func2);

     readyList.push(uctx_func2);

     create_uctx(uctx_disp, disp_stack, dispatcher);

     getcontext(&uctx_main);
     uctx_tmp = uctx_func1;
     swapcontext(&uctx_main, uctx_func1);
     
     /*
     free(func1_stack);
     free(func2_stack);
     free(disp_stack);
     */

     free(uctx_func1);
     free(uctx_func2);
     free(uctx_disp);
     
     fcntl(STDIN_FILENO, F_SETFL, flags);
     printf("\nПрограмма завершила работу\n");
     return 0;
 }