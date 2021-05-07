#ifndef OBJECT_PRIMITIVES_H
#define OBJECT_PRIMITIVES_H

#include <primitives.h>
#include <object-char.h>
#include <object-int.h>
#include <object-unsigned.h>
#include <object-ptr.h>

static inline const char* trash_get_name(void)
{
    return "";
}

static inline const char* trash_to_chars(const void* object)
{
    (void)object;
    return "";
}

#define OBJECT_PRIMITIVES_GET_NAME(x) \
    _Generic((x), \
             char: char_get_name, \
             int: int_get_name, \
             unsigned int: unsigned_get_name, \
             default: trash_get_name \
    )

#define OBJECT_PRIMITIVES_TO_CHARS(x) \
    _Generic((x), \
             char: char_to_chars, \
             int: int_to_chars, \
             unsigned int: unsigned_to_chars, \
             default: trash_get_name \
    )

#endif