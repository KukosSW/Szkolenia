#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef enum PRIMITIVES
{
    PRIMITIVES_BOOL,
    PRIMITIVES_CHAR,
    PRIMITIVES_SIGNED_CHAR,
    PRIMITIVES_UNSIGNED_CHAR,
    PRIMITIVES_SHORT,
    PRIMITIVES_UNSIGNED_SHORT,
    PRIMITIVES_INT,
    PRIMITIVES_UNSIGNED_INT,
    PRIMITIVES_LONG,
    PRIMITIVES_UNSIGNED_LONG,
    PRIMITIVES_LONG_LONG,
    PRIMITIVES_UNSIGNED_LONG_LONG,
    PRIMITIVES_FLOAT,
    PRIMITIVES_DOUBLE,
    PRIMITIVES_LONG_DOUBLE,
    PRIMITIVES_NON_PRIMITIVE
} PRIMITIVES;

/* Gdybym mial gnu, to takie makro jest idealne potem do switch w innych makrach */
#define PRIMITIVE_GET_TYPE(x) \
    _Generic((x), \
             _Bool: PRIMITIVES_BOOL, \
             char: PRIMITIVES_CHAR, \
             signed char: PRIMITIVES_SIGNED_CHAR, \
             unsigned char: PRIMITIVES_UNSIGNED_CHAR, \
             short: PRIMITIVES_SHORT, \
             unsigned short: PRIMITIVES_UNSIGNED_SHORT, \
             int: PRIMITIVES_INT, \
             unsigned int: PRIMITIVES_UNSIGNED_INT, \
             long: PRIMITIVES_LONG, \
             unsigned long: PRIMITIVES_UNSIGNED_LONG, \
             long long: PRIMITIVES_LONG_LONG, \
             unsigned long long: PRIMITIVES_UNSIGNED_LONG_LONG, \
             float: PRIMITIVES_FLOAT, \
             double: PRIMITIVES_DOUBLE, \
             long double: PRIMITIVES_LONG_DOUBLE, \
             default: PRIMITIVES_NON_PRIMITIVE \
    )

#define IS_PRIMITIVE(x)     (PRIMITIVE_GET_TYPE(x) != PRIMITIVES_NON_PRIMITIVE)
#define IS_NON_PRIMITIVE(x) (PRIMITIVE_GET_TYPE(x) == PRIMITIVES_NON_PRIMITIVE)

#endif