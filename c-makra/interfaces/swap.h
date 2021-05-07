#ifndef SWAP_H
#define SWAP_H

#define CONCAT(a, b) _CONCAT(a, b)
#define _CONCAT(a, b) a ## b

#define SWAP_GENERIC(a, b, type) __SWAP_GENERIC(a, b, type, CONCAT(CONCAT(a, b), CONCAT(, __LINE__)))
#define __SWAP_GENERIC(a, b, type, var) \
    do { \
        (void)(&(a) - &(b)); \
        (void)(&(a) - (type *)0); \
        type var = (a); \
        (a) = (b); \
        (b) = var; \
    } while (0)

#define SWAP_INT(a, b)      SWAP_GENERIC(a, b, int)
#define SWAP_DOUBLE(a, b)   SWAP_GENERIC(a, b, double)
#define SWAP_CHAR(a, b)     SWAP_GENERIC(a, b, char)

#endif