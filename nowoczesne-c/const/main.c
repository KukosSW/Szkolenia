#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Foo
{
    int a;
    const int b;
    const char* const name;
};

static void func1(void);
static void func2(void);

/*
    Blagam nie piszcie tutaj const int a, const int t[const]
    To tylko wpowadza zamieszanie, ludzie sie zaczynaja zastanawiac o co kaman,
    czy musze pisac const aby sie nie zmienilo na zewnatrz

    const struct Foo* jest super, bo to pokazuje ze mam read only struct
 */
static void func3(const int t[], const struct Foo* foo, int a);

/* Mozna tez zwracac consty */
static const char* func4(const struct Foo* foo);

static void func1(void)
{
    int var1 = 5;
    var1++; /* moge bo to nie const */

    const int var2 = 5;
    /* var2++; error bo przeciez to const */

    /* const to nie constexpr */
    int t[var2]; /* VLA a nie fixed size array */
    (void)t;

    /* mamy ptr do inta ktory wskazuje na inta OK! */
    int* const ptr1 = &var1;
    (*ptr1)++; /* ok bo moge zmieniac var1 */
    /* ptr++; / error bo ptr jest const  */

    /* ptr1 = &var2; nie moze nieconst wskazywac na consta, bo przeciez (*ptr1)++ dziala */

    const int* ptr2;
    ptr2 = &var1; /* const moze wskazywac na nieconsta jesli to primitive value */
    /* (*ptr2)++; chociaz var1 jest modyfikowalne, to ptr2 wskazuje na const int, wiec tak czy siak nie wolno zmieniac */
    ptr2 = &var2;
    ptr2++;

    /* dozwolone tylko *ptr3 */
    const int* const ptr3 = &var2;
    (void)ptr3;

    char *str = NULL;
    /* const char** str_p = &str; lipa const char** nie moze wskazywac na char*, bo to 2 poziomy abstrakcji */
    char* const* str_p = &str; /* ale moge  tak jak poprzednio wskazywac na nie consta o 1 poziom abstracko nizej */
    (void)str_p;

    /* Jak zmienic consta w C? */
    const int var3 = 10;
    int* ptr4 = (int *)&var3;
    *ptr4 = 15;
    /* MOZLIWE ZE DALEJ 10, DLACZEGO???? */
    printf("VAR3 = %d\n", var3);

    /* Kompilator wie, ze const jest nie zmienny wiec moze je wczytac do rejestru i nie  robic update */
    const volatile int var4 = 20;
    ptr4 = (int *)&var4;
    *ptr4 = 100;
    /* Tutaj jest jest 100 */
    printf("VAR4 = %d\n", var4);

    /* "Ala ma kota" musi byc const char*, bo zmiana tego literalu to SeqFault, bo on siedzie w.rodata */
    const char* str2 = "Ala ma kota";
    (void)str2;
}

static void func2(void)
{
    /*
        Tak nie wolno, bo consty sie tylko raz przypisuje, alokujac na stosie zostaly przypisane wartoscia ze stosu
        struct Foo f1;
        f1.a = 10;
        f1.b = 15;
        f1.name = "Foo1";
    */
    struct Foo f1 = {.a = 5, .b = 10, .name = "Foo1"};
    (void)f1;
    /*
        Consty ladne nas bronia przed nadpisaniem
        f1.b = 15;
        f1.name++;
        f1.name[0]++;
    */

    struct Foo f2 = {.a = 10, .b = 15, .name = "Foo2"};
    /* f1 = f2; tak sie nie da, bo nadpisujemy consty */

    /* Tak mozna nadpisac, bo memcpy castuje to void* i nie wie ze to struct z constami */
    memcpy(&f1, &f2, sizeof(f1));
    printf("Foo1 = {%d, %d, %s}\n", f1.a, f1.b, f1.name);

    /*
        Tak tez w sumie lipa, bo nie da sie po mallocu nadpisac consta
        struct Foo* f3 = malloc(sizeof(*f3));
        f3->a = 10;
        f3->b = 15;
        f3->name = "Foo3";
    */
}

/* Tutaj moge wpisac sobie const int t[const] pokazujac ze nie chce robic t++ */
static void func3(const int t[const], const struct Foo* foo, const int a)
{
    /* t++; error mam t[const] */
    const int _a = a; /* tak w sumie dziala const int a jako param */
    (void)t;
    (void)foo;
    (void)_a;
}

static const char* func4(const struct Foo* foo)
{
    return foo->name;
}

int main(void)
{
    func1();
    func2();
    func3((int[]){1, 2, 3}, &(const struct Foo){.a = 5, .b = 10, .name = "FOO"}, 10);

    const char* name1 = func4(&(const struct Foo){.a = 5, .b = 10, .name = "FOO"});
    (void)name1;
    return 0;
}