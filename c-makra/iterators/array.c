#include <array.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

struct Array
{
    int* t;
    size_t len;
};

static inline void array_init(Array* a, size_t len);
static inline void array_init(Array* a, size_t len)
{
    a->t = malloc(len * sizeof(a->t[0]));
    a->len = len;
}

Array* array_create(size_t len)
{
    Array *a = malloc(sizeof(Array));
    if (a == NULL)
        return NULL;

    array_init(a, len);

    return a;
}

void array_destroy(Array *a)
{
    free(a->t);
    free(a);
}

void array_set(Array* a, size_t index, int value)
{
    if (index >= a->len)
        return;

    a->t[index] = value;
}

int array_get(const Array* a, size_t index)
{
    if (index >= a->len)
        return 0;

    return a->t[index];
}

/* Iterator framework */

struct Array_iterator array_iterator_init(const Array *a, iterable_option_t option)
{
    return (struct Array_iterator){.array = a->t,
                                   .array_length = a->len,
                                   .index = (option == ITERABLE_OPTION_START_NORMAL ? 0 : (ssize_t)a->len - 1)};
}


void array_iterator_next(struct Array_iterator *iterator)
{
    ++iterator->index;
}

void array_iterator_prev(struct Array_iterator *iterator)
{
    --iterator->index;
}

bool array_iterator_is_end(const struct Array_iterator *iterator)
{
    return (iterator->index < 0 || (size_t)iterator->index >= iterator->array_length);
}

void array_iterator_get_value(const struct Array_iterator* iterator, void *local_addr)
{
    *(int *)local_addr = iterator->array[iterator->index];
}
