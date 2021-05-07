#ifndef VAARGS_H
#define VAARGS_H

#include <stdarg.h>
#include <stddef.h>

int sum(size_t n, ...);
int vsum(size_t n, va_list args);

void printf_simply(const char* fmt, ...);
void vprintf_simply(const char* fmt, va_list args);

int sum_with_debug(size_t n, ...);

typedef struct Foo
{
    int a;
    long b;
} Foo;

void struct_passed_to_valist(size_t n, ...);

#endif