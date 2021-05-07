#include <stdio.h>

/* Pierwszy sposob na zapisanie trybow pracy */
#define DMODE_1 1
#define DMODE_2 2
#define DMODE_3 3

/* Teraz tak, mozna uzyc jakiegos inta, unsigned jako typu, najlepiej zrobic nowy typ */
typedef unsigned mode_d; /* mode_defined */


void foo_d(mode_d mode);

void foo_d(mode_d mode)
{
    printf("DMODE = %d\t", mode);

    switch (mode)
    {
        case DMODE_1:
        case DMODE_2:
        case DMODE_3:
            printf("jest OK!\n");
            break;
        default:
            printf("jest nieprawidlowy!\n");
    }
}


/* Drugi sposob to enum */
typedef enum EMODE
{
    EMODE_1 = 1,
    EMODE_2 = 2,
    EMODE_3 = 3,
} mode_e;

void foo_e(mode_e mode);

void foo_e(mode_e mode)
{
    printf("EMODE = %d\t", mode);

    switch (mode)
    {
        case EMODE_1:
        case EMODE_2:
        case EMODE_3:
            printf("jest OK!\n");
            break;
        default:
            printf("jest nieprawidlowy!\n");
    }
}

/* Jak dziala enum? */
enum MYENUM
{
    VAL0, /* z automatu enum = 0 */
    VAL1, /* kazdy kolejny to poprzedni + 1 */
    VAL5 = 5, /* mozna przypisac dowolna wartosc */
    VAL00 = VAL0, /* nawet taka jaka juz byla */
    VALNEG = -1, /* enum to int, wiec negative tez ok */
    VAL000, /* nast to poprzedni + 1, wiec -1 + 1 znowu 0 */
};

void myenum_print(void);

void myenum_print(void)
{
    printf("VAL0   = %d\n", VAL0);
    printf("VAL1   = %d\n", VAL1);
    printf("VAL5   = %d\n", VAL5);
    printf("VAL00  = %d\n", VAL00);
    printf("VALNEG = %d\n", VALNEG);
    printf("VAL000 = %d\n", VAL000);

    /*
        jaki jest size enuma?
        Standard mowi jasno. enum to int.
        Niektore kompilatory robia tak, ze patrza na max value i dostosowuja enuma
        gcc tez tak robi
     */
    printf("SIZE   = %zu\n", sizeof(enum MYENUM));
}

int main(void)
{
    foo_d(DMODE_1); /* OK */
    foo_d(100); /* tez ok, bo mode_d to unsigned */

    foo_e(EMODE_1); /* OK */
    foo_e(100); /* tez ok, wartosc nie jest enumem, ale enum to tak naprawde int */

    // enum E{A, B, C};
    // enum E e = 10;
    // mode_e m = e; /* warn nie ten enum */
    // foo_e(e); /* warn nie ten enum */
    // foo_e(m);

    myenum_print();


    /*
        Czego nie da sie zrobic enumem?
        Ano nie da sie laczyc wartosci legitnie
        Tzn chce miec mozliwosc
        DMODE1 i DMODE2 rownoczesnie
        Robie   DMODE1 = 1 << 0
                DMODE2 = 1 << 2
        mode_d m = DMODE1 | DMODE2
        W enumie to niezgodnosc typu, kompi C to przyjmie, C++ / lint juz nie

        Ofc nie da sie tez wyjsc poza rozmiar int. Kompilatory na to pozwalaja
        ale lepiej trzymac sie stdC
     */
    return 0;
}
