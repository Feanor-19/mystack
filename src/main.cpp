#include <stdio.h>

typedef int Elem_t;
#define ELEM_T_SPECF "%d"

#define STACK_DO_DUMP

#include "stack.h"

int main()
{

    Stack stk = {};
    StackErrorCode code = stack_ctor(&stk);

    printf("code: %d\n", code);

    return 0;
}
