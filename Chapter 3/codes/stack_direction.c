#include <stdio.h>
#include <stdbool.h>

bool stack_grows_up(void);

int main(void)
{
    printf("Stack grows %s\n", stack_grows_up() ? "up" : "down");
    return 0;
}

// Version 1. 지역변수를 사용
// bool stack_grows_up(void)
// {
//     int a = 0, b = 1;
//     return &a < &b;
// }

// Version 1b. 
// bool stack_grows_up(void)
// {
//     auto volatile int a = 0, b = 1;
//     return (void *) &a < (void *) &b;
// }

// Version 2. 스택 프레임 활용 (함수 호출)

// void * stack_function(void)
// {
//     auto volatile int b = 0;
//     return (void *) &b;
// }
// bool stack_grows_up(void)
// {
//     auto volatile int a = 1;
//     void *b = stack_function();
//     return (void *) &a < b;
// }

// Version 3. 인라인 어셈블리 활용

bool stack_grows_up(void)
{
    void *prev, *after;

    __asm__ __volatile__ (
        "mov %%rsp, %0;\n"
        "push $0;\n"
        "mov %%rsp, %1;\n"
        "pop %%r8;\n"
        : "=r" (prev), "=r" (after)
        :
        : "%r8"
    );
    return prev < after;
}