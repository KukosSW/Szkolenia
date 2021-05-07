#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <macros.h>

#define FOO(x, y) (x + y)
#define PRINT_INT(x) printf("%4d", x)

CMP_CREATE_GENERIC(int, cmp_int)

int main(void)
{
    printf("%s\n", MACRO_DEBUG(FOO(2, 3)));
    DOMACRO(PRINT_INT, 1, 2, 3, 4, 5);
    printf("\n");
    PRINT_PRETTY("Ala ma kota\n");

    int a = 5;
    int b = 10;
    printf("a = %d, b = %d\n", a, b);
    printf("MIN(%d, %d) = %d\n", a, b, MIN(a, b));
    printf("MAX(%d, %d) = %d\n", a, b, MAX(a, b));
    SWAP_GENERIC(a, b, int);
    printf("Teraz a = %d, b = %d\n", a, b);
    SWAP(a, b);
    printf("Teraz a = %d, b = %d\n", a, b);

    int t[] = {1, 2, 3, 4, 5};
    char buffer[sizeof(t[0]) + sizeof(t[1])];
    printf("T len = %zu\n", ARRAY_SIZE(t));
    STATIC_ASSERT(ARRAY_SIZE(t) == 5);

    int i;
    WRITE_ONCE(buffer, t);
    WRITE_ONCE(i, buffer);
    printf("I = %d\n", i);

    int c = 2;
    int d = BIT_SET(c, 4);
    printf("%d | (1 << 4) = %d\n", c, d);

    printf(COLOR_BOLDBLUE "Kolorowanka\n" COLOR_RESET);
    printf(COLORED_STRING(COLOR_GREEN, "Bezpieczna kolorowanka\n"));

    /* patenciaki */
    int x;
    unused_var(x);

    uninitialized_var(int y);
    variable_opt_off(int z);
    unused_var(y);
    unused_var(z);
    x = 10;
    printf("X = %d, bool typu int = %d\n", x, cast_to_bool(x));

    unused_retval(scanf("%d", &x));

    return 0;
}