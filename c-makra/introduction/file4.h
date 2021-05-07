#ifndef FILE4_H
#define FILE4_H

#define DEF1

/* To jest zawsze */
#ifndef DEF1
    #error "ERROR: BRAK DEF1"
#endif

/* To jest czasami */
#ifndef DEF1
    #warning "WARNING: BRAK DEF1"
    #pragma message "MESSAGE: BRAK DEF1"
#endif

#endif