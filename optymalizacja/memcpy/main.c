
#include <memcpy.h>
#include <memset.h>

/********************** TESTY ****************************/

#include <test_framework.h>
#include <stdlib.h>
#include <string.h>

typedef void* (*memcpy_f)(void* restrict, const void* restrict, size_t);
typedef void* (*memset_f)(void*, int, size_t);

static void* array_create_random(size_t n);

static void test_suite(void);
static void test_suite_memcpy(memcpy_f memcpyf);
static void test_suite_memset(memset_f memsetf);

static test_f test_case_memcpy_error(memcpy_f memcpyf);
static test_f test_case_memcpy_fewbytes(memcpy_f memcpyf);
static test_f test_case_memcpy_aligned_big_array(memcpy_f memcpyf);
static test_f test_case_memcpy_unaligned_big_array(memcpy_f memcpyf);

static test_f test_case_memset_error(memset_f memsetf);
static test_f test_case_memset_fewbytes(memset_f memsetf);
static test_f test_case_memset_aligned_big_array(memset_f memsetf);
static test_f test_case_memset_unaligned_big_array(memset_f memsetf);

static void* array_create_random(size_t n)
{
    char *t = malloc(n * sizeof(*t));
    if (t == NULL)
        perror("Malloc error");

    for (size_t i = 0; i < n; ++i)
        t[i] = (char)((rand() % ('Z' - 'A')) + 'A');

    return t;
}

static test_f test_case_memcpy_error(memcpy_f memcpyf)
{
    char buf[2];
    T_EXPECT(memcpyf(NULL, &buf[0], 1), NULL);
    T_EXPECT(memcpyf(&buf[0], NULL, 1), NULL);
    T_EXPECT(memcpyf(&buf[0], &buf[1], 0), &buf[0]);
}

static test_f test_case_memcpy_fewbytes(memcpy_f memcpyf)
{
    const size_t size = 100;
    char* src = array_create_random(size);
    char* dst = array_create_random(size);

    T_EXPECT(memcpyf(dst, src, size), dst);
    T_EXPECT(memcmp(dst, src, size), 0);

    free(src);
    free(dst);
}

static test_f test_case_memcpy_aligned_big_array(memcpy_f memcpyf)
{
    const size_t size = (1 << 20) - 31;
    char* src = array_create_random(size);
    char* dst = array_create_random(size);

    T_EXPECT(memcpyf(dst, src, size), dst);
    T_EXPECT(memcmp(dst, src, size), 0);

    free(src);
    free(dst);
}

static test_f test_case_memcpy_unaligned_big_array(memcpy_f memcpyf)
{
    const size_t size = 1 << 20;
    char* src = array_create_random(size);
    char* dst = array_create_random(size);

    T_EXPECT(memcpyf(dst + 5, src + 5, size - 5), dst + 5);
    T_EXPECT(memcmp(dst + 5, src + 5, size - 5), 0);

    free(src);
    free(dst);
}

static test_f test_case_memset_error(memset_f memsetf)
{
    char buf[2];
    int pat = 1;
    T_EXPECT(memsetf(NULL, pat, 1), NULL);
    T_EXPECT(memsetf(&buf[0], pat, 0), &buf[0]);
}

static test_f test_case_memset_fewbytes(memset_f memsetf)
{
    const size_t size = 100;
    int pat = 41;
    char* src = array_create_random(size);
    char* expected = array_create_random(size);

    T_EXPECT(memsetf(src, pat, size), src);

    memset(expected, pat, size);

    T_EXPECT(memcmp(expected, src, size), 0);

    free(src);
    free(expected);
}

static test_f test_case_memset_aligned_big_array(memset_f memsetf)
{
    const size_t size = (1 << 20) - 31;
    int pat = 17;
    char* src = array_create_random(size);
    char* expected = array_create_random(size);

    T_EXPECT(memsetf(src, pat, size), src);

    memset(expected, pat, size);

    T_EXPECT(memcmp(expected, src, size), 0);

    free(src);
    free(expected);
}

static test_f test_case_memset_unaligned_big_array(memset_f memsetf)
{
    const size_t size = (1 << 20);
    int pat = 17;
    char* src = array_create_random(size);
    char* expected = array_create_random(size);

    T_EXPECT(memsetf(src + 5, pat, size - 5), src + 5);

    memset(expected + 5, pat, size - 5);

    T_EXPECT(memcmp(expected + 5, src + 5, size - 5), 0);

    free(src);
    free(expected);
}

