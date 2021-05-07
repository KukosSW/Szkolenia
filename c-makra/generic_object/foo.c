#include <foo.h>
#include <stdlib.h>

Foo* foo_new(char a, int b, unsigned c, long d)
{
    Foo* foo;
    foo = malloc(sizeof(*foo));
    foo->a = a;
    foo->b = b;
    foo->c = c;
    foo->d = d;

    return foo;
}

void foo_delete(Foo* foo)
{
    free(foo);
}

const char* foo_get_name(void)
{
    return "Foo";
}

const char* foo_to_chars(const void* object)
{
    /* To be implemented */
    (void)object;
    return "";
}