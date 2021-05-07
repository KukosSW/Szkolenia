#ifndef DOUBLEMATH_H
#define DOUBLEMATH_H

#include <mathinterface.h>

double doublemath_add(double a, double b);
double doublemath_sub(double a, double b);
double doublemath_mul(double a, double b);
double doublemath_div(double a, double b);

MATHINTERFACE_CHECK_COMPATIBILITY(doublemath, double)

#endif