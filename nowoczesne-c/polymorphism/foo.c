#include <foo.h>
#include <stdio.h>
#include <stdlib.h>

static void foo_print_wrapper(const void* foo)
{
    foo_print(foo);
}

static void foo_delete_wrapper(void* foo)
{
    free(foo);
}

void foo_print(const Foo* foo)
{
    printf("Foo = {%d, %d}\n", foo->a, foo->b);
}

Object* foo_create_object(int a, int b)
{
    Foo* foo = malloc(sizeof(*foo));
    foo->a = a;
    foo->b = b;

    return object_create(foo, foo_get_name, foo_print_wrapper, foo_delete_wrapper);
}