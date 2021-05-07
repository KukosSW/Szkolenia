#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct Foo
{
    int x;
    int y;
    int z;
} Foo;

void f1(const int t[], size_t n);
void f2(const int (*t)[4]);
void f3(const Foo* foo);
void f4(const char* day);
void f5(int k);

void f1(const int t[], size_t n)
{
    printf("T[%zu] =", n);
    for (size_t i = 0; i < n; ++i)
        printf("%4d", t[i]);
    printf("\n");
}

void f2(const int (*t)[4])
{
    printf("T[4] = {%d %d %d %d}\n", (*t)[0], (*t)[1], (*t)[2], (*t)[3]);
}

void f3(const Foo* foo)
{
    printf("Foo = {%d %d %d}\n", foo->x, foo->y, foo->z);
}

void f4(const char* day)
{
    printf("DAY = %s\n", day);
}

void f5(int k)
{
    printf("K = %d\n", k);
}

#define DAYS \
    (const char* const []){"pon", "wt", "sr", "czw", "pt", "sob", "nd"}

#define K_SEQ \
    (int[]){100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}

/***** MIN ze sprawdzaniem typu ****/
#define min_normal(a, b) (a) < (b) ? (a) : (b)
#define min(a, b) check_types(a, b) * 0 + (a) < (b) ? (a) : (b)
#define check_types(a, b) (&(a) - &(b))

int main(void)
{
    f1((const int[]){1, 2, 3, 4}, 4);
    f2(&(const int[4]){1, 2, 3, 4});
    f3(&(const Foo){.x = 3, .y = 5, .z = 7});

    f4(DAYS[3]);
    volatile int k = 5;
    f5(K_SEQ[k]);


    /*
        int i = 10;
        double d = 3.14;

        min_normal(i, d) daje tylko warna
        Moge uzyc sztuczki z arytmetyka pointerow i sprawdzac typy
        &a - &b zwraca cokolowiek i nas to nie martwi, ale bedzie  error jesli typy sa zle
        o ile min(i, d) zadziala
        to min(2, 3) juz nie bo nie mozna zrobic &2
        Do tego uzyjemy zmiennych bez imienia
        min((int){2}, (int){3}) Teraz & da sie wykonac wiec git
    */
    printf("MIN(%d, %d) = %d\n", 2, 3, min((int){2}, (int){3}));

    return 0;
}