#ifndef FILE3_H
#define FILE3_H

#define FOO printf("FOO\n")
#define BAR() printf("BAR\n")

#define MULTILINE() \
    printf("First Line\n"); \
    printf("Second Line\n")

#define MULTILINE2() \
{ \
    printf("First Line\n"); \
    printf("Second Line\n"); \
}

#define MULTILINE3() \
    do { \
        printf("First Line\n"); \
        printf("Second Line\n"); \
    } while (0)


#define x(x) \
    do { \
        x++; \
        void y(int x) {printf("X = %d\n", x);} \
        y(++x); \
    } while (0)


#endif