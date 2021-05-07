#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <test_framework-priv.h>

/* typy dla uzytkownika */
typedef void tc_ret_t;

/* MAKRA DLA UZYTKOWNIKA */

/* Uzyj tego makra raz do inicjalizacji test suity, fmt to nazwa ktora zostanie wyprintowana w headerze */
#define TEST_SUITE_INIT(fmt) __TEST_FRAMEWORK_INIT(fmt, NULL, NULL)

/* Uzyj tego makra tak jak poprzedniego, roznica jest tak za przed i po kazdym testcasem wykona sie Twoja funkcja, mozesz podac NULLA */
#define TEST_SUITE_INIT_WITH_FUNC(fmt, tc_init, tc_clean) __TEST_FRAMEWORK_INIT(fmt, tc_init, tc_clean);

/* Uzyj tego makra raz do podsumowania test suity, na ekranie pojawia sie statystyki testow */
#define TEST_SUITE_SUMMARY() __TEST_FRAMEWORK_SUMMARY()

/* Uzyj tego makra do puszczenia testcase */
#define TEST_CASE_RUN(function) __TEST_FRAMEWORK_TEST_TC(function)

/* Uzyj tego makra do pobrania rezulatatu testu (0 jesli passed != 0 jesli failed), przydatne do return w mainie */
#define TEST_SUITE_GET_RESULT() __TEST_FRAMEWORK_TEST_SUITE_GET_RESULT()

/*
    Uzyj tych makr do sprawdzenia czy val jest zgodne wedlug operacji z val2
    np czy val == val2
    czy val != val2
    czy val < val2

    Jesli warunek jest false to testcase jest FAILED ale dalej jest kontynuowany
*/
#define T_EXPECT_EQ(val, val2)  __TEST_FRAMEWORK_TEXPECT_EQ(val, val2)
#define T_EXPECT_NEQ(val, val2) __TEST_FRAMEWORK_TEXPECT_NEQ(val, val2)
#define T_EXPECT_GT(val, val2)  __TEST_FRAMEWORK_TEXPECT_GT(val, val2)
#define T_EXPECT_GEQ(val, val2) __TEST_FRAMEWORK_TEXPECT_GEQ(val, val2)
#define T_EXPECT_LT(val, val2)  __TEST_FRAMEWORK_TEXPECT_LT(val, val2)
#define T_EXPECT_LEQ(val, val2) __TEST_FRAMEWORK_TEXPECT_LEQ(val, val2)

/*
    Uzyj tego makra aby sprawdzic dowolny warunek
    np. T_EXPECT(list->head != NULL && (list->size == 0 || list->head->val == 5));
    Tak jak wszystki EXPECT nawet gdy jest blad kontynuujemy testcase
*/
#define T_EXPECT(cond)          __TEST_FRAMEWORK_TEXPECT_COND(cond)


/*
    Ten zestaw makr dziaja podobnie do EXPECT z ta roznica ze jesli
    cos pojdzie nie tak, to leci print i za nim od razu konczy sie testcase
    (nie testsuite). Stosuj te makra np do sprawdzania NULLa i innych warunkow
    ktore moga spowodowac crash
*/
#define T_ASSERT_EQ(val, val2)  __TEST_FRAMEWORK_TASSERT_EQ(val, val2)
#define T_ASSERT_NEQ(val, val2) __TEST_FRAMEWORK_TASSERT_NEQ(val, val2)
#define T_ASSERT_GT(val, val2)  __TEST_FRAMEWORK_TASSERT_GT(val, val2)
#define T_ASSERT_GEQ(val, val2) __TEST_FRAMEWORK_TASSERT_GEQ(val, val2)
#define T_ASSERT_LT(val, val2)  __TEST_FRAMEWORK_TASSERT_LT(val, val2)
#define T_ASSERT_LEQ(val, val2) __TEST_FRAMEWORK_TASSERT_LEQ(val, val2)
#define T_ASSERT(cond)          __TEST_FRAMEWORK_TASSERT_COND(cond)

#endif