static void test_suite_memcpy(memcpy_f memcpyf)
{
    TEST(test_case_memcpy_error(memcpyf));
    TEST(test_case_memcpy_fewbytes(memcpyf));
    TEST(test_case_memcpy_aligned_big_array(memcpyf));
    TEST(test_case_memcpy_unaligned_big_array(memcpyf));
}

static void test_suite_memset(memset_f memsetf)
{
    TEST(test_case_memset_error(memsetf));
    TEST(test_case_memset_fewbytes(memsetf));
    TEST(test_case_memset_aligned_big_array(memsetf));
    TEST(test_case_memset_unaligned_big_array(memsetf));
}

static void test_suite(void)
{
    test_suite_memcpy(memcpy_u8);
    test_suite_memcpy(memcpy_u64);
    test_suite_memcpy(memcpy_u256);
    test_suite_memcpy(memcpy_u64_unroll);
    test_suite_memcpy(memcpy_u256_unroll);

    test_suite_memset(memset_u8);
    test_suite_memset(memset_u64);
    test_suite_memset(memset_u256);
}

/********************************************************/

/********************* POMIARY **************************/
#include <stdio.h>
#include <time.h>

#define MEASURE_FUNCTION(func, label) \
    do { \
        struct timespec start; \
        struct timespec end; \
        double timeTaken; \
        \
        clock_gettime(CLOCK_MONOTONIC, &start); \
        (void)func; \
        clock_gettime(CLOCK_MONOTONIC, &end); \
        \
        timeTaken = (double)(end.tv_sec - start.tv_sec) * 1e9; \
        timeTaken = (double)(timeTaken + (double)(end.tv_nsec - start.tv_nsec)) * 1e-9; \
        \
        printf(label ": czas = %lf[s]\n", timeTaken); \
    } while (0)

static void meas(void);
static void meas_memcpy(memcpy_f memcpyf);
static void meas_single_memcpy(memcpy_f memcpyf, size_t size);

static void meas_memset(memset_f memsetf);
static void meas_single_memset(memset_f memsetf, size_t size);

static void meas_single_memcpy(memcpy_f memcpyf, size_t size)
{
    char *src = array_create_random(size);
    char *dst = array_create_random(size);

    printf("\t%zu Bytes", size);
    MEASURE_FUNCTION(memcpyf(dst, src, size), "");

    free(src);
    free(dst);
}

static void meas_single_memset(memset_f memsetf, size_t size)
{
    char *src = array_create_random(size);
    int pattern = rand();

    printf("\t%zu Bytes", size);
    MEASURE_FUNCTION(memsetf(src, pattern, size), "");

    free(src);
}

static void meas_memcpy(memcpy_f memcpyf)
{
    meas_single_memcpy(memcpyf, (1 << 10) - 10);
    meas_single_memcpy(memcpyf, (1 << 30) - 30);
}

static void meas_memset(memset_f memsetf)
{
    meas_single_memset(memsetf, (1 << 10) - 10);
    meas_single_memset(memsetf, (1 << 30) - 30);
}

static void meas(void)
{
    /* MEMCPY */
    printf("memcpy\n");
    meas_memcpy(memcpy);

    printf("memcpy_u8\n");
    meas_memcpy(memcpy_u8);

    printf("memcpy_u64\n");
    meas_memcpy(memcpy_u64);

    printf("memcpy_u256\n");
    meas_memcpy(memcpy_u256);

    printf("memcpy_u64_unroll\n");
    meas_memcpy(memcpy_u64_unroll);

    printf("memcpy_u256_unroll\n");
    meas_memcpy(memcpy_u256_unroll);

    /* MEMSET */
    printf("memset\n");
    meas_memset(memset);

    printf("memset_u8\n");
    meas_memset(memset_u8);

    printf("memset_u64\n");
    meas_memset(memset_u64);

    printf("memset_u256\n");
    meas_memset(memset_u256);
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("memcpy & memset");
        test_suite();
        TEST_SUMMARY();
    }

    if (argc == 2 && strcmp(argv[1], "pomiary") == 0)
    {
        meas();
        return 0;
    }

    /* tutaj mozecie sie pobawic, jakies wlasne testy, printf, scanf itp */

    return 0;
}