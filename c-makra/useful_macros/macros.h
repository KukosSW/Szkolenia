#ifndef MACROS_H
#define MACROS_H

/* Zamiana makr i tokenow na stringi */
#define __TOSTRING(x) #x
#define TOSTRING(x) __TOSTRING(x)

#define __CONCAT(x, y) x ## y
#define CONCAT(x, y) __CONCAT(x, y)

#define MACRO_DEBUG(MACRO) #MACRO " = " TOSTRING(MACRO)

/* VARIADIC ARGS */
#define _NARGS(_1, _2, _3, _4, _5, _N, ...) _N
#define NARGS(...) _NARGS(__VA_ARGS__, 5, 4, 3, 2, 1)

#define __DO_MACRO_1_1(job, x) job(x)
#define __DO_MACRO_1_2(job, x, ...) job(x); __DO_MACRO_1_1(job, __VA_ARGS__)
#define __DO_MACRO_1_3(job, x, ...) job(x); __DO_MACRO_1_2(job, __VA_ARGS__)
#define __DO_MACRO_1_4(job, x, ...) job(x); __DO_MACRO_1_3(job, __VA_ARGS__)
#define __DO_MACRO_1_5(job, x, ...) job(x); __DO_MACRO_1_4(job, __VA_ARGS__)
#define DOMACRO_1_N(n, job, ...) CONCAT(__DO_MACRO_1_, n)(job, __VA_ARGS__)
#define DOMACRO_1(job, ...) do { DOMACRO_1_N(NARGS(__VA_ARGS__), job, __VA_ARGS__); } while (0)
#define DOMACRO(job, ...) DOMACRO_1(job, __VA_ARGS__)

/* Wzbogacony print */
#define __PRINT_HEAD(X, ...) X
#define PRINT_HEAD(...) __PRINT_HEAD(__VA_ARGS__, "")

#define __PRINT_TAIL(X, ...) __VA_ARGS__
#define PRINT_TAIL(...) __PRINT_TAIL(__VA_ARGS__, "")

#define __PRINT_PRETTY(fmt, ...) printf("%s:%s.%d " fmt "%s", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define PRINT_PRETTY(...) __PRINT_PRETTY(PRINT_HEAD(__VA_ARGS__), PRINT_TAIL(__VA_ARGS__))

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

/* Sprawdzenie typow */
#define CHECK_TYPES(a, b) (&(a) - &(b))

/* Tworzenie unikalnej nazwy do makr */
#define UNIQUE_VAR_NAME(prefix) CONCAT(CONCAT(____unique_var_name, prefix), __LINE__)

/* Bezpieczny swap */
#define SWAP_GENERIC(a, b, type) __SWAP_GENERIC(a, b, type, UNIQUE_VAR_NAME(CONCAT(a, b)))
#define __SWAP_GENERIC(a, b, type, var) \
    do { \
        (void)CHECK_TYPES(a, b); \
        (void)CHECK_TYPES(a, (type){0}); \
        type var = (a); \
        (a) = (b); \
        (b) = var; \
    } while (0)

/* generyczny zapis / odczyt, dosc szybki bo nie zawsze wywoluje memcpy */
/*
    Testowalem zapisy intow gdy sizeof byl ze zmiennej volatile, zapis 2^30 razy
    memcpy: 3.4s
    macro: 0.94s
*/
#define WRITE_ONCE_SIZE(dst, src, size) \
    do{ \
        _Pragma("GCC diagnostic push"); \
        _Pragma("GCC diagnostic ignored \"-Wstrict-aliasing\""); \
        switch (size) \
        { \
            case 1: *(uint8_t *)&dst = *(uint8_t *)&src; break; \
            case 2: *(uint16_t *)&dst = *(uint16_t *)&src; break; \
            case 4: *(uint32_t *)&dst = *(uint32_t *)&src; break; \
            case 8: *(uint64_t *)&dst = *(uint64_t *)&src; break; \
            default: (void)memcpy(&dst, &src, (size_t)(size)); \
        } \
        _Pragma("GCC diagnostic pop"); \
    } while (0)

