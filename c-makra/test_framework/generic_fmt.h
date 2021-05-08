#ifndef GENERIC_FMT_H
#define GENERIC_FMT_H

#define PRIMITIVE_GET_FMT(x) \
    _Generic((x), \
             _Bool: "%u", \
             char: "%c", \
             signed char: "%c", \
             unsigned char: "%c", \
             short: "%hd", \
             unsigned short: "%hu", \
             int: "%d", \
             unsigned int: "%u", \
             long: "%ld", \
             unsigned long: "%lu", \
             long long: "%ll", \
             unsigned long long: "%llu", \
             float: "%f", \
             double: "%lf", \
             long double: "%Lf", \
             default: "%p" \
    )

#endif