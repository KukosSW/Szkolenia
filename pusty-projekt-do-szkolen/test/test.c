#include <ktest/ktest.h>
#include <foo.h>

static tc_ret_t foo_get_test(void);

static tc_ret_t foo_get_test(void)
{
    const int n = foo_get_n();

    T_ASSERT_EQ(n, 41);
}

int main(void)
{
    TEST_SUITE_INIT("Foo testsuite");
    TEST_CASE_RUN(foo_get_test());
    TEST_SUITE_SUMMARY();

    return TEST_SUITE_GET_RESULT();
}