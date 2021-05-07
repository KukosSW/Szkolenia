#include <stdio.h>
#include <vaargs.h>

/*
    Zmienna liczba argumentow czesto sluzy do przeslaniania funkcji

    Np mamy w linuxie dwie funkcje ktore maja taka sama nazwe ale inne argumenty:

    int open(const char *pathname, int flags);
    int open(const char *pathname, int flags, mode_t mode);

    W rzeczywistosci to:
    int open(const char *pathname, int flags, ...);
    Jesli we flagach mamy O_CREAT to wiemy ze ... nie sa puste i zawieraja dokladnie 1 argument
*/

int main(void)
{
    printf("Suma = %d\n", sum(5, 1, 2, 3, 4, 5));

    printf_simply("Ala ma kota\n");
    printf_simply("Kolejny print int %d, long %l char %c ptr %p\n", (int)1, (long)2, 'C', (void *)0xdeadbeef);

    sum_with_debug(5, 1, 2, 3, 4, 5);

    struct_passed_to_valist(3, (Foo){1, 2}, (Foo){3, 4}, (Foo){5, 6});

    return 0;
}