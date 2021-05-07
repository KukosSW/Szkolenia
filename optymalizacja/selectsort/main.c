
#include <selectsort.h>

/********************** TESTY ****************************/

#include <test_framework.h>
#include <stdlib.h>

static int cmp_int(const void* a, const void *b);
static int cmp_int_rev(const void *a, const void *b);

static int* array_create_random(size_t n);
static int* array_create_sorted(size_t n);
static int* array_create_reverse_sorted(size_t n);

static void test_suite(void);
static void test_suite_sort(void (*selectsort)(int[], size_t), size_t n);
static test_f test_case(void (*selectsort)(int[], size_t), int* (*array_create)(size_t), size_t n);

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

static test_f test_case(void (*selectsort)(int[], size_t), int* (*array_create)(size_t), size_t n)
{
    int *t = array_create(n);

    /* przygotuj tablice expected */
    int *expected = malloc(n * sizeof(*expected));
    if (expected == NULL)
        perror("Malloc error");

    memcpy(expected, t, sizeof(*expected) * n);
    qsort(expected, n, sizeof(*expected), cmp_int);


    /* testuj selectsorta */
    selectsort(t, n);

    /* sprawdz czy dobrze posortowal */
    T_EXPECT(memcmp(t, expected, sizeof(*t) * n), 0);

    free(t);
    free(expected);
}

static void test_suite_sort(void (*selectsort)(int[], size_t), size_t n)
{
    TEST(test_case(selectsort, array_create_random, n));
    TEST(test_case(selectsort, array_create_sorted, n));
    TEST(test_case(selectsort, array_create_reverse_sorted, n));
}

static void test_suite(void)
{
    test_suite_sort(selectsort_cormen_max, 0);
    test_suite_sort(selectsort_cormen_max, 1);
    test_suite_sort(selectsort_cormen_max, 10000);

    test_suite_sort(selectsort_cormen_min, 0);
    test_suite_sort(selectsort_cormen_min, 1);
    test_suite_sort(selectsort_cormen_min, 10000);

    test_suite_sort(selectsort_cormen_min2, 0);
    test_suite_sort(selectsort_cormen_min2, 1);
    test_suite_sort(selectsort_cormen_min2, 10000);

    test_suite_sort(selectsort_minmax, 0);
    test_suite_sort(selectsort_minmax, 1);
    test_suite_sort(selectsort_minmax, 10000);

    test_suite_sort(selectsort_minmax2, 0);
    test_suite_sort(selectsort_minmax2, 1);
    test_suite_sort(selectsort_minmax2, 10000);

    test_suite_sort(selectsort_subarrays, 0);
    test_suite_sort(selectsort_subarrays, 1);
    test_suite_sort(selectsort_subarrays, 10000);
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
static void meas_sort(void (*selectsort)(int[], size_t), size_t n);

static void meas_sort(void (*selectsort)(int[], size_t), size_t n)
{
    int *t;

    t = array_create_random(n);
    MEASURE_FUNCTION(selectsort(t, n), "\tdane losowe");
    free(t);

    t = array_create_sorted(n);
    MEASURE_FUNCTION(selectsort(t, n), "\tdane posortowane");
    free(t);

    t = array_create_reverse_sorted(n);
    MEASURE_FUNCTION(selectsort(t, n), "\tdane odwrotnie posortowane");
    free(t);
}

void meas(void)
{
    const size_t n = 100000;

    printf("Cormen max, n = %zu\n", n);
    meas_sort(selectsort_cormen_max, n);

    printf("Cormen min, n = %zu\n", n);
    meas_sort(selectsort_cormen_min, n);

    printf("Cormen min v2, n = %zu\n", n);
    meas_sort(selectsort_cormen_min2, n);

    printf("min max, n = %zu\n", n);
    meas_sort(selectsort_minmax, n);

    printf("min max v2, n = %zu\n", n);
    meas_sort(selectsort_minmax2, n);

    printf("subarray max, n = %zu\n", n);
    meas_sort(selectsort_subarrays, n);
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("Sortowanie przez wybor");
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
