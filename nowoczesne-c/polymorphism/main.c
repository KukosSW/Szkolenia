#include <stdio.h>
#include <array.h>
#include <stdlib.h>
#include <static_poly.h>
#include <dynamic_poly.h>
#include <foo.h>
#include <bar.h>

CMP_CREATE_GENERIC(int, cmp_int)

static void test_array(void);
static void test_static_poly(void);
static void test_dynamic_poly(void);

static int get_random(size_t i);
static int get_step1(size_t i);
static int get_step5(size_t i);

static int get_random(size_t i)
{
    (void)i;
    return rand() % 100;
}

static int get_step1(size_t i)
{
    return (int)i;
}

static int get_step5(size_t i)
{
    return (int)i * 5;
}

static void test_array(void)
{
    const size_t n = 10;
    int* t;

    t = array_create_random(n);
    array_print(t, n);
    array_destroy(t);

    t = array_create_step1(n);
    array_print(t, n);
    array_destroy(t);

    t = array_create_step5(n);
    array_print(t, n);
    array_destroy(t);

    printf("\n");
    int (*get_number[3])(size_t) = {get_random, get_step1, get_step5};
    for (size_t i = 0; i < 3; ++i)
    {
        t = array_create(n, get_number[i]);
        array_print(t, n);
        array_destroy(t);
    }
    printf("\n");
}

static void test_static_poly(void)
{
    const size_t n = 10;
    int* t;

    t = array_create(n, get_random);
    array_print(t, n);

    qsort(t, n, sizeof(*t), cmp_int);
    array_print(t, n);

    array_destroy(t);
    printf("\n");
}

static void test_dynamic_poly(void)
{
    Object* objects[2] = {foo_create_object(10, 20), bar_create_object(3.14, 41, 'C')};

    for (size_t i = 0; i < 2; ++i)
    {
        printf("Object %zu to: %s\n", i, object_get_name(objects[i]));
        object_print(objects[i]);
        object_destroy(objects[i]);
    }
    printf("\n");
}

int main(void)
{
    test_static_poly();
    test_array();
    test_dynamic_poly();

    return 0;
}