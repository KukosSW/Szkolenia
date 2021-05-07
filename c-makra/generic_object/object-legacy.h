#ifndef OBJECT_LEGACY_H
#define OBJECT_LEGACY_H

#include <object-priv.h>

const char* legacy_get_name(void);
const char* legacy_to_chars(const void* object);

OBJECT_INTERFACE_CHECK_COMPATIBILITY(legacy, legacy)

#endif