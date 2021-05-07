#include <stdio.h>
#include <test_framework.h>
#include <generic_fmt.h>
#include <math.h>
#include <immintrin.h>

void tc_start(void);
void tc_clean(void);

void tc_start(void)
{
    printf("MOJA F DO INITU\n");
}

void tc_clean(void)
{
    printf("MOJA F DO SPRZATANIA\n");;
}

tc_ret_t f(void);
tc_ret_t g(int a);
tc_ret_t h(void);

tc_ret_t f(void)
{
    T_EXPECT(0 < 1);
    T_EXPECT_EQ(0ul, 1ul);
    T_EXPECT_NEQ((short)1 , (short)1);
    T_EXPECT_GT((char)'A', (char)'Z');
    /* T_ASSERT(0 == 1); */
    T_EXPECT_GEQ(2.78f, 3.14f);
    T_EXPECT_LEQ(100.01, sqrt(90.001));
    T_EXPECT_LT((size_t)1, (size_t)0);
    T_EXPECT(0 == 1);
    T_ASSERT(0 == 1);
}

tc_ret_t g(int a)
{
    T_EXPECT(a > 0);
    T_EXPECT_GT(a, 10);
    T_ASSERT_EQ(a, 11);
}

tc_ret_t h(void)
{
    for (;;);
}

typedef struct Foo
{
    int a;
    int b;
    double c;
    int t[];
} Foo;

Foo* foo_create(int a, int b, double c, int n)
{
    Foo* f = malloc(sizeof(*f) + (sizeof(*f->t) * n));
    f->a = a;
    f->b = b;
    f->c = c;
    memset(f->t, 0, sizeof(*f->t) * n);


    return f;
}

#define GENERIC(x, t1, v1, t2, v2) _Generic((x), t1: v1, t2: v2)
#define GENERIC2(x, t1, v1, t2, v2) \
    __builtin_choose_expr(__builtin_types_compatible_p(typeof(x), t1), v1, \
         __builtin_choose_expr(__builtin_types_compatible_p(typeof(x), t2), v2, 0))

int main(void)
{
    TEST_SUITE_INIT("Moje testy");
    TEST_CASE_RUN(f());
    TEST_CASE_RUN(g(11));
    /* TEST_CASE_RUN(h()); */
    TEST_SUITE_SUMMARY();
    int result = TEST_SUITE_GET_RESULT();

    TEST_SUITE_INIT_WITH_FUNC("Kolejna porcja testow", tc_start, tc_clean);
    TEST_CASE_RUN(g(11));
    TEST_CASE_RUN(g(11));
    TEST_SUITE_SUMMARY();

    typedef int my_int;
    int x;
    printf("%d\n", __builtin_types_compatible_p(int, my_int));
    printf("%d\n", GENERIC(x, int, 1, char, 2));
    printf("%d\n", GENERIC2(x, int, 1, char, 2));

    Foo *f = foo_create(1, 2, 3.14, 100);
    f->t[5] = 121;
    Foo f2 = *f;
    printf("%d\n", f2.t[5]);




    return result + TEST_SUITE_GET_RESULT();
}