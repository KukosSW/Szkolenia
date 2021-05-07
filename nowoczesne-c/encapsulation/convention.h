#ifndef CONVENTION_H
#define CONVENTION_H

/*
    Jest taka ciekawa konwencja
    Jesli zmienna nie ma __ to jest public, pewnie beda get/set ale mozna jej uzywac normalnie . ->
    Jesli zmienna ma __ to znaczy ze jest private i nie mowinno jej sie uzywac przez . ->

    Zaleta jest taka ze mam get set inline i mamy na stacku
    Wada ze to tylko umowa, kompi nas nie obroni
*/
typedef struct FooConv
{
    int a;
    int __b;
    int __c;
    int __d;
} FooConv;

static inline int fooconv_get_a(const FooConv* foo)
{
    return foo->a;
}

static inline void fooconv_set_a(FooConv* foo, int a)
{
    foo->a = a;
}

static inline int fooconv_get_b(const FooConv* foo)
{
    return foo->__b;
}

static inline void fooconv_set_b(FooConv* foo, int b)
{
    foo->__b = b;
}

static inline int fooconv_get_c(const FooConv* foo)
{
    return foo->__c;
}

static inline void fooconv_set_c(FooConv* foo, int c)
{
    foo->__c = c;
}

static inline int fooconv_get_d(const FooConv* foo)
{
    return foo->__d;
}

static inline void fooconv_set_d(FooConv* foo, int d)
{
    foo->__d = d;
}

void fooconv_print(const FooConv* foo);

#endif