#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <stdio.h>
#include <time.h>

#define MEASURE_FUNTION_LABEL_PRIV(func, label) \
    do { \
        struct timespec start; \
        struct timespec end; \
        double timeTaken; \
        \
        clock_gettime(CLOCK_MONOTONIC, &start); \
        (void)func; \
        clock_gettime(CLOCK_MONOTONIC, &end); \
        \
        timeTaken = (double)(end.tv_sec - start.tv_sec) * 1e9; \
        timeTaken = (double)(timeTaken + (double)(end.tv_nsec - start.tv_nsec)) * 1e-9; \
        \
        printf("func:%s" label ": time = %lf[s]\n", #func, timeTaken); \
    } while (0)

#define MEASURE_FUNCTION_LABEL(func, label) MEASURE_FUNTION_LABEL_PRIV(func, " {" label "}")
#define MEASURE_FUNCTION(func) MEASURE_FUNTION_LABEL_PRIV(func, "")

#endif