
#include <msearch.h>

/********************** TESTY ****************************/

#include <test_framework.h>
#include <stdlib.h>

static size_t cmp_n;

static int cmp_int(const void* a, const void *b);

static int* array_create_sorted(size_t n);
static int* array_create_sorted_1(size_t n);
static int* array_create_sorted_5(size_t n);

static void test_suite(void);

static int cmp_int(const void* a, const void *b)
{
    ++cmp_n;
    return *(int *)a - *(int *)b;
}

static int* array_create_sorted(size_t n)
{
    int *t = malloc(n * sizeof(*t));
    if (t == NULL)
        perror("Malloc error");

    for (size_t i = 0; i < n; ++i)
        t[i] = rand() + 1;

    qsort(t, n, sizeof(*t), cmp_int);

    return t;
}

static int* array_create_sorted_1(size_t n)
{
    int *t = malloc(n * sizeof(*t));
    if (t == NULL)
        perror("Malloc error");

    for (size_t i = 0; i < n; ++i)
        t[i] = (int)i;

    return t;
}

static int* array_create_sorted_5(size_t n)
{
    int *t = malloc(n * sizeof(*t));
    if (t == NULL)
        perror("Malloc error");

    for (size_t i = 0; i < n; ++i)
        t[i] = (int)i + 5;

    return t;
}


static test_f test_case(ssize_t (*search)(const int[], size_t, int, cmp_int_f), int* (*array_create)(size_t), size_t n)
{
    int *t = array_create(n);

    /* testuj search, gdy ma znalezc dane */
    for (size_t i = 0; i < n; ++i)
        T_EXPECT(search(t, n, t[i], cmp_int), (ssize_t)i);

    /* testuj gdy nie ma znalzc danych */
    for (size_t i = 0; i < n + 10; ++i)
        T_EXPECT(search(t, n, n > 0 ? t[n - 1] + (int)i + 1 : (int)i, cmp_int), (ssize_t)-1);

    free(t);
}

static void test_suite(void)
{
    TEST(test_case(search_linear, array_create_sorted_1, 0));
    TEST(test_case(search_linear, array_create_sorted_1, 1));
    TEST(test_case(search_linear, array_create_sorted_1, 10000));

    TEST(test_case(search_linear, array_create_sorted_5, 0));
    TEST(test_case(search_linear, array_create_sorted_5, 1));
    TEST(test_case(search_linear, array_create_sorted_5, 10000));

    TEST(test_case(search_binary, array_create_sorted_1, 0));
    TEST(test_case(search_binary, array_create_sorted_1, 1));
    TEST(test_case(search_binary, array_create_sorted_1, 10000));

    TEST(test_case(search_binary, array_create_sorted_5, 0));
    TEST(test_case(search_binary, array_create_sorted_5, 1));
    TEST(test_case(search_binary, array_create_sorted_5, 10000));

    TEST(test_case(search_interpolation, array_create_sorted_1, 0));
    TEST(test_case(search_interpolation, array_create_sorted_1, 1));
    TEST(test_case(search_interpolation, array_create_sorted_1, 10000));

    TEST(test_case(search_interpolation, array_create_sorted_5, 0));
    TEST(test_case(search_interpolation, array_create_sorted_5, 1));
    TEST(test_case(search_interpolation, array_create_sorted_5, 10000));
}

/********************************************************/

/********************* POMIARY **************************/
#include <stdio.h>

#define MEAS_N 10000

static void meas(void);
static void meas_search(ssize_t (*search)(const int[], size_t, int, cmp_int_f), size_t n);

static void meas_search(ssize_t (*search)(const int[], size_t, int, cmp_int_f), size_t n)
{
    int *t = array_create_sorted(n);

    cmp_n = 0;
    for (size_t i = 0; i < MEAS_N; ++i)
        search(t, n, t[(size_t)rand() % n], cmp_int);
    cmp_n /= MEAS_N;
    printf("\tZnaleziono: liczba porownan = %zu\n", cmp_n);

    cmp_n = 0;
    search(t, n, 0, cmp_int);
    printf("\tNie znaleziono: liczba porownan = %zu\n", cmp_n);

    free(t);
}

void meas(void)
{
    srand((unsigned)time(NULL));
    const size_t n = 1000;

    printf("Liniowe n = %zu\n", n);
    meas_search(search_linear, n);

    printf("Binarne n = %zu\n", n);
    meas_search(search_binary, n);

    printf("Interpolacyjne n = %zu\n", n);
    meas_search(search_interpolation, n);
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("Wyszukiwania");
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