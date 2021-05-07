#ifndef UNIQUE_H
#define UNIQUE_H

#include <stddef.h>

size_t unique_count_erase(const int t[], size_t n);
size_t unique_count(const int t[], size_t n);
size_t unique_count_sort(const int t[], size_t n);

/* C++, bo potrzebne sa struktury danych */
#ifdef __cplusplus
extern "C" {
#endif
size_t unique_hashset(const int t[], size_t n);
size_t unique_set(const int t[], size_t n);
size_t unique_cpp(const int t[], size_t n);
#ifdef __cplusplus
}
#endif

#endif