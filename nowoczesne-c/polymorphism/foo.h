#ifndef FOO_H
#define FOO_H

#include <dynamic_poly.h>

typedef struct Foo
{
    int a;
    int b;
} Foo;

static inline const char* foo_get_name(void)
{
    return "Foo";
}

void foo_print(const Foo* foo);
Object* foo_create_object(int a, int b);


#endif