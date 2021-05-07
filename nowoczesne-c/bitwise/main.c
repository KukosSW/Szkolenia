#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>

#define BIT_MASK(s, e)           (((1u << ((e) - (s) + 1)) - 1) << (s))
#define BIT_GET_MASK(n, s, e)    (((n) & (BIT_MASK(s, e))) >> s)
#define BIT_SET_MASK(n, s, e)    ((n) | (BIT_MASK(s, e)))
#define BIT_CLEAR_MASK(n, s, e)  ((n) & ~(BIT_MASK(s, e)))
#define BIT_TOGGLE_MASK(n, s, e) ((n) ^ (BIT_MASK(s, e)))

/*
#define BIT_N(n)         (1u << (n))
#define BIT_GET(n, k)    (((n) & BIT_N(k)) >> (k))
#define BIT_SET(n, k)    ((n) | (BIT_N(k)))
#define BIT_CLEAR(n, k)  ((n) & ~(BIT_N(k)))
#define BIT_TOGGLE(n, k) ((n) ^ (BIT_N(k)))
*/

#define BIT_N(n)         BIT_MASK(n, n)
#define BIT_GET(n, k)    BIT_GET_MASK(n, k, k)
#define BIT_SET(n, k)    BIT_SET_MASK(n, k, k)
#define BIT_CLEAR(n, k)  BIT_CLEAR_MASK(n, k, k)
#define BIT_TOGGLE(n, k) BIT_TOGGLE_MASK(n, k, k)

static void bit_print(unsigned int a);

static void bitwise_defined(void);
static void bit_macros(void);
static void bit_endianness(void);
static void bit_shifts(void);
static void bit_fields(void);

static void bit_print(unsigned int a)
{
    /* wpisanie 0 charow do nulla jest OK :) dzieki temu dostane liczbe charow potrzebnych na moj string */
    int len = snprintf(NULL, 0, "%u (%X)", a, a);

    printf("%u (%X)%*s= ", a, a, 30 - len, " ");
    for (int i = sizeof(a) * CHAR_BIT - 1; i >= 0; --i)
    {
        putchar('0' + (int)BIT_GET(a, i));
        if (i % 8 == 0)
            putchar(' ');
    }
    putchar('\n');
}

static void bit_fields(void)
{
    struct S
    {
        uint32_t x:3;
        uint32_t y:10;
        uint32_t pad:32-13;
    } s = {0};

    /* ISO C99 6.7.7 Type definitions 6 */
    struct N
    {
        int32_t x:3;
        int32_t  :0; /* x i y to inne inty */
        int32_t y:10;
        int32_t pad:32-10;
    } n = {0};

    s.x = 6;
    n.x = -3;
    printf("s.x = %u (%X), us.x = %d (%X)\n", s.x, s.x, n.x, n.x);

    s.x = 100; /* UBecja, nie wiadomo co sie stanie */
    n.x = 6; /* UB, przeciez signed nie ma [0:7], bo 1 bit to signed */
    printf("s.x = %u (%X), us.x = %d (%X)\n", s.x, s.x, n.x, n.x);

    s.x = -1; /* Ubecja, -1 to out of range unsigneda */
    n.x = 2; /* OK! */
    printf("s.x = %u (%X), us.x = %d (%X)\n", s.x, s.x, n.x, n.x);

    union U
    {
        uint32_t x:12; /* to jest osobny int */
        uint32_t y:20; /* to nie jest dopelniienie X! */
        uint32_t a;
    } u = {0};

    u.x = 100;
    u.y = (1 << 15) - 1;

    printf("u.x\t");
    bit_print(u.x);

    printf("u.y\t");
    bit_print(u.y);

    printf("u.a\t");
    bit_print(u.a);

    printf("\n");

    /*
        struct S2
        {
            uint32_t x:u.a blad to nie jest compile time
            uint32_t y:10 * !!1 + sizeof(int) * 4 to jest w compile time wiec ok
            uint32_t t:10[10] blad nie ma arraya bitfieldow
            uint32_t t2[10]:10 blad taki jak wyzej
        } s2;
    */
}

static void bit_endianness(void)
{
    union U
    {
        uint32_t a;
        uint8_t b[4];
    } u = {0};

    /* ADDRESS -------------------------> */
    /* BE = 00 00 00 FF, LE = FF 00 00 00 */
    u.a = 0xFF;

    u.b[0] == 0xFF ? (printf("Little Endian\n")) : (printf("Big Endian\n"));

    printf("\n");

    /*
        The order of allocation of bit-fields within a unit (high-order to
        low-order or low-order to high-order) is implementation-defined.
    */
    union U2
    {
        struct B
        {
            uint32_t y:10;
            uint32_t x:3;
            uint32_t pad:32-13;
        } b;
        uint32_t a;
    } u2 = {0};

    u2.b.y = 0xB3;
    u2.b.x = 6;

    printf("u2.a\t");
    bit_print(u2.a); /* nie wiadomo co bedzie w u2.a */

    printf("u2.b.x\t");
    bit_print(u2.b.x); /* piszemy po b.x wiec czytanie jest gucci */

    printf("u2.b.y\t");
    bit_print(u2.b.y); /* Tak samo tutaj gucci */
}

static void bitwise_defined(void)
{
    /* ADDRESS -------------------------> */
    /* BE = DE AD BE EF, LE = EF BE AD DE */
    unsigned int a = 0xdeadbeef; /*  */
    /* W operacjach bitowych Endianess nie ma znaczenia */

    printf("a\t\t");
    bit_print(a);

    printf("~a\t\t");
    bit_print(~a); /* NEG per bit */

    printf("a | 0xff\t");
    bit_print(a | 0xFF); /* OR per bit */

    printf("a & 0x5\t\t");
    bit_print(a & 0x5); /* AND per bit */

    printf("a ^ 0x5\t\t");
    bit_print(a ^ 0x5); /* XOR per bit */

    printf("0xff << 3\t");
    bit_print(0xFF << 3); /* STD mowi jasno, to a * 8 (jesli liczba sie miesci w typie) */

    printf("0xff >> 3\t");
    bit_print(0xFF >> 3); /* STD mowi jasno, to a / 8 (jesli liczba sie miesci w typie) */

    printf("\n");
}

static void bit_macros(void)
{
    unsigned int a = 10;

    printf("A:\t\t");
    bit_print(a);

    printf("GET(A, 0)\t");
    bit_print(BIT_GET(a, 0));
    printf("GET(A, 1)\t");
    bit_print(BIT_GET(a, 1));

    printf("SET(A, 0)\t");
    bit_print(BIT_SET(a, 0));

    printf("CLEAR(A, 1)\t");
    bit_print(BIT_CLEAR(a, 1));

    printf("TOGGLE(A, 5)\t");
    bit_print(BIT_TOGGLE(a, 5));

    printf("GET(A, 0, 4)\t");
    bit_print(BIT_GET_MASK(a, 0, 4));

    printf("SET(A, 3, 10)\t");
    bit_print(BIT_SET_MASK(a, 3, 10));

    printf("CLEAR(A, 0, 30)\t");
    bit_print(BIT_CLEAR_MASK(a, 0, 30));

    printf("TOGGLE(A, 0, 3)\t");
    bit_print(BIT_TOGGLE_MASK(a, 0, 3));

    printf("\n");
}



/*
    E1 >> E2
    If E1 has a signed type  and a negative value, the resulting value is implementation-defined.

    E1 << E2
    If E1 has a signed type  and a negative value, then this is undefined behavior.
*/
static void bit_shifts(void)
{
    uint32_t a = 0xdeadbeef;

    printf("SAL i SHL\n");
    bit_print(a);
    asm volatile
    (
        "sal %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    a = 0xdeadbeef;
    asm volatile
    (
        "shl %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    /*
        SAR - arithmetic (zostawia ostatni bit)
        SHR - logical (zawsze nadpisuje bity 0)
    */
    printf("SAR i SHR\n");
    a = 0xdeadbeef;
    bit_print(a);
    asm volatile
    (
        "sar %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    a = 0xdeadbeef;
    asm volatile
    (
        "shr %0"
        : "=r" (a) : "0" (a)
    );
    bit_print(a);

    /*
        Widzimy ze jak mamy >=0 to musimy nadpisac 0
        no bo jak zostawimy to wyjdzie zla liczba tzn nie podzielilismy przez 2
        A jesli to byl <= to znowu trzeba zostawic, bo wyjdzie nam liczba nagle dodatnia

        Ze wzgledu ze int moze miec ujemne albo dodatnie to jak kompi nie zobaczy wartosci w compile time
        wezmie 1 z 2, wiec zawsze znajde taki przyklad ze bedzie zle

        Stad to Ubecja!!!
    */

    printf("C << i >> uint32_t\n");

    a = 0xdeadbeef;
    a <<= 1;
    bit_print(a);

    a = 0xdeadbeef;
    a >>= 1;
    bit_print(a);

    printf("C << i >> int32_t\n");

    int32_t b = (int32_t)0xdeadbeef;
    b <<= 1;
    bit_print((unsigned)b);

    b = (int32_t)0xdeadbeef;
    b >>= 1;
    bit_print((unsigned)b);
}

int main(void)
{
    bit_fields();
    bit_endianness();
    bitwise_defined();
    bit_macros();
    bit_shifts();

    return 0;
}