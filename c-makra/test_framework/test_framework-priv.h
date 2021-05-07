#ifndef TEST_FRAMEWORK_PRIV_H
#define TEST_FRAMEWORK_PRIV_H

/* includy wszystkie co potrzeba */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <terminal_colors.h>
#include <generic_fmt.h>
#include <stddef.h>

/* DEKLARUJEMY SWOJE MAKRA DO TWORZENIA FRAMEWORKU */
#ifndef TOSTRING
#define _TOSTRING(x) #x
#define TOSTRING(x) _TOSTRING(x)
#endif

/* Nowe typy definiowane przez framework */

typedef void (*test_case_init_f)(void);
typedef void (*test_case_clean_f)(void);

typedef unsigned long test_framework_counter_t;
typedef enum
{
    TEST_FRAMEWORK_TC_RESULT_PASSED,
    TEST_FRAMEWORK_TC_RESULT_FAILED,
} test_framework_tc_result_t;

typedef enum
{
    TEST_FRAMEWORK_UNINITIALIZED,
    TEST_FRAMEWORK_STARTED,
} test_framework_states_t;

/*
    Zmienne dla frameworku, testy maja byc w 1 pliku. 1 testsuite = 1plik
    dlatego mozemy wpisac zmienne jako static a nie extern + plik .c do frameworku
*/
static test_case_init_f test_framework_init_case; /* call before each TC */
static test_case_clean_f test_framework_clean_case; /* call after each TC */
static test_framework_counter_t test_framework_passed_counter;
static test_framework_counter_t test_framework_failed_counter;
static test_framework_tc_result_t test_framework_tc_result;
static test_framework_states_t test_framework_state = TEST_FRAMEWORK_UNINITIALIZED;

#define __TEST_FRAMEWORK_STRING_MAX_LEN 60
static  char test_framework_spaces[__TEST_FRAMEWORK_STRING_MAX_LEN];
static  char test_framework_equals[__TEST_FRAMEWORK_STRING_MAX_LEN];

/* PRIVATE MACROS */
#define __TEST_FRAMEWORK_COLOR_INFO          COLOR_CYAN
#define __TEST_FRAMEWORK_COLOR_PASSED        COLOR_GREEN
#define __TEST_FRAMEWORK_COLOR_FAILED        COLOR_RED
#define __TEST_FRAMEWORK_SUMMARY_TEXT        "TEST SUMMARY"
#define __TEST_FRAMEWORK_SUMMARY_TEXT_LEN    (strlen(__TEST_FRAMEWORK_SUMMARY_TEXT))
#define __TEST_FRAMEWORK_PRINT_INFO(fmt)     __TEST_FRAMEWORK_COLOR_INFO fmt COLOR_RESET
#define __TEST_FRAMEWORK_PRINT_PASSED(fmt)   __TEST_FRAMEWORK_COLOR_PASSED fmt COLOR_RESET
#define __TEST_FRAMEWORK_PRINT_FAILED(fmt)   __TEST_FRAMEWORK_COLOR_FAILED fmt COLOR_RESET

/* PRINT === fmt == w taki sposob aby wszystko zajelo 60 znakow a fmt bylo wycentrowane */
#define __TEST_FRAMEWORK_PRINT_HEADER(fmt) \
    do { \
        const int test_framework_fmt_len_with_spaces = (int)strlen(fmt) + 2; \
        const int test_framework_equals_left_len = (__TEST_FRAMEWORK_STRING_MAX_LEN - test_framework_fmt_len_with_spaces + 1) / 2; \
        const int test_framework_equals_right_len = (__TEST_FRAMEWORK_STRING_MAX_LEN - test_framework_fmt_len_with_spaces) / 2; \
        fprintf(stderr, \
                __TEST_FRAMEWORK_PRINT_INFO("%.*s %s %.*s\n"), \
                test_framework_equals_left_len, \
                test_framework_equals, \
                fmt, \
                test_framework_equals_right_len, \
                test_framework_equals); \
    } while (0)

#define __TEST_FRAMEWORK_INIT(fmt, tc_init, tc_clean) \
    do { \
        if (test_framework_state != TEST_FRAMEWORK_UNINITIALIZED) \
        { \
            fprintf(stderr, "Skoncz poprzedni test suite zanim zaczniesz kolejny\n"); \
            exit(1); \
        } \
        test_framework_passed_counter = 0; \
        test_framework_failed_counter = 0; \
        test_framework_init_case = tc_init; \
        test_framework_clean_case = tc_clean; \
        memset(&test_framework_spaces, ' ', sizeof(test_framework_spaces)); \
        memset(&test_framework_equals, '=', sizeof(test_framework_equals)); \
        srand((unsigned)time(NULL)); \
        __TEST_FRAMEWORK_PRINT_HEADER(fmt); \
        test_framework_state = TEST_FRAMEWORK_STARTED; \
    } while (0)


