#ifndef DOMACRO_H
#define DOMACRO_H

#include <nargs.h>

#define CONCAT(x, y) _CONCAT(x, y)
#define _CONCAT(x, y) x ## y

/* job(x) */
#define __DO_MACRO_1_1(job, x) job(x)
#define __DO_MACRO_1_2(job, x, ...) job(x); __DO_MACRO_1_1(job, __VA_ARGS__)
#define __DO_MACRO_1_3(job, x, ...) job(x); __DO_MACRO_1_2(job, __VA_ARGS__)
#define __DO_MACRO_1_4(job, x, ...) job(x); __DO_MACRO_1_3(job, __VA_ARGS__)
#define __DO_MACRO_1_5(job, x, ...) job(x); __DO_MACRO_1_4(job, __VA_ARGS__)
#define DOMACRO_1_N(n, job, ...) CONCAT(__DO_MACRO_1_, n)(job, __VA_ARGS__)
#define DOMACRO_1(job, ...) do { DOMACRO_1_N(NARGS(__VA_ARGS__), job, __VA_ARGS__); } while (0)

/* job(x, y) */
#define __DO_MACRO_2_1(job, x, y) job(x, y)
#define __DO_MACRO_2_2(job, x, y, ...) job(x, y); __DO_MACRO_2_1(job, __VA_ARGS__)
#define __DO_MACRO_2_3(job, x, y, ...) job(x, y); __DO_MACRO_2_2(job, __VA_ARGS__)
#define __DO_MACRO_2_4(job, x, y, ...) job(x, y); __DO_MACRO_2_3(job, __VA_ARGS__)
#define __DO_MACRO_2_5(job, x, y, ...) job(x, y); __DO_MACRO_2_4(job, __VA_ARGS__)
#define DOMACRO_2_N(n, job, ...) CONCAT(__DO_MACRO_2_, n)(job, __VA_ARGS__)
#define DOMACRO_2(job, ...) do { DOMACRO_2_N(NARGS2(__VA_ARGS__), job, __VA_ARGS__); } while (0)

/* Domyslnie robimy job(x) */
#define DOMACRO(job, ...) DOMACRO_1(job, __VA_ARGS__)

#endif