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
#define STACK_USE_PROTECTION_CANARY
#define STACK_USE_PROTECTION_HASH
//#define STACK_FULL_DEBUG_INFO

#include "stack.h"

int main()
{

    Stack stk = {};

    printf("----ctor\n");
    stack_ctor(&stk);
    STACK_DUMP(&stk, 0);

    printf("----push1\n");
    stack_push(&stk, {19, 19.19, 'f'});
    STACK_DUMP(&stk, 0);

    printf("----pop1\n");
    fortest x = {};
    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    STACK_DUMP(&stk, 0);

    printf("----push2\n");
    stack_push(&stk, {-1, 23.42, 's'});
    STACK_DUMP(&stk, 0);

    printf("----push3\n");
    stack_push(&stk, {-1, 23.42, 's'});
    STACK_DUMP(&stk, 0);

    printf("----push4\n");
    stack_push(&stk, {-1, 23.42, 's'});
    STACK_DUMP(&stk, 0);

    printf("----push5\n");
    stack_push(&stk, {-1, 23.42, 's'});
    STACK_DUMP(&stk, 0);

    printf("----push6\n");
    stack_push(&stk, {2, 0.01, 'k'});
    STACK_DUMP(&stk, 0);

    printf("----pop2\n");
    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    STACK_DUMP(&stk, 0);

    printf("----pop3\n");
    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    STACK_DUMP(&stk, 0);

    stack_dtor(&stk);

    printf("The END!\n");

    return 0;
}
