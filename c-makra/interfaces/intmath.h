#ifndef INTMATH_H
#define INTMATH_H

#include <mathinterface.h>

int intmath_add(int a, int b);
int intmath_sub(int a, int b);
int intmath_mul(int a, int b);
int intmath_div(int a, int b);

MATHINTERFACE_CHECK_COMPATIBILITY(intmath, int)

#endif