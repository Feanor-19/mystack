#include <stdio.h>

typedef int Elem_t;
#define ELEM_T_SPECF "%d"

#define STACK_DO_DUMP
#define STACK_USE_POISON
#define EXIT_ON_DUMP

#include "stack.h"

int main()
{

    Stack stk = {};
    StackErrorCode code;

    stack_ctor(&stk);

    stack_push(&stk, 19);

    stack_push(&stk, 23);

    STACK_DUMP(&stk, 0);

    stack_dtor(&stk);

    return 0;
}
