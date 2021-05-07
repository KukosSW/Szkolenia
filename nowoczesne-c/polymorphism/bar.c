#include <bar.h>
#include <stdio.h>
#include <stdlib.h>

static void bar_print_wrapper(const void* bar)
{
    bar_print(bar);
}

static void bar_delete_wrapper(void* bar)
{
    free(bar);
}

void bar_print(const Bar* bar)
{
    printf("Bar = {%lf, %d, %c}\n", bar->a, bar->b, bar->c);
}

Object* bar_create_object(double a, int b, char c)
{
    Bar* bar = malloc(sizeof(*bar));
    bar->a = a;
    bar->b = b;
    bar->c = c;

    return object_create(bar, bar_get_name, bar_print_wrapper, bar_delete_wrapper);
}