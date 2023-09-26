#include <stdio.h>

struct fortest
{
    int i;
    double d;
    char c;
};

//typedef int Elem_t;
//void inline print_elem_t(FILE *stream, Elem_t val) { fprintf(stream, "%d", val); }

typedef fortest Elem_t;
void inline print_elem_t(FILE *stream, Elem_t val) { fprintf(stream, "{i = %d, d = %f, c = %c}", val.i, val.d, val.c); }

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

    stack_push(&stk, {19, 19.19, 'f'});
    //printf("dump3\n");
    //STACK_DUMP(&stk, 0);

    fortest x = {};
    stack_pop(&stk, &x);
    print_elem_t(stdout, x);
    printf("\n");
    //printf("dump4\n");
    //STACK_DUMP(&stk, 0);

    //left canary damaging example
    //*((char *) &stk) = 'f';

    //right data canary damaging example
    //*((long long *)(stk.data + stk.capacity + 1)) = 0xAFAFAFA;

    //invalid pop
    //stack_pop(&stk, &x);

    //right canary damaging example
    //*(((char *) (&stk + 1)) - 1) = 'f';

    stack_push(&stk, {-1, 23.42, 's'});

    stack_dtor(&stk);

    printf("The END!\n");

    return 0;
}
