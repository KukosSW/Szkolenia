#ifndef NORMAL_H
#define NORMAL_H

#include <stdio.h>

typedef struct FooNormal
{
    int a;
    int b;
    int c;
    int d;
} FooNormal;

static inline void print_fooNormal(const FooNormal* foo)
{
    printf("Foo={%d %d %d %d}\n", foo->a, foo->b, foo->c, foo->d);
}

#endif