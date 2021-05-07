#ifndef FIB_H
#define FIB_H

#include <inttypes.h>

/* Wersja rekurencyjna */
uint64_t fib_rec(uint64_t n);

/* Linearyzacja rekurencji dzieki dodatkowej pamieci */
uint64_t fib_rec_mem(uint64_t n);

/* Iteracyjna wersja potrzebuje sporo pamieci (O(n)) */
uint64_t fib_ite_mem(uint64_t n);

/* Iteracyjna wersja, stala pamiec */
uint64_t fib_ite(uint64_t n);

#endif