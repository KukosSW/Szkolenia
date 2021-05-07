#include <fib.h>

/********************** TESTY ****************************/

#include <test_framework.h>
/* tyle bylo latwo wkleic z neta :) */
static uint64_t fib_seq[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811};
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static test_f test_fib(uint64_t (*fib)(uint64_t));
static void test_suite(void);



static test_f test_fib(uint64_t (*fib)(uint64_t))
{
    size_t len = ARRAY_SIZE(fib_seq);
    for (size_t i = 0; i < len; ++i)
        T_EXPECT(fib(i), fib_seq[i]);
}

static void test_suite(void)
{
    TEST(test_fib(fib_rec));
    TEST(test_fib(fib_rec_mem));
    TEST(test_fib(fib_ite_mem));
    TEST(test_fib(fib_ite));
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("Fibonacci");
        test_suite();
        TEST_SUMMARY();
    }

    /* tutaj mozecie sie pobawic, jakies wlasne testy, printf, scanf itp */

    return 0;
}
