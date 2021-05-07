#include <memcpy.h>
#include <immintrin.h> /* avx */
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------- STATIC FUNCTION DECLARATION --------------------------------------------- */

/* Core funkcji ktore wystawiamy, zrobione jako osobne aby latwiej bylo wolac je miedzy soba */
static inline void* __memcpy_u8(void* restrict dst, const void* restrict src, size_t size);
static inline void* __memcpy_u64(void* restrict dst, const void* restrict src, size_t size);
static inline void* __memcpy_u256(void* restrict dst, const void* restrict src, size_t size);
static inline void* __memcpy_u64_unroll(void* restrict dst, const void* restrict src, size_t size);
static inline void* __memcpy_u256_unroll(void* restrict dst, const void* restrict src, size_t size);

/* --------------------------------------- STATIC FUNCTION DEFINITION ---------------------------------------------- */

static inline void* __memcpy_u8(void* restrict dst, const void* restrict src, size_t size)
{
    if (dst == NULL || src == NULL)
        return NULL;

    if (size == 0)
        return dst;

    uint8_t* restrict _dst = (uint8_t *)dst;
    const uint8_t* restrict _src = (uint8_t *)src;

    for (size_t i = 0; i < size; ++i)
        _dst[i] = _src[i];

    return dst;
}

static inline void* __memcpy_u64(void* restrict dst, const void* restrict src, size_t size)
{
    if (dst == NULL || src == NULL)
        return NULL;

    if (size == 0)
        return dst;

    /* Najpierw kopiujemy po 8 bajtow, pozniej skopiujemy koncowke */
    uint64_t* restrict _dst = (uint64_t *)dst;
    const uint64_t* restrict _src = (uint64_t *)src;
    const size_t nr_of_u64_chunks = size / sizeof(uint64_t);

    for (size_t i = 0; i < nr_of_u64_chunks; ++i)
        _dst[i] = _src[i];

    /* Kopiujemy koncowke */
    const size_t bytes_to_copy = size % sizeof(uint64_t);

    (void)__memcpy_u8(&_dst[nr_of_u64_chunks], &_src[nr_of_u64_chunks], bytes_to_copy);

    return dst;
}

static inline void* __memcpy_u64_unroll(void* restrict dst, const void* restrict src, size_t size)
{
    if (dst == NULL || src == NULL)
        return NULL;

    if (size == 0)
        return dst;

    /*
        Poprzednio kopiowalismy po 8 bajtow. Teraz recznie rozwijamy petle.
        W jednej iteracji bedziemy kopiowac 8 x 8 bajtow.
    */
    uint64_t* restrict _dst = (uint64_t *)dst;
    const uint64_t* restrict _src = (uint64_t *)src;
    const size_t nr_of_u64_chunks = size / sizeof(uint64_t);

    /* (A / B) * B w jezyku C to nie A tylko A - A % B i o to nam chodzi, bo to pelne 8 */
    const size_t max_unroll_idx = (nr_of_u64_chunks / 8) * 8;

    for (size_t i = 0; i < max_unroll_idx; i += 8)
    {
        _dst[i]     = _src[i];
        _dst[i + 1] = _src[i + 1];
        _dst[i + 2] = _src[i + 2];
        _dst[i + 3] = _src[i + 3];
        _dst[i + 4] = _src[i + 4];
        _dst[i + 5] = _src[i + 5];
        _dst[i + 6] = _src[i + 6];
        _dst[i + 7] = _src[i + 7];
    }

    /*
        Kopiujemy koncowke, jednak mozliwe ze az 7 pelnych 8-bajtowych czesci zostalo.
        Dlatego koncowka kopiowana jest zwyklym __memcpy_u64 ktora skopiuje koncowke (<8 bajtow)
        za pomoca __memcpy_u8
    */

    const size_t bytes_to_copy  = size - max_unroll_idx * sizeof(*_dst);
    (void)__memcpy_u64(&_dst[max_unroll_idx], &_src[max_unroll_idx], bytes_to_copy);

    return dst;
}

