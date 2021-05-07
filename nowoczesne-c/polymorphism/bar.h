#ifndef BAR_H
#define BAR_H

#include <dynamic_poly.h>

typedef struct Bar
{
    double a;
    int b;
    char c;
} Bar;

static inline const char* bar_get_name(void)
{
    return "Bar";
}

void bar_print(const Bar* bar);
Object* bar_create_object(double a, int b, char c);


#endif