/* Makro wywoluje dowolne makro do sprawdzania warunkow w tescie, konczy testcase jesli cos poszlo nie tak */
#define __TEST_FRAMEWORK_RETURN_IF_TERROR(TMACRO) \
    do { \
        TMACRO; \
        if (test_framework_tc_result == TEST_FRAMEWORK_TC_RESULT_FAILED) \
        { \
            return; \
        } \
    } while (0)

/* Jesli expr == false to print i tc=failed, kontynuuje tc */
#define __TEST_FRAMEWORK_TEXPECT_OR_ASSERT_COND(cond, label) \
    do { \
        if (!(cond)) \
        { \
            fprintf(stderr, "[" label "] %s:%d\t%s\n", __FILE__, __LINE__, TOSTRING(cond)); \
            test_framework_tc_result = TEST_FRAMEWORK_TC_RESULT_FAILED; \
        } \
    } while (0)

/* Jesli !(val op val2) to print i tc failed */
#define __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val, val2, op, label) \
    do { \
            const typeof(val) test_framework_val = (val); \
            const typeof(val2) test_framework_val2 = (val2); \
            (void)(&test_framework_val - &test_framework_val2); \
            char test_framework_print_fmt[256] = {"[" label "] %s:%d\t"}; \
            strcat(&test_framework_print_fmt[0], PRIMITIVE_GET_FMT(test_framework_val)); \
            strcat(&test_framework_print_fmt[0], " " TOSTRING(op) " "); \
            strcat(&test_framework_print_fmt[0], PRIMITIVE_GET_FMT(test_framework_val2)); \
            strcat(&test_framework_print_fmt[0], "\n"); \
            if (!(test_framework_val op test_framework_val2)) \
            { \
                fprintf(stderr, \
                        test_framework_print_fmt, \
                        __FILE__, \
                        __LINE__, \
                        test_framework_val, \
                        test_framework_val2); \
                test_framework_tc_result = TEST_FRAMEWORK_TC_RESULT_FAILED; \
            } \
    } while (0)

/* Zbior predefiniowanych operacji ==, !=, < ... */
#define __TEST_FRAMEWORK_TEXPECT_EQ(val1, val2)  __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, ==, "T_EXPECT")
#define __TEST_FRAMEWORK_TEXPECT_NEQ(val1, val2) __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, !=, "T_EXPECT")
#define __TEST_FRAMEWORK_TEXPECT_LT(val1, val2)  __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, <, "T_EXPECT")
#define __TEST_FRAMEWORK_TEXPECT_LEQ(val1, val2) __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, <=, "T_EXPECT")
#define __TEST_FRAMEWORK_TEXPECT_GT(val1, val2)  __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, >, "T_EXPECT")
#define __TEST_FRAMEWORK_TEXPECT_GEQ(val1, val2) __TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, >=, "T_EXPECT")
#define __TEST_FRAMEWORK_TEXPECT_COND(cond)      __TEST_FRAMEWORK_TEXPECT_OR_ASSERT_COND(cond, "T_EXPECT")

#define __TEST_FRAMEWORK_TASSERT_EQ(val1, val2)  __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, ==, "T_ASSERT"))
#define __TEST_FRAMEWORK_TASSERT_NEQ(val1, val2) __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, !=, "T_ASSERT"))
#define __TEST_FRAMEWORK_TASSERT_LT(val1, val2)  __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, <, "T_ASSERT"))
#define __TEST_FRAMEWORK_TASSERT_LEQ(val1, val2) __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, <=, "T_ASSERT"))
#define __TEST_FRAMEWORK_TASSERT_GT(val1, val2)  __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, >, "T_ASSERT"))
#define __TEST_FRAMEWORK_TASSERT_GEQ(val1, val2) __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT(val1, val2, >=, "T_ASSERT"))
#define __TEST_FRAMEWORK_TASSERT_COND(cond)      __TEST_FRAMEWORK_RETURN_IF_TERROR(__TEST_FRAMEWORK_TEXPECT_OR_ASSERT_COND(cond, "T_ASSERT"))

