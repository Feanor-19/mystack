#include <stdio.h>


struct fortest
{
    int i;
    double d;
    char c;
};


typedef fortest Elem_t;
void inline print_elem_t(FILE *stream, Elem_t val) { fprintf(stream, "{i = %d, d = %f, c = %c}", val.i, val.d, val.c); }

#define STACK_DO_DUMP
#define STACK_USE_POISON
//#define STACK_ABORT_ON_DUMP
#define STACK_DUMP_ON_INVALID_POP
//#define STACK_USE_PROTECTION_CANARY
//#define STACK_USE_PROTECTION_HASH

#include "stack.h"


int main()
{
    Stack stk = {};

    stack_ctor(&stk);
    printf("----ctor\n");
    STACK_DUMP(&stk, 0);

    stack_push(&stk, {19, 19.19, 'f'});
    printf("----push1\n");
    STACK_DUMP(&stk, 0);

    fortest x = {};
    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    printf("----pop1\n");
    STACK_DUMP(&stk, 0);

    stack_push(&stk, {-1, 23.42, 's'});
    printf("----push2\n");
    STACK_DUMP(&stk, 0);

    stack_push(&stk, {-1, 23.42, 's'});
    printf("----push3\n");
    STACK_DUMP(&stk, 0);

    stack_push(&stk, {-1, 23.42, 's'});
    printf("----push4\n");
    STACK_DUMP(&stk, 0);


    stack_push(&stk, {-1, 23.42, 's'});
    printf("----push5\n");
    STACK_DUMP(&stk, 0);

    stack_push(&stk, {2, 0.01, 'k'});
    printf("----push6\n");
    STACK_DUMP(&stk, 0);

    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    printf("----pop2\n");
    STACK_DUMP(&stk, 0);

    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    printf("----pop3\n");
    STACK_DUMP(&stk, 0);

    stack_dtor(&stk);

    printf("The END!\n");

    return 0;
}
