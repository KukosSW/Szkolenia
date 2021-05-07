#include <stdio.h>
#include <sum.h>
#include <nargs.h>
#include <print.h>

int main(void)
{
    int t[] = {1, 2, 3, 4, 5, 6};
    printf("SUM(%d, %d) = %d (%d)\n", t[0], t[1], SUM2(t[0], t[1]), SUM(2, t[0], t[1]));
    printf("SUM(%d, %d, %d) = %d (%d)\n", t[0], t[1], t[2], SUM3(t[0], t[1], t[2]), SUM(3, t[0], t[1], t[2]));
    printf("SUM(%d, %d, %d, %d) = %d (%d)\n", t[0], t[1], t[2], t[3], SUM4(t[0], t[1], t[2], t[3]), SUM(4, t[0], t[1], t[2], t[3]));

    /* C99 zabrania bez parametrowego calla makra z ..., gcc wstawia tutaj jakis shit i przesuwa makro NARGS na 1 */
    NARGS_EXAMPLE();
    NARGS_EXAMPLE(1, 2);

    printf("SUM(%d, %d) = %d (%d)\n", t[0], t[1], SUM2(t[0], t[1]), SUM_VA(t[0], t[1]));
    printf("SUM(%d, %d, %d) = %d (%d)\n", t[0], t[1], t[2], SUM3(t[0], t[1], t[2]), SUM_VA(t[0], t[1], t[2]));
    printf("SUM(%d, %d, %d, %d) = %d (%d)\n", t[0], t[1], t[2], t[3], SUM4(t[0], t[1], t[2], t[3]), SUM_VA(t[0], t[1], t[2], t[3]));

    PRINT_WEAK("Szczesliwa = %d\n", 41);
    // PRINT_WEAK("Sam print\n");

    PRINT_GCC("Szczesliwa = %d\n", 41);
    PRINT_GCC("Sam print\n");

    PRINT("Szczesliwa = %d\n", 41);
    PRINT("Sam print\n");

    PRINT_PRETTY("Szczesliwa = %d\n", 41);
    PRINT_PRETTY("Sam print\n");

    return 0;
}