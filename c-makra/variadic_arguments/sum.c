#include <sum.h>
#include <stdarg.h>

int sum(int n, ...)
{
    va_list args;
    va_start(args, n);

    int sum = 0;
    for (int i = 0; i < n; ++i)
        sum += (int)va_arg(args, int);

    va_end(args);
    return sum;
}