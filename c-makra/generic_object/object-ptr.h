#ifndef OBJECT_PTR_H
#define OBJECT_PTR_H

#include <object-priv.h>

const char* ptr_get_name(void);
const char* ptr_to_chars(const void* object);

OBJECT_INTERFACE_CHECK_COMPATIBILITY(ptr, ptr)

#endif