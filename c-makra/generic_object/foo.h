#ifndef FOO_H
#define FOO_H

#include <object.h>


typedef struct Foo
{
    char a;
    int b;
    unsigned c;
    long d;
} Foo;

Foo* foo_new(char a, int b, unsigned c, long d);
void foo_delete(Foo* foo);
const char* foo_get_name(void);
const char* foo_to_chars(const void* object);

OBJECT_INTERFACE_CHECK_COMPATIBILITY(foo, Foo)

static inline void foo_delete_wrapper(void *foo)
{
    foo_delete(*(Foo **)foo);
}

#endif