#include <file_common.h>
#include <stdio.h>
#include <stdarg.h>

__attribute__((__format__ (__printf__, 1, 0)))
void print(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
