#ifndef STATIC_POLY_H
#define STATIC_POLY_H

/* Komparator do qsorta */
#define CMP_CREATE_GENERIC(type, function_name) \
    static inline int function_name(const void *a, const void* b); \
    static inline int function_name(const void *a, const void* b) \
    { \
        const type _a = *(const type *)a; \
        const type _b = *(const type *)b; \
        \
        if (_a < _b) \
            return -1; \
        if (_a > _b) \
            return 1; \
        return 0; \
    }

#endif