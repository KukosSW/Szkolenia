#ifndef NARGS_H
#define NARGS_H

#define _NARGS(_1, _2, _3, _4, _5, _N, ...) _N
#define NARGS(...) _NARGS(__VA_ARGS__, 5, 4, 3, 2, 1)

#define _NARGS2(_1, _2, _3, _4, _N, ...) _N
#define NARGS2(...) _NARGS2(__VA_ARGS__, 2, 2, 1, 1)

#endif