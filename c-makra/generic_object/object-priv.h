#ifndef OBJECT_PRIV_H
#define OBJECT_PRIV_H

#include <stddef.h>

typedef const char* (*get_name_f)(void);
typedef const char* (*to_chars_f)(const void* object);
typedef void        (*delete_f)(void* object);

typedef struct Object Object;

const char* object_get_name(const Object *object);
const char* object_to_chars(const Object* object);
Object* object_new(void* object,
                   size_t object_size,
                   get_name_f get_name,
                   to_chars_f to_chars,
                   delete_f deletef);
void object_delete(Object* object);

#define OBJECT_INTERFACE_CHECK_COMPATIBILITY(className, classType) \
    static inline void object_interface_check_## className ## classType(void) \
    { \
        get_name_f get_name = className ## _get_name; \
        to_chars_f to_chars = className ## _to_chars; \
        (void)get_name; \
        (void)to_chars; \
    }

#endif