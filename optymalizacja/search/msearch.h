#ifndef MSEARCH_H
#define MSEARCH_H

#include <stddef.h>
#include <sys/types.h>

typedef int (*cmp_int_f)(const void*, const void*);

ssize_t search_linear(const int t[], size_t n, int key, cmp_int_f cmp);

ssize_t search_binary(const int t[], size_t n, int key, cmp_int_f cmp);

ssize_t search_interpolation(const int t[], size_t n, int key, cmp_int_f cmp);

#endif