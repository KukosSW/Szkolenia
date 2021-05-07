#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct Array Array;

Array* array_create(size_t len);
void array_destroy(Array* a);
void array_set(Array* a, size_t index, int value);
int array_get(const Array* a, size_t index);

#include <interface-iterable.h>
#include <sys/types.h>
#include <stdbool.h>

struct Array_iterator
{
    int*        array;
    ssize_t     index;
    size_t      array_length;
};

struct Array_iterator array_iterator_init(const Array *a, iterable_option_t option);
void array_iterator_next(struct Array_iterator *iterator);
void array_iterator_prev(struct Array_iterator *iterator);
bool array_iterator_is_end(const struct Array_iterator *iterator);
void array_iterator_get_value(const struct Array_iterator* iterator, void *local_addr);

INTERFACE_ITERABLE_CHECK_COMPATIBILITY(array, Array)
INTERFACE_ITERABLE_PRIV_HEADER(array, Array)

#endif