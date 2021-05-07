#ifndef FILE2_H
#define FILE2_H

#include <file1.h>
#ifdef DEF1
    #define DEF1_MSG "Mamy Def1"
#else
    #define DEF1_MSG "Nie mamy Def1"
#endif

#if defined(DEF2) && defined(DEF3)
    #define DEF2_MSG "Mamy Def2 i Def3"
#elif defined(DEF2)
    #define DEF2_MSG "Mamy tylko Def2"
#elif defined(DEF3)
    #define DEF3_MSG "Mamy tylko Def3"
#endif

/* Preproceor to nie C! tutaj ogarnia inty i enumy */
#if defined(DEF1) && (DEF1 == 2 || DEF1 < 5)
    #undef DEF4
    #define DEF4 "3"
#endif

#ifndef DEF5
    #define DEF5 5
#endif


#endif