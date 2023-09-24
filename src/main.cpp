#include <stdio.h>

//typedef int Elem_t;
//#define ELEM_T_SPECF "%d"

#define STACK_DO_DUMP

#include "stack.h"

int main()
{

    Stack stk = {};
    StackErrorCode code;

    code = stack_ctor(&stk);
    printf("ctor code = %d\n", code);
    STACK_DUMP(&stk, 0);

    code = stack_push(&stk, 19);
    printf("push1 code = %d\n", code);
    STACK_DUMP(&stk, 0);

    code = stack_push(&stk, 23);
    printf("push2 code = %d\n", code);
    STACK_DUMP(&stk, 0);

    int x;
    code = stack_pop(&stk, &x);
    printf("pop1 code = %d\n", code);
    printf("popped: %d\n", x);
    STACK_DUMP(&stk, 0);

    code = stack_push(&stk, -19);
    printf("push3 code = %d\n", code);
    STACK_DUMP(&stk, 0);

    code = stack_dtor(&stk);
    printf("dtor code = %d\n", code);
    STACK_DUMP(&stk, 0);

    return 0;
}
