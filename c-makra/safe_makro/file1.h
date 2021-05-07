// #ifndef FILE1_H
// #define FILE1_H
/* Jesli nie ma include guardow, to mozliwe ze bedzie wklejone kilka razy, zatem redeclaration */
void f(void);

/* defined but not used */
static void g(void)
{

}

/* OK, inline nie musi byc uzyte */
static inline void h(void)
{

}

// #endif