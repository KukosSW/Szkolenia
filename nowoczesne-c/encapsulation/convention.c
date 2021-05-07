#include <convention.h>
#include <stdio.h>

void fooconv_print(const FooConv* foo)
{
    printf("Foo={%d %d %d %d}\n", foo->a, foo->__b, foo->__c, foo->__d);
}