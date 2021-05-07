#ifndef MEMCPY_H
#define MEMCPY_H

/*
    Implementation of many versions of memcpy.

    author: Kamil Kielbasa
    email: dusergithub@gmail.com

    LICENCE: GPL 3.0

    PLIK ZOSTAL SKOPIOWANY I ZMODYFIKOWANY ZA ZGODA AUTORA!
    Github z oryginalem: github.com/duser96/KS
*/

#include <stddef.h> /* size_t */


/* Memcpy kopiowanie 1,8,32 Bajtow w jednym cyklu. Wersja 256 uzywa ckfow do avx256 */
void* memcpy_u8(void* restrict dst, const void* restrict src, size_t size);
void* memcpy_u64(void* restrict dst, const void* restrict src, size_t size);
void* memcpy_u256(void* restrict dst, const void* restrict src, size_t size);

/* Sami dodajemy "optymalizacje" i rozwijamy 8 iteracji petli w 8 krokow pojedynczej iteracji */
void* memcpy_u64_unroll(void* restrict dst, const void* restrict src_p, size_t size);
void* memcpy_u256_unroll(void* restrict dst, const void* restrict src, size_t size);

#endif