#define WRITE_ONCE(dst, src) WRITE_ONCE_SIZE(dst, src, sizeof(dst))


/* Bezpieczny lepszy SWAP :) */
#define SWAP(a, b) __SWAP(a, b, UNIQUE_VAR_NAME(CONCAT(a, b)))
#define __SWAP(a, b, var) \
    do { \
        (void)CHECK_TYPES(a, b); \
        char var[sizeof(a)]; \
        WRITE_ONCE(var, a); \
        WRITE_ONCE(a, b); \
        WRITE_ONCE(b, var); \
    } while(0)


/* Prawie bezpieczne min i max */
#define MIN(a, b) (CHECK_TYPES(a, b) * 0 + ((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (CHECK_TYPES(a, b) * 0 + ((a) >= (b)) ? (a) : (b))

/* free + NULL */
#define FREE(ptr) \
    do { \
        free(ptr); \
        ptr = NULL; \
    } while (0);

/* rozmiar tablic (nie pointerow!!!) */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/* BITY */
#define BIT_MASK(s, e)           (((1ul << ((e) - (s) + 1)) - 1) << (s))
#define BIT_GET_MASK(n, s, e)    (((n) & (BIT_MASK(s, e))) >> s)
#define BIT_SET_MASK(n, s, e)    ((n) | (BIT_MASK(s, e)))
#define BIT_CLEAR_MASK(n, s, e)  ((n) & ~(BIT_MASK(s, e)))
#define BIT_TOGGLE_MASK(n, s, e) ((n) ^ (BIT_MASK(s, e)))

#define BIT_N(n)         BIT_MASK(n, n)
#define BIT_GET(n, k)    BIT_GET_MASK(n, k, k)
#define BIT_SET(n, k)    BIT_SET_MASK(n, k, k)
#define BIT_CLEAR(n, k)  BIT_CLEAR_MASK(n, k, k)
#define BIT_TOGGLE(n, k) BIT_TOGGLE_MASK(n, k, k)

/* static assert jesli nie mamy C11 */
#define STATIC_ASSERT(cond) ((void)sizeof(char[1 - 2 * (!(cond))]));


/* ESCAPE COLORS, do uber pretty print :) */
#define COLOR_RESET           "\033[0m"
#define COLOR_BLACK           "\033[30m"
#define COLOR_RED             "\033[31m"
#define COLOR_GREEN           "\033[32m"
#define COLOR_YELLOW          "\033[33m"
#define COLOR_BLUE            "\033[34m"
#define COLOR_MAGENTA         "\033[35m"
#define COLOR_CYAN            "\033[36m"
#define COLOR_WHITE           "\033[37m"
#define COLOR_BOLDBLACK       "\033[1m\033[30m"
#define COLOR_BOLDRED         "\033[1m\033[31m"
#define COLOR_BOLDGREEN       "\033[1m\033[32m"
#define COLOR_BOLDYELLOW      "\033[1m\033[33m"
#define COLOR_BOLDBLUE        "\033[1m\033[34m"
#define COLOR_BOLDMAGENTA     "\033[1m\033[35m"
#define COLOR_BOLDCYAN        "\033[1m\033[36m"
#define COLOR_BOLDWHITE       "\033[1m\033[37m"

#define COLORED_STRING(COLOR, string) COLOR string COLOR_RESET


/*
    Jesli ktos nie zna patenciakow to mozna uzyc tych makr
    Poniewaz to patenciaki a nie prawilne makra dalem je malymi literami
*/
#define unused_var(x) (void)(x)
#define uninitialized_var(x) x = {0}
#define cast_to_bool(x) (!!(x))
#define variable_opt_off(x) volatile x
#define unused_retval(func) do { if (func) { } else { } } while (0)

#endif