#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

void f1(int *t);
void f2(int t[]);
void f3(int t[5]);
void f4(size_t n, int t[*]);
void f5(int (*t)[5]);
void f6(int (*t)[5]);
//void f7(const int (*t)[5]);

/* to po co sa te int[*] ? */
void fVla(size_t n, size_t m, int t[*][*]);

/* c11 */
void fC11(int t[static 5]);

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

void f1(int *t)
{
    /* to daje sizeof(int *) */
    printf("F1: SIZE=%zu\n", sizeof(t));
}

void f2(int t[])
{
    /* int t[] przeksztalca sie do int* */
    printf("F2: SIZE=%zu\n", sizeof(t));
}

void f3(int t[5])
{
    /* int t[5] przeksztalca sie do int* */
    printf("F3: SIZE=%zu %d\n", sizeof(t), t[5]);
}

void f4(size_t n, int t[n])
{
    /* OK mamy niby VLA param, ale int[n] przeksztlca sie do int[*] a on dalej do int* */
    printf("F4: SIZE=%zu, %d\n", sizeof(t), t[n]);
}

void f5(int (*t)[5])
{
    /* Tutaj mamy wskaznik do bloku array, wiec sizeof(t) to sizeof(int *[5]) a sizeof(*t) to rozmiar tablic 5 elementowej */
    printf("F5: SIZE=%zu\n", sizeof(*t));
}


void f6(int (*t)[5])
{
    int (*old)[5] = t;

    /* Tutaj mamy wskaznik do bloku array, wiec sizeof(t) to sizeof(int *[5]) a sizeof(*t) to rozmiar tablic 5 elementowej */
    printf("F6: SIZE=%zu\n", sizeof(*t));

    /* int* ktory wskazuje na 5 elemento to nie top samo co int (*)[5], bo skad mamy wiecec ze t1 ma 5 elementow? */
    int *t1 = malloc(sizeof(*t1) * 5);
    t = t1;

    /* To poprawny malloc takiego tworu */
    int (*t2)[5] = malloc(sizeof(*t2));
    for (size_t i = 0; i < 5; ++i)
        (*t2)[i] = 100 + (int)i; /* niestety tutaj t2[i] oznacza ity wektor [5] elemntow, stad *t2 */
    /* samo t jako wskaznik jest przez wartosc, wiec to outside nic nie zmienia */
    t = t2;

    /* *t = *t2 */
    /* Tak jak wyzej to zmiana widoczna na outside, tylko ze nie da sie robic = na array (int [x]), dlatego to bezpieczne */

    t = old;
    /* ofc dalej mozna zmieniac value tablicy */
    for (size_t i = 0; i < 5; ++i)
        (*t)[i] = (*t2)[i];

    free(t1);
    free(t2);
}

/* Teraz juz nie mozna zmieniac, bo const */
// void f7(const int (*t)[5])
// {
//     for (size_t i = 0; i < 5; ++i)
//         (*t)[i] = 200 + (int)i;
// }

void fVla(size_t n, size_t m, int t[n][m])
{
    /* Kompi wie ze to VLA przekazane do funkcji, wiec to nie int** i pamiec lezy obok siebie */
    printf("fVla SIZE=%zu\n", sizeof(t[0]));
    printf("\t&t[0][0] = %p\n", (void *)&t[0][0]);
    printf("\t&t[0][1] = %p(%" PRIdPTR ")\n", (void *)&t[0][1], (intptr_t)&t[0][1] - (intptr_t)&t[0][0]);
    printf("\t&t[1][0] = %p(%" PRIdPTR ")\n", (void *)&t[1][0], (intptr_t)&t[1][0] - (intptr_t)&t[0][0]);
}

void fC11(int t[static 5])
{
    printf("FC11 SIZE=%zu\n", sizeof(t));
}

int main(void)
{
    int t[] = {1, 2, 3, 4, 5};
    f1(t);
    f2(t);
    f3(t);
    volatile size_t size = 10;
    int ttt[size];
    f4(size, ttt);
    f5(&t);

    f6(&t);
    for (size_t i = 0; i < ARRAY_SIZE(t); ++i)
        printf("%4d", t[i]);

    printf("\n");

    /* Jak pilnuje nas kompilator */
    int t2[] = {1, 2};
    int* t3 = malloc(sizeof(*t3) * 100);

    /* int t[5] nic dla kompi nie znaczy, bo on widzi int* */
    f3(t2);
    f3(t3);

    /* Mamy warningi */
    f5(&t2); /* (*)[2] != (*)[5] */
    f5(&t3); /* int ** rozne od (*)[5] */

    /* Taki malloc tez nic nie da */
    int* t4 = malloc(sizeof(*t4) * 5);
    f5(&t4); /* bo malloc jest runtime i still mamy int ** vs (*)[5] */

    /* Taki malloc jest OK */
    int (*t5)[5] = malloc(sizeof(*t5));
    f5(t5);

    free(t3);
    free(t4);
    free(t5);

    /* fVla oczekuje [][] a nie int** */
    int matrix[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    fVla(3, 4, matrix);

    fC11(t);
    fC11(NULL); /* warning */
    fC11(t2); /* warning */

    enum __attribute__((packed)) E
    {
        A,
        B,
        C,
    };

    struct __attribute__((packed, aligned(32)))  S
    {
        int a;
        char b;
        long c;
        char e;
    };

    #define offset(type, member) &(((type *)0)->member)

    printf("ENUM %zu\n", sizeof(enum E));
    printf("STRUCT %zu\n", sizeof(struct S));
    printf("%d %d %d %d\n", offset(struct S, a), offset(struct S, b), offset(struct S, c), offset(struct S, e));

    #define container(ptr, type, member) (type *)((intptr_t)ptr - (intptr_t)offset(type, member))
    struct S2
    {
        int a;
        struct S s;
        char c;
    };

    struct S2 s2 = {.a = 5, .s = {.a = 10, .e = 'C'}, .c = 'K'};
    struct S* s = &s2.s;

    struct S2* ptr = container(s, struct S2, s);
    printf("S2.a = %d\n", ptr->a);

    return 0;
}