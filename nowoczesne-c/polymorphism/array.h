#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

int* array_create_random(size_t n);
int* array_create_step1(size_t n);
int* array_create_step5(size_t n);

void array_print(const int* t, size_t n);
void array_destroy(int* t);

int* array_create(size_t n, int (*get_number)(size_t i));

#endif