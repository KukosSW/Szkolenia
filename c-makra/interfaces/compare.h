#ifndef COMPARE_H
#define COMPARE_H

typedef int (*cmp_f)(const void* a, const void* b);

#define CMP_CREATE_TYPE(type) \
    static inline int cmp_##type(type a, type b) \
    { \
        if (a < b) \
            return -1; \
        if (a > b) \
            return 1; \
        return 0; \
    }

#define CMP_CREATE_GENERIC(type) \
    static inline int cmp_generic_##type(const void *a, const void* b) \
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