/* HELPER do TEST_TC, sluzy do printowania komunikatu */
#define __TEST_FRAMEWORK_TEST_TC_HELPER(msg, tc_str) \
    fprintf(stderr, \
            __TEST_FRAMEWORK_PRINT_INFO("[TEST] %s%.*s") \
            msg "\n", \
            tc_str, \
            (int)(__TEST_FRAMEWORK_STRING_MAX_LEN - strlen(tc_str) - strlen("[TEST ] ")), \
            test_framework_spaces) \


/* Makro do testowania testcase */
#define __TEST_FRAMEWORK_TEST_TC(tc) \
    do { \
        if (test_framework_state != TEST_FRAMEWORK_STARTED) \
        { \
            fprintf(stderr, "Wykonaj TEST_SUITE_INIT zanim zaczniesz puszczac test case\n"); \
            exit(1); \
        } \
        if (test_framework_init_case != NULL) \
        { \
            test_framework_init_case(); \
        } \
        __TEST_FRAMEWORK_TEST_TC_HELPER(__TEST_FRAMEWORK_PRINT_INFO("STARTED"), TOSTRING(tc)); \
        test_framework_tc_result = TEST_FRAMEWORK_TC_RESULT_PASSED; \
        tc; \
        if (test_framework_clean_case != NULL) \
        { \
            test_framework_clean_case(); \
        } \
        if (test_framework_tc_result == TEST_FRAMEWORK_TC_RESULT_PASSED) \
        { \
            ++test_framework_passed_counter; \
            __TEST_FRAMEWORK_TEST_TC_HELPER(__TEST_FRAMEWORK_PRINT_PASSED("PASSED"), TOSTRING(tc)); \
        } \
        else \
        { \
            ++test_framework_failed_counter; \
            __TEST_FRAMEWORK_TEST_TC_HELPER(__TEST_FRAMEWORK_PRINT_FAILED("FAILED"), TOSTRING(tc)); \
        } \
    } while (0)

#define __TEST_FRAMEWORK_SUMMARY_HELPER(COLOR, STR_RESULT) \
    do { \
        const int test_framework_fmt_len_with_spaces = __TEST_FRAMEWORK_SUMMARY_TEXT_LEN + 2; \
        const int test_framework_equals_left_len = (__TEST_FRAMEWORK_STRING_MAX_LEN - test_framework_fmt_len_with_spaces + 1) / 2; \
        const int test_framework_equals_right_len = (__TEST_FRAMEWORK_STRING_MAX_LEN - test_framework_fmt_len_with_spaces) / 2; \
        const test_framework_counter_t test_framework_tests_executed = test_framework_passed_counter + test_framework_failed_counter; \
        fprintf(stderr, \
                COLOR "%.*s" " "__TEST_FRAMEWORK_SUMMARY_TEXT " " "%.*s\n", \
                test_framework_equals_left_len, \
                test_framework_equals, \
                test_framework_equals_right_len, \
                test_framework_equals); \
        fprintf(stderr, \
                "TESTS EXECUTED:\t%ld\n", \
                test_framework_tests_executed); \
        fprintf(stderr, \
                "PASSED:\t\t%ld / %ld\n", \
                test_framework_passed_counter, \
                test_framework_tests_executed); \
        fprintf(stderr, \
                "FAILED:\t\t%ld / %ld\n", \
                test_framework_failed_counter, \
                test_framework_tests_executed); \
        fprintf(stderr, STR_RESULT); \
        fprintf(stderr, \
                "%.*s\n" COLOR_RESET, \
                __TEST_FRAMEWORK_STRING_MAX_LEN, \
                test_framework_equals); \
    } while(0)

#define __TEST_FRAMEWORK_SUMMARY() \
    do { \
        if (test_framework_state != TEST_FRAMEWORK_STARTED) \
        { \
            fprintf(stderr, "Wykonaj TEST_SUITE_INIT zanim wykonasz podsumowanie\n"); \
            exit(1); \
        } \
        if (test_framework_failed_counter) \
        { \
            __TEST_FRAMEWORK_SUMMARY_HELPER(__TEST_FRAMEWORK_COLOR_FAILED, "TESTS FAILED\n"); \
        } \
        else \
        { \
            __TEST_FRAMEWORK_SUMMARY_HELPER(__TEST_FRAMEWORK_COLOR_PASSED, "TESTS PASSED\n"); \
        } \
        test_framework_state = TEST_FRAMEWORK_UNINITIALIZED; \
    } while(0)

#define __TEST_FRAMEWORK_TEST_SUITE_GET_RESULT() (int)test_framework_failed_counter

#endif