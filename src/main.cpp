#include <stdio.h>

typedef int Elem_t;
#define ELEM_T_SPECF "%d"

#define STACK_DO_DUMP
#define STACK_USE_POISON
#define STACK_EXIT_ON_DUMP
#define STACK_DUMP_ON_INVALID_POP
#define STACK_USE_PROTECTION_CANARY

#include "stack.h"

int main()
{
    Stack stk = {};

    stack_ctor(&stk);

    stack_push(&stk, 19);

    int x = 0;
    stack_pop(&stk, &x);

    //left canary damaging example
    //*((char *) &stk) = 'f';

    stack_pop(&stk, &x);

    //right canary damaging example
    //*(((char *) (&stk + 1)) - 1) = 'f';

    stack_push(&stk, 42);

    stack_dtor(&stk);

    return 0;
}
