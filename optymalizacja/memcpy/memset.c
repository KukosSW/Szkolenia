#include <memset.h>
#include <immintrin.h>
#include <stdint.h>

/* --------------------------------------- STATIC FUNCTION DECLARATION --------------------------------------------- */

/* Core funkcji ktore wystawiamy, zrobione jako osobne aby latwiej bylo wolac je miedzy soba */
static inline void* __memset_u8(void* dst, int pattern, size_t size);
static inline void* __memset_u64(void* dst, int pattern, size_t size);
static inline void* __memset_u256(void* dst, int pattern, size_t size);

/* --------------------------------------- STATIC FUNCTION DEFINITION ---------------------------------------------- */

static inline void* __memset_u8(void* dst, int pattern, size_t size)
{
    if (dst == NULL)
        return NULL;

    if (size == 0)
        return dst;


    const uint8_t _pattern = (uint8_t)pattern;
    uint8_t* _dst = (uint8_t *)dst;

    for (size_t i = 0; i < size; ++i)
        _dst[i] = _pattern;

    return dst;
}

static inline void* __memset_u64(void* dst, int pattern, size_t size)
{
    if (dst == NULL)
        return NULL;

    if (size == 0)
        return dst;

    /* Pattern trzeba wypelnic, tzn mamy 1 bajt. I kopiujemy ten 1 bajt do kazdego bajtu 8bajtowej zmiennej */
    uint64_t _pattern;
    memset_u8(&_pattern, pattern, sizeof(_pattern));

    /* Najpierw ustawimy po 8bajtow, pozniej koncowe */
    uint64_t* _dst = (uint64_t *)dst;
    const size_t nr_of_u64_chunks = size / sizeof(uint64_t);

    for (size_t i = 0; i < nr_of_u64_chunks; ++i)
        _dst[i] = _pattern;

    /* Ustawiamy koncowke */
    const size_t bytes_to_copy = size % sizeof(uint64_t);

    (void)__memset_u8(&_dst[nr_of_u64_chunks], pattern, bytes_to_copy);

    return dst;
}

static inline void* __memset_u256(void* dst, int pattern, size_t size)
{
    if (dst == NULL)
        return NULL;

    if (size == 0)
        return dst;

    /* Pattern trzeba wypelnic, tzn mamy 1 bajt. I kopiujemy ten 1 bajt do kazdego bajtu 8bajtowej zmiennej */
    uint8_t buf[sizeof(__m256i)];
    memset_u64(&buf[0], pattern, sizeof(buf));

    /* Wczytaj bajty z bufora do ymm (avx) */
    __m256i _pattern = _mm256_loadu_si256((__m256i *)&buf);

    /* Najpierw kopiujemy po 32 bajty, pozniej skopiujemy koncowke */
    __m256i* _dst = (__m256i *)dst;
    const size_t nr_of_u256_chunks = size / sizeof(__m256i);

    for (size_t i = 0; i < nr_of_u256_chunks; ++i)
        _mm256_storeu_si256(&_dst[i], _pattern);

    /* Kopiujemy koncowke, przy czym tam moga byc nawet > 64 */
    const size_t bytes_to_copy = size % sizeof(__m256i);

    (void)__memset_u64(&_dst[nr_of_u256_chunks], pattern, bytes_to_copy);

    return dst;
}

/* -------------------------------------------- FUNCTION DEFINITION ------------------------------------------------ */

void* memset_u8(void* dst, int pattern, size_t size)
{
    return __memset_u8(dst, pattern, size);
}

void* memset_u64(void* dst, int pattern, size_t size)
{
    return __memset_u64(dst, pattern, size);
}

void* memset_u256(void* dst, int pattern, size_t size)
{
    return __memset_u256(dst, pattern, size);
}