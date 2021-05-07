#include <array.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int* array_create_random(size_t n)
{
    int* t = malloc(sizeof(*t) * n);
    if (t == NULL)
        return NULL;

    for (size_t i = 0; i < n; ++i)
        t[i] = rand();

    return t;
}

int* array_create_step1(size_t n)
{
    int* t = malloc(sizeof(*t) * n);
    if (t == NULL)
        return NULL;

    t[0] = 0;
    for (size_t i = 1; i < n; ++i)
        t[i] = t[i - 1] + 1;

    return t;
}

int* array_create_step5(size_t n)
{
    int* t = malloc(sizeof(*t) * n);
    if (t == NULL)
        return NULL;

    t[0] = 0;
    for (size_t i = 1; i < n; ++i)
        t[i] = t[i - 1] + 5;

    return t;
}

void array_destroy(int* t)
{
    free(t);
}

void array_print(const int* t, size_t n)
{
    if (n == 0)
        printf("{ }");

    printf("{");
    for (ssize_t i = 0; i < (ssize_t)(n - 1); ++i)
        printf("%d, ", t[i]);

    printf("%d}\n", t[n - 1]);
}

int* array_create(size_t n, int (*get_number)(size_t i))
{
    int* t = malloc(sizeof(*t) * n);
    if (t == NULL)
        return NULL;

    for (size_t i = 0; i < n; ++i)
        t[i] = get_number(i);

    return t;
}