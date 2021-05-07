#include <stdio.h>
#include <domacro.h>

void f(int a);
void g(int a, int b);

void f(int a)
{
    printf("%d\n", a);
}

void g(int a, int b)
{
    printf("%d + %d = %d\n", a, b, a + b);
}

#define PRINT(x) printf("%d\n", x)
#define PRINT2(x, y) printf("%d + %d = %d\n", x, y, x + y)

int main(void)
{
    DOMACRO(f, 1, 2, 3, 4);
    DOMACRO(PRINT, 1, 2, 3, 4);

    DOMACRO_2(g, 1, 2, 3, 4);
    DOMACRO_2(PRINT2, 1, 2, 3, 4);

    return 0;
}