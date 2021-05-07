#ifndef OBJECT_CHAR_H
#define OBJECT_CHAR_H

#include <object-priv.h>

const char* char_get_name(void);
const char* char_to_chars(const void* object);

OBJECT_INTERFACE_CHECK_COMPATIBILITY(char, char)

#endif