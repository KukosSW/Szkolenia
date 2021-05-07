
#include <insort.h>

/********************** TESTY ****************************/

#include <test_framework.h>
#include <stdlib.h>

static int cmp_int(const void* a, const void *b);
static int cmp_int_rev(const void *a, const void *b);

static int* array_create_random(size_t n);
static int* array_create_sorted(size_t n);
static int* array_create_reverse_sorted(size_t n);

static void test_suite(void);
static void test_suite_sort(void (*insort)(int[], size_t), size_t n);
static test_f test_case(void (*insort)(int[], size_t), int* (*array_create)(size_t), size_t n);

static int* array_create_random(size_t n)
{
    int *t = malloc(n * sizeof(*t));
    if (t == NULL)
        perror("Malloc error");

    srand((unsigned)time(NULL));
    for (size_t i = 0; i < n; ++i)
        t[i] = rand();

    return t;
}

static int cmp_int(const void* a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static int cmp_int_rev(const void *a, const void *b)
{
    return cmp_int(b, a);
}

static int* array_create_sorted(size_t n)
{
    int *t = array_create_random(n);

    qsort(t, n, sizeof(*t), cmp_int);

    return t;
}

static int* array_create_reverse_sorted(size_t n)
{
    int *t = array_create_random(n);

    qsort(t, n, sizeof(*t), cmp_int_rev);

    return t;
}

static test_f test_case(void (*insort)(int[], size_t), int* (*array_create)(size_t), size_t n)
{
    int *t = array_create(n);

    /* przygotuj tablice expected */
    int *expected = malloc(n * sizeof(*expected));
    if (expected == NULL)
        perror("Malloc error");

    memcpy(expected, t, sizeof(*expected) * n);
    qsort(expected, n, sizeof(*expected), cmp_int);


    /* testuj insorta */
    insort(t, n);

    /* sprawdz czy dobrze posortowal */
    T_EXPECT(memcmp(t, expected, sizeof(*t) * n), 0);

    free(t);
    free(expected);
}

static void test_suite_sort(void (*insort)(int[], size_t), size_t n)
{
    TEST(test_case(insort, array_create_random, n));
    TEST(test_case(insort, array_create_sorted, n));
    TEST(test_case(insort, array_create_reverse_sorted, n));
}

static void test_suite(void)
{
    test_suite_sort(insort_cormen, 0);
    test_suite_sort(insort_cormen, 1);
    test_suite_sort(insort_cormen, 10000);

    test_suite_sort(insort_memmove, 0);
    test_suite_sort(insort_memmove, 1);
    test_suite_sort(insort_memmove, 10000);

    test_suite_sort(insort_bs_memmove, 0);
    test_suite_sort(insort_bs_memmove, 1);
    test_suite_sort(insort_bs_memmove, 10000);

    test_suite_sort(insort_bs_memmove_bdir, 0);
    test_suite_sort(insort_bs_memmove_bdir, 1);
    test_suite_sort(insort_bs_memmove_bdir, 10000);
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
static void meas_sort(void (*insort)(int[], size_t), size_t n);

static void meas_sort(void (*insort)(int[], size_t), size_t n)
{
    int *t;

    t = array_create_random(n);
    MEASURE_FUNCTION(insort(t, n), "\tdane losowe");
    free(t);

    t = array_create_sorted(n);
    MEASURE_FUNCTION(insort(t, n), "\tdane posortowane");
    free(t);

    t = array_create_reverse_sorted(n);
    MEASURE_FUNCTION(insort(t, n), "\tdane odwrotnie posortowane");
    free(t);
}

void meas(void)
{
    const size_t n = 100000;

    printf("Cormen, n = %zu\n", n);
    meas_sort(insort_cormen, n);

    printf("Memmove, n = %zu\n", n);
    meas_sort(insort_memmove, n);

    printf("BS + Memmove, n = %zu\n", n);
    meas_sort(insort_bs_memmove, n);

    printf("BS + Memmove + BD, n = %zu\n", n);
    meas_sort(insort_bs_memmove_bdir, n);
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("Sortowanie przez wstawianie");
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