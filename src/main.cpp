#include <stdio.h>

typedef int Elem_t;
void inline print_elem_t(FILE *stream, Elem_t val) { fprintf(stream, "%d", val); }

#define STACK_DO_DUMP
#define STACK_USE_POISON
#define STACK_ABORT_ON_DUMP
#define STACK_DUMP_ON_INVALID_POP
#define STACK_USE_PROTECTION_CANARY

#include "stack.h"

int main()
{
    Stack stk = {};
    //printf("dump1\n");
    //STACK_DUMP(&stk, 0);

    stack_ctor(&stk);
    //printf("dump2\n");
    //STACK_DUMP(&stk, 0);

    stack_push(&stk, 19);
    //printf("dump3\n");
    //STACK_DUMP(&stk, 0);

    int x = 0;
    stack_pop(&stk, &x);
    //printf("dump4\n");
    //STACK_DUMP(&stk, 0);

    //left canary damaging example
    *((char *) &stk) = 'f';

    //right data canary damaging example
    *((long long *)(stk.data + stk.capacity + 1)) = 0xAFAFAFA;

    stack_pop(&stk, &x);

    //right canary damaging example
    //*(((char *) (&stk + 1)) - 1) = 'f';

    stack_push(&stk, 42);

    stack_dtor(&stk);

    printf("The END!\n");

    return 0;
}