static inline void* __memcpy_u256(void* restrict dst, const void* restrict src, size_t size)
{
    if (dst == NULL || src == NULL)
        return NULL;

    if (size == 0)
        return dst;

    /* Najpierw kopiujemy po 32 bajty, pozniej skopiujemy koncowke */
    __m256i* restrict _dst = (__m256i *)dst;
    const __m256i* restrict _src = (__m256i *)src;
    const size_t nr_of_u256_chunks = size / sizeof(__m256i);

    for (size_t i = 0; i < nr_of_u256_chunks; ++i)
        _mm256_storeu_si256(&_dst[i], _mm256_loadu_si256(&_src[i]));

    /* Kopiujemy koncowke, przy czym tam moga byc nawet > 64 */
    const size_t bytes_to_copy = size % sizeof(__m256i);

    (void)__memcpy_u64(&_dst[nr_of_u256_chunks], &_src[nr_of_u256_chunks], bytes_to_copy);

    return dst;
}

static inline void* __memcpy_u256_unroll(void* restrict dst, const void* restrict src, size_t size)
{
    if (dst == NULL || src == NULL)
        return NULL;

    if (size == 0)
        return dst;

    /* Najpierw kopiujemy po 32 bajty, pozniej skopiujemy koncowke */
    __m256i* restrict _dst = (__m256i *)dst;
    const __m256i* restrict _src = (__m256i *)src;
    const size_t nr_of_u256_chunks = size / sizeof(__m256i);

    const size_t max_unroll_idx = (nr_of_u256_chunks / 8) * 8;
    for (size_t i = 0; i < max_unroll_idx; i += 8)
    {
        _mm256_storeu_si256(&_dst[i], _mm256_loadu_si256(&_src[i]));
        _mm256_storeu_si256(&_dst[i + 1], _mm256_loadu_si256(&_src[i + 1]));
        _mm256_storeu_si256(&_dst[i + 2], _mm256_loadu_si256(&_src[i + 2]));
        _mm256_storeu_si256(&_dst[i + 3], _mm256_loadu_si256(&_src[i + 3]));
        _mm256_storeu_si256(&_dst[i + 4], _mm256_loadu_si256(&_src[i + 4]));
        _mm256_storeu_si256(&_dst[i + 5], _mm256_loadu_si256(&_src[i + 5]));
        _mm256_storeu_si256(&_dst[i + 6], _mm256_loadu_si256(&_src[i + 6]));
        _mm256_storeu_si256(&_dst[i + 7], _mm256_loadu_si256(&_src[i + 7]));
    }

    /*
        Kopiujemy koncowke, jednak mozliwe ze az 7 pelnych 32-bajtowych czesci zostalo.
        Dlatego koncowka kopiowana jest zwyklym __memcpy_u256 ktora skopiuje koncowke (<8 bajtow)
        za pomoca __memcpy_u8
    */

    const size_t bytes_to_copy  = size - max_unroll_idx * sizeof(*_dst);
    (void)__memcpy_u256(&_dst[max_unroll_idx], &_src[max_unroll_idx], bytes_to_copy);

    return dst;
}

/* -------------------------------------------- FUNCTION DEFINITION ------------------------------------------------ */

void* memcpy_u8(void* restrict dst, const void* restrict src, size_t size)
{
    return __memcpy_u8(dst, src, size);
}

void* memcpy_u64(void* restrict dst, const void* restrict src, size_t size)
{
    return __memcpy_u64(dst, src, size);
}

void* memcpy_u64_unroll(void* restrict dst, const void* restrict src, size_t size)
{
    return __memcpy_u64_unroll(dst, src, size);
}

void* memcpy_u256(void* restrict dst, const void* restrict src, size_t size)
{
    return __memcpy_u256(dst, src, size);
}

void* memcpy_u256_unroll(void* restrict dst, const void* restrict src, size_t size)
{
    return __memcpy_u256_unroll(dst, src, size);
}