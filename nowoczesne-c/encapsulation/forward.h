#ifndef FORWARD_H
#define FORWARD_H

#include <stddef.h>

/* Daje znac ze jest taki typ jak strukt FooForward, mozemy uzywac tylko pointera */
typedef struct FooForward FooForward;

/* Poniewaz nie mam struktury to nawet malloca nie moge zrobic, bo jak podam size? */
FooForward* fooforward_create(int a, int b, int c, int d);
void fooforward_destroy(FooForward* foo);

/* nie moge napisac tutaj static inline, bo stad nie moge odwolac sie do pol struktury */
int fooforward_get_a(const FooForward* foo);
int fooforward_get_b(const FooForward* foo);
int fooforward_get_c(const FooForward* foo);
int fooforward_get_d(const FooForward* foo);

void fooforward_set_a(FooForward* foo, int a);
void fooforward_set_b(FooForward* foo, int b);
void fooforward_set_c(FooForward* foo, int c);
void fooforward_set_d(FooForward* foo, int d);


void fooforward_print(const FooForward* foo);

size_t fooforward_sizeof(void);

#endif