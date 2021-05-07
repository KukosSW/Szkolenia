#ifndef FILE5_H
#define FILE5_H

/* Preprocessor nie ma rekursji! */
#define FOO1(x) (1 + FOO1(x - 1))

#define FOO(x) (1 + BAR(x))
#define BAR(x) (FOO(x) + 2)

/* NIE MA TEZ Pamieci */

#define A 10
#define TMP_A A
#undef A
#define A 5

#endif