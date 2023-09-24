#ifndef TEST_H
#define TEST_H

#include <stdio.h>

static int f(int x);

//---

int f(int x)
{
    printf("in f: x: %d\n", x);
    return x + 19;
}

#endif
