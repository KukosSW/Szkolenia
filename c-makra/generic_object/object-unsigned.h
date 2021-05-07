#ifndef OBJECT_UNSIGNED_H
#define OBJECT_UNSIGNED_H

#include <object-priv.h>

const char* unsigned_get_name(void);
const char* unsigned_to_chars(const void* object);

OBJECT_INTERFACE_CHECK_COMPATIBILITY(unsigned, unsigned)

#endif