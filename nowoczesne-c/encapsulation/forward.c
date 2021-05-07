#include <forward.h>

/* Teraz ta zaleznosc jest ladnie w srodku, nie kazdy musi tez includowac stdio */
#include <stdio.h>
#include <stdlib.h>


/* deklaracja struktury jest tutaj */
struct FooForward
{
    int a;
    int b;
    int c;
    int d;
};

FooForward* fooforward_create(int a, int b, int c, int d)
{
    FooForward* foo;
    foo = malloc(sizeof(*foo));
    if (foo == NULL)
        return NULL;

    foo->a = a;
    foo->b = b;
    foo->c = c;
    foo->d = d;

    return foo;
}

void fooforward_destroy(FooForward* foo)
{
    free(foo);
}

int fooforward_get_a(const FooForward* foo)
{
    return foo->a;
}

int fooforward_get_b(const FooForward* foo)
{
    return foo->b;
}

int fooforward_get_c(const FooForward* foo)
{
    return foo->c;
}

int fooforward_get_d(const FooForward* foo)
{
    return foo->d;
}

void fooforward_set_a(FooForward* foo, int a)
{
    foo->a = a;
}

void fooforward_set_b(FooForward* foo, int b)
{
    foo->b = b;
}

void fooforward_set_c(FooForward* foo, int c)
{
    foo->c = c;
}

void fooforward_set_d(FooForward* foo, int d)
{
    foo->d = d;
}

void fooforward_print(const FooForward* foo)
{
    printf("Foo={%d %d %d %d}\n", foo->a, foo->b, foo->c, foo->d);
}

size_t fooforward_sizeof(void)
{
    return sizeof(FooForward);
}