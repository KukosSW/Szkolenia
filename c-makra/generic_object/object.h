#ifndef OBJECT_H
#define OBJECT_H

#include <object-priv.h>
#include <object-primitives.h>
#include <object-legacy.h>
#include <stdbool.h>
#define object_primitive_new(obj) \
    object_new(&obj, \
               sizeof(obj), \
               OBJECT_PRIMITIVES_GET_NAME(obj), \
               OBJECT_PRIMITIVES_TO_CHARS(obj), \
               NULL)

#define object_non_primitive_new(obj, className) \
    object_new(&obj, \
               sizeof(obj), \
               className ## _get_name, \
               className ## _to_chars, \
               NULL)

#define object_ptr_new(obj, destructor) \
    object_new(&obj, \
               sizeof(obj), \
               ptr_get_name, \
               ptr_to_chars, \
               destructor)

#define object_legacy_new(obj) \
    object_new(&obj, \
               sizeof(obj), \
               legacy_get_name, \
               legacy_to_chars, \
               NULL)
#endif