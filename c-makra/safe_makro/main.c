#include <stdio.h>


#include <file1.h>
// #include <file1.h>

#include <file2.h>
#include <file3.h>

int main(void)
{
    printf("MULT = %d %d %d\n", MULT(2 + 1, 3) / 2, MULT2(2 + 1, 3) / 2, MULT3(2 + 1, 3) / 2);

    volatile int a = 4;
    volatile int b = 4;
    volatile int c = MIN(a++, --b);
    printf("MIN(%d, %d) = %d\n", a, b, c);

    FOO;
    BAR();

    printf("A = %d, B = %d\n", a, b);
    if (a < b)
        MULTILINE();

    if (a < b)
        MULTILINE2()//;
    else
        printf("ELSE\n");

    if (a < b)
        MULTILINE3();
    else
        MULTILINE3();

    int xxx = 16;
    x(xxx);
    return 0;
}