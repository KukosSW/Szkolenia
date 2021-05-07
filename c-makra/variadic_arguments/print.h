#ifndef PRINT_H
#define PRINT_H

#define PRINT_WEAK(fmt, ...) printf(fmt, __VA_ARGS__)
#define PRINT_GCC(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define PRINT(...) printf(__VA_ARGS__)

#define __PRINT_HEAD(X, ...) X
#define PRINT_HEAD(...) __PRINT_HEAD(__VA_ARGS__, "")

#define __PRINT_TAIL(X, ...) __VA_ARGS__
#define PRINT_TAIL(...) __PRINT_TAIL(__VA_ARGS__, "")

#define __PRINT_PRETTY(fmt, ...) printf("%s:%s.%d " fmt "%s", __FILE__, __func__, __LINE__, __VA_ARGS__)
#define PRINT_PRETTY(...) __PRINT_PRETTY(PRINT_HEAD(__VA_ARGS__), PRINT_TAIL(__VA_ARGS__))

#endif