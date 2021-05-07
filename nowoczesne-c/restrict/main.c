#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/************** JAK KOMPILATOR OPTYMALIZUJE RESTRICT ********************************/

/*
    Tutaj wie, ze bedzie store load, wiec zmienna a nie jest wczytywana, dziala na wolnej pamieci :(

	add	DWORD PTR [rdi], 5
	mov	eax, DWORD PTR [rsi]
	add	eax, 5
	mov	DWORD PTR [rsi], eax
	add	eax, DWORD PTR [rdi]
*/
int sum5(int* a, int* b)
{
    *a += 5;
    *b += 5;

    return *a + *b;
}

/*
    Tutaj wie, ze moze sobie pozwolic na load a, load b, add a, 5 add b, 5 add a, b ...
    Czyli dziala na rejestrach bez doczytywania tego samego kilka razy bo nie ma side effectow

	mov	eax, DWORD PTR [rdi]
	mov	ecx, DWORD PTR [rsi]
	add	eax, 5
	lea	edx, 5[rcx]
	mov	DWORD PTR [rdi], eax
	add	eax, edx
	mov	DWORD PTR [rsi], edx

*/
int sum5R(int* restrict a, int* restrict b)
{
    *a += 5;
    *b += 5;

    return *a + *b;
}

/*
    Jesli a == b wtedy return 12 else return 11

	mov	DWORD PTR [rdi], 5
	mov	DWORD PTR [rsi], 6
	mov	eax, DWORD PTR [rdi]
	add	eax, 6
*/
int sum11czy12(int* a, int* b)
{
    *a = 5;
    *b = 6;

    return *a + *b;
}

/*
    Nie ma side effectow, zawsze return 11

    mov	DWORD PTR [rdi], 5
	mov	eax, 11
	mov	DWORD PTR [rsi], 6
*/
int sum11(int* restrict a, int* restrict b)
{
    *a = 5;
    *b = 6;

    return *a + *b;
}

/*************************************************************************************************/

/* te same typy restrict potrzebny */
void f1(int* restrict a, int* restrict b)
{

}

/* inne typy kompilator wie ze to restrict by default */
void f2(int* a, double* b)
{

}

/* inne typy, ale char* jest kompatybilny z innymi typami wiec musimy decydowac czy restrict czy nie */
void f3(int* restrict a, char* restrict b)
{

}

/* void* nie da sie dereferencjowac, ale co jesli beda casty? Lepiej dac restrict skoro sa kompatybilne */
void f4(int* restrict a, void* restrict b)
{

}

void f5(int* restrict a, int* restrict b)
{
    /* Kompialtor zna kontekst _a i _b wiec to tylko dla naszej wygody, bedzie wiedzial ze to restrict */
    int* _a = a;
    int* _b = b;

    (void)_a;
    (void)_b;
}

void f6(int* a, int* b)
{
    /* NOK, a i b moga byc tym samym wiec nie wiemy czy _a i _b nie sa tym samym */
    int* restrict _a = a;
    int* restrict _b = b;

    (void)_a;
    (void)_b;

    /* Ubecja, skoro sa restrict to nie moga zostac przypisane na to samo, kompilator decyduje co zrobic */
    _a = _b;
}

/*
    Block scope
    Pointery ogolnie moga byc nie restrict
    ale na ten moment bedziemy sie nimi tak poslugiwac ze beda restrict
    np t[100]; a = &t[0], b = &t[50]; robimy jakas petle 50 razy
*/
#define f1_MACRO(a, b) \
    do { \
        int* restrict ____a = (a); \
        int* restrict ____b = (b); \
        \
    } while (0)

/*
    Zawsze ptr1 ma pamiec na wylacznosc, nie tylko w bloku, funkcji itp
    to jest file global scope wiec ZAWSZE
*/
int* restrict ptr1;
int* restrict ptr2;
size_t array_size;

void sumup(int dst[restrict], int src1[restrict], int src2[restrict], size_t n)
{
    for (size_t i = 0; i < n; ++i)
        dst[i] = src1[i] + src2[i];
}

typedef struct Foo
{
    int* restrict a;
    int* restrict b;
} Foo;

typedef struct Bar
{
    int t[100];
} Bar;

void fFoo(Foo* foo1, Foo* foo2)
{
    /*
        Restrict w strukturach to nie tylko scope struktury
        tzn foo1->a != foo1->b
        tutaj chodzi o to, ze w momencie gdy struktura jest wykorzystywana
        to pointer restrict jest jedyny do danego obiektu
        Nie tylko foo1->a != foo1->b
        ale takze
        foo1->a != foo2->a i foo1->b != foo2->b
        dodatkowo
        foo1->a != foo2->b i foo1->b != foo1->a
        Czyli kazdy jest jedyny
    */
}

void fBar(Bar* restrict bar1, Bar* restrict bar2, int* restrict t)
{
    /*
        caly obszar pamieci bar1 jest jedyny i bar2 jedyny i t jedyne
        Wiec bar1->t != bar2->t != t
        Chociaz bar->t nie jest restrict jako taki, ale skoro caly ptr do Bar jest
        to wszyskie inne tez sa
     */
}

int main(void)
{
    /* OK, bo malloc zwraca restrict pointer dzieki atrybutowi */
    int* t = malloc(100 * sizeof(*t));
    ptr1 = t;
    ptr2 = t + 50;
    array_size = 50;
    free(t);


    int dst[10];
    int src1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int src2[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    /* OK, bo nikt nie wchodzi na nikogo */
    sumup(dst, src1, src2, 10);

    /* NOK we wzgledu na restrict, ale OK ze wzgledu na kod w sumup, nic sie nie zepsuje */
    sumup(dst, src1, src1, 10);

    /* NOK, bo src1 i src1 to to samo */
    sumup(src1, src1, src2, 10);

    /*
        OK, src1 + 5 w petli bedzie src1 + {5, 6, 7, 8, 9}
        a src1 bedzie src1 + {0, 1, 2, 3, 4}
        Wiec sie nie spotkaja, nie ma overlapa wiec OK
    */
    sumup(src1 + 5, src1, src2, 5);

    return 0;
}