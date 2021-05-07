#ifndef OBJECT_INT_H
#define OBJECT_INT_H

#include <object-priv.h>

const char* int_get_name(void);
const char* int_to_chars(const void* object);

OBJECT_INTERFACE_CHECK_COMPATIBILITY(int, int)

#endif