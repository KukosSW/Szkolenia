#include <stdio.h>

#include <compare.h>
#include <swap.h>
#include <intmath.h>
#include <doublemath.h>

CMP_CREATE_TYPE(int)
CMP_CREATE_TYPE(double)
CMP_CREATE_TYPE(char)

CMP_CREATE_GENERIC(int)
CMP_CREATE_GENERIC(double)
CMP_CREATE_GENERIC(char)

void compare_test(void);
void swap_test(void);
void math_test(void);

void compare_test(void)
{
    cmp_f cmp_i = cmp_generic_int;
    cmp_f cmp_d = cmp_generic_double;
    cmp_f cmp_c = cmp_generic_char;

    printf("%d vs %d ? %d (%d)\n", 2, 10, cmp_int(2, 10), cmp_i(&(int){2}, &(int){10}));
    printf("%lf vs %lf ? %d (%d)\n", 3.14, 2.78, cmp_double(3.14, 2.78), cmp_d(&(double){3.14}, &(double){2.78}));
    printf("%c vs %c ? %d (%d)\n", 'A', 'A', cmp_char('A', 'A'), cmp_c(&(char){'A'}, &(char){'A'}));
}

void swap_test(void)
{
    int a = 10, b = 20;
    double c = 4.5, d = 5.4;
    char e = 'A', f = 'Z';

    SWAP_INT(a, b);
    SWAP_DOUBLE(c, d);
    SWAP_CHAR(e, f);
    SWAP_GENERIC(e, f, char);
    /* arytmetyka wskznikow broni nas przed takimi bledami juz podczas kompilacji */
    //SWAP_GENERIC(e, f, int);
    // SWAP_INT(a, e);

    printf("a = %d, b = %d\n", a, b);
    printf("c = %lf, d = %lf\n", c, d);
    printf("e = %c, f = %c\n", e, f);
}

void math_test(void)
{
    int a = 10;
    int b = -5;

    double c = 4.9;
    double d = 9.4;

    printf("%d + %d = %d\n", a, b, intmath_add(a, b));
    printf("%d - %d = %d\n", a, b, intmath_sub(a, b));
    printf("%d * %d = %d\n", a, b, intmath_mul(a, b));
    printf("%d / %d = %d\n", a, b, intmath_div(a, b));

    printf("%lf + %lf = %lf\n", c, d, doublemath_add(c, d));
    printf("%lf - %lf = %lf\n", c, d, doublemath_sub(c, d));
    printf("%lf * %lf = %lf\n", c, d, doublemath_mul(c, d));
    printf("%lf / %lf = %lf\n", c, d, doublemath_div(c, d));
}

int main(void)
{
    compare_test();
    swap_test();
    math_test();
    return 0;
}