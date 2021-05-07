#include <stdio.h>
#include <time.h>
#include <object.h>
#include <foo.h>

/*
    primitives --> patenciaki na wyluskanie z typedef primitives
    object-private --> zbior funkcji do interfejsu aby zaimplementowac object
    object-primitives --> zbior funkcji na primitywnych typow
    object-char ---> zbior funkcji dla char
    object --> interfejs dla usera, ma juz za darmo primitives jak i makra do objectu


            object-private                          object-private
                |                                         |
        object-primitives  <--- primitives                |
                |                                         |
    object-char, object-int, object-ptr                   / object-legacy
                | ---------------------------------------
              object
                |
        Twoje objecty jak Foo
*/


int main(void)
{
    int a;
    char b;
    unsigned int c;
    typedef int myint_t;
    myint_t d;
    enum {A, B, C} e;
    Foo foo;
    Foo *ptr = foo_new('A', 10, 12u, 100);
    struct tm tm;
    Object* t[] = {
        object_primitive_new(a),
        object_primitive_new(b),
        object_primitive_new(c),
        object_primitive_new(d),
        object_primitive_new(e),
        object_non_primitive_new(foo, foo),
        object_ptr_new(ptr, foo_delete_wrapper),
        object_legacy_new(tm)
    };

    for (size_t i = 0; i < sizeof(t) / sizeof(t[0]); ++i)
    {
        printf("Obiekt[%zu] to: %s\n", i, object_get_name(t[i]));
        object_delete(t[i]);
    }

    return 0;
}