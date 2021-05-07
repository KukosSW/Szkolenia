#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

void foo(size_t n);
void bar(int** t, size_t n);

void foo(size_t n)
{
    /*
        Potrafimy tylko alokowac 1 strone na raz z asma main.s:
            .L2:
            cmp	rsp, rdx
            je	.L3
            sub	rsp, 4096
            or	QWORD PTR 4088[rsp], 0
            jmp	.L2
     */
    int t[n];

    /* int t[n] = {1, 2, 3}; blad kompilacji */
    /* int t[n] = {0}; blad kompilacji */
    /* static int t[n]; blad kompilacji */

    /* wyswietla sizeof(int) * n, bo to bedzie w runtime */
    printf("SIZE = %zu\n", sizeof(t));
}

/*
    UKLAD w Pamieci
    t = {&t[0], &t[1], ... &t[n - 1]}
    t[0] = {1, 2, 3}
    ...
    t[3] = {3, 4, 5}

    temp = {1, 2, 3, . . ., . . ., 3, 4, 5}
    Tutaj podzial jest tylko wirtualny, pamiec jest przeciez niezmieniona, mamy tylko casta
    T = {{1, 2, 3}, {. . . }, {. . . }, {3, 4, 5}}
*/
void bar(int** t, size_t n)
{
    /*
        Zalozmy ze t bylo alokowane w taki sposob
        int **t = malloc(n * sizeof(*t);
        for (size_t i = 0; i < n; ++i)
            t[i] = malloc(n * sizeof(*t[i]));

        Mamy gdzies kod ktory dziala tak:
        t[1][1]++;

        Chcemy zanuzyc 2D array w 1D array
     */
    int *temp = malloc(n * n * sizeof(*temp));
    t[1 * n + 1]++;
    /*
        Nie moge zrobic temp[1][1]++
        Musze recznie tlumaczyc na t[1 * n + 1]++
     */

    /* Tlumacze temp 1D na wektor tablic [n], dzieki temu moge robic [][], kompi sam zamieni na 1 * n + 1 */
    int (*T)[n] = (int (*)[n])temp;
    T[1][1]++;

    free(temp);
}

int main(void)
{
    foo(1000);
    return 0;
}