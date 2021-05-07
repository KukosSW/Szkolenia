
#include <unique.h>

/********************** TESTY ****************************/

#include <test_framework.h>
#include <stdlib.h>

static test_f test_unique_empty(size_t (*unique_f)(const int[], size_t));
static test_f test_unique(size_t (*unique_f)(const int[], size_t));

static void test_suite(void);

static test_f test_unique_empty(size_t (*unique_f)(const int[], size_t))
{
    int t[] = {0};

    T_EXPECT(unique_f(t, 0), (size_t)0);
}

static test_f test_unique(size_t (*unique_f)(const int[], size_t))
{
    int t[] = {0, 10, 9, 9, 3, 5, 7, 1, 9 ,9, 0, 3, 7, 1, 10, 0, 6};

    T_EXPECT(unique_f(t, sizeof(t) / sizeof(t[0])), (size_t)8);
}

static void test_suite(void)
{
    TEST(test_unique_empty(unique_count_erase));
    TEST(test_unique(unique_count_erase));

    TEST(test_unique_empty(unique_count));
    TEST(test_unique(unique_count));

    TEST(test_unique_empty(unique_count_sort));
    TEST(test_unique(unique_count_sort));

    TEST(test_unique_empty(unique_set));
    TEST(test_unique(unique_set));

    TEST(test_unique_empty(unique_hashset));
    TEST(test_unique(unique_hashset));

    TEST(test_unique_empty(unique_cpp));
    TEST(test_unique(unique_cpp));
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

#define _SWAP(a, b, type) \
    do { \
        type ____temp = (a); \
        a = (b); \
        b = ____temp; \
    } while (0)

#define SWAP(a, b) _SWAP(a, b, int)

static void meas(void);
static void meas_unique_random(size_t n, size_t (*unique_f)(const int[], size_t));
static void meas_unique_all_same(size_t n, size_t (*unique_f)(const int[], size_t));
static void meas_unique_all_diff(size_t n, size_t (*unique_f)(const int[], size_t));

static void meas_unique_random(size_t n, size_t (*unique_f)(const int[], size_t))
{
    int* t = malloc(n * sizeof(*t));

    srand((unsigned)(time(NULL)));

    for (size_t i = 0; i < n; ++i)
        t[i] = rand();

    (void)unique_f(t, n);

    free(t);
}

static void meas_unique_all_diff(size_t n, size_t (*unique_f)(const int[], size_t))
{
    int* t = malloc(n * sizeof(*t));

    for (size_t i = 0; i < n; ++i)
        t[i] = (int)i + 1;

    /* pomieszaj kolejnosc */
    for (size_t i = 0; i < n; ++i)
    {
        const size_t pos = (size_t)rand() % (n - i);
        SWAP(t[pos], t[n - i - 1]);
    }

    (void)unique_f(t, n);

    free(t);
}

static void meas_unique_all_same(size_t n, size_t (*unique_f)(const int[], size_t))
{
    int* t = malloc(n * sizeof(*t));
    memset(t, 0xabcd, sizeof(*t) * n);

    (void)unique_f(t, n);

    free(t);
}


void meas(void)
{
    const size_t n = 100000;

    printf("Naiwne zliczanie z usuwaniem (n = %zu)\n", n);
    MEASURE_FUNCTION(meas_unique_random(n, unique_count_erase), "\tlosowe");
    MEASURE_FUNCTION(meas_unique_all_diff(n, unique_count_erase), "\twszystkie rozne");
    MEASURE_FUNCTION(meas_unique_all_same(n, unique_count_erase), "\twszystkie takie same");

    printf("Troche mniej naiwne (n = %zu)\n", n);
    MEASURE_FUNCTION(meas_unique_random(n, unique_count), "\tlosowe");
    MEASURE_FUNCTION(meas_unique_all_diff(n, unique_count), "\twszystkie rozne");
    MEASURE_FUNCTION(meas_unique_all_same(n, unique_count), "\twszystkie takie same");

    printf("Sortowanie i zliczanie (n = %zu)\n", n);
    MEASURE_FUNCTION(meas_unique_random(n, unique_count_sort), "\tlosowe");
    MEASURE_FUNCTION(meas_unique_all_diff(n, unique_count_sort), "\twszystkie rozne");
    MEASURE_FUNCTION(meas_unique_all_same(n, unique_count_sort), "\twszystkie takie same");

    printf("Posortowany zbior (n = %zu)\n", n);
    MEASURE_FUNCTION(meas_unique_random(n, unique_set), "\tlosowe");
    MEASURE_FUNCTION(meas_unique_all_diff(n, unique_set), "\twszystkie rozne");
    MEASURE_FUNCTION(meas_unique_all_same(n, unique_set), "\twszystkie takie same");

    printf("Hashowany zbior (n = %zu)\n", n);
    MEASURE_FUNCTION(meas_unique_random(n, unique_hashset), "\tlosowe");
    MEASURE_FUNCTION(meas_unique_all_diff(n, unique_hashset), "\twszystkie rozne");
    MEASURE_FUNCTION(meas_unique_all_same(n, unique_hashset), "\twszystkie takie same");

    printf("C++ (n = %zu)\n", n);
    MEASURE_FUNCTION(meas_unique_random(n, unique_cpp), "\tlosowe");
    MEASURE_FUNCTION(meas_unique_all_diff(n, unique_cpp), "\twszystkie rozne");
    MEASURE_FUNCTION(meas_unique_all_same(n, unique_cpp), "\twszystkie takie same");
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("Zliczanie unikatowych elementow");
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