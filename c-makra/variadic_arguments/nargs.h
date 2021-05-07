#ifndef NARGS_H
#define NARGS_H

#define _NARGS(_1, _2, _3, _4, _5, _N, ...) _N

#define NARGS(...) _NARGS(__VA_ARGS__, 5, 4, 3, 2, 1)

#define NARGS_EXAMPLE(...) printf("MAM %d argsow\n", NARGS(__VA_ARGS__))

#define SUM_VA(...) SUM(NARGS(__VA_ARGS__), __VA_ARGS__)

#endif