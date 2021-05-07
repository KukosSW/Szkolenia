#ifndef FILE1_H
#define FILE1_H

#define SUM2(a, b) ((a) + (b))
#define SUM3(a, b, c) (SUM2(a, b) + (c))
#define SUM4(a, b, c, d) (SUM3(a, b, c) + (d))

#define SUM(n, ...) sum(n, __VA_ARGS__)

int sum(int n, ...);

#endif