#ifndef MEMSET_H
#define MEMSET_H

/*
    Implementation of many versions of memset.

    author: Kamil Kielbasa
    email: dusergithub@gmail.com

    LICENCE: GPL 3.0

    PLIK ZOSTAL SKOPIOWANY I ZMODYFIKOWANY ZA ZGODA AUTORA!
    Github z oryginalem: github.com/duser96/KS
*/

#include <stddef.h> /* size_t */

void* memset_u8(void* dst, int pattern, size_t size);
void* memset_u64(void* dst, int pattern, size_t size);
void* memset_u256(void* dst, int pattern, size_t size);

#endif