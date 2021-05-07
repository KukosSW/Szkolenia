#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct Matrix Matrix;
typedef enum matrix_t
{
    MATRIX_2D,
    MATRIX_1D,
} matrix_t;

/* Obsluga struktury matrix, glownie sluzy do testowania w tym projekcie */
Matrix* matrix_create(size_t n, size_t m, matrix_t type);

void matrix_destroy(Matrix* matrix);

void matrix_copy(Matrix* restrict dst, const Matrix* restrict src);

void* matrix_get_array(const Matrix* m);

int matrix_get_element(const Matrix *m, size_t i, size_t j);

void matrix_set_element(Matrix *m, size_t i, size_t j, int val);

int matrix_cmp(const Matrix *m1, const Matrix *m2);

/***************************************************************************/

/* Klasyczne mnozenie, naiwna metoda O(n^3), tzw row-column */
Matrix* matrix_mul(const Matrix* m1, const Matrix *m2);

/* Przed odpaleniem klasycznego algorytmu, robimy transpozycje macierzy m2 (alokujac dodatkowa pamiec O(n^2)) */
Matrix* matrix_mul_transpose(const Matrix* m1, const Matrix *m2);

/* Dalej naiwna metoda O(n^3), jednak jest cache-friendly bez transpozycji, czyli nie ma dodatkowej pamieci */
Matrix* matrix_mul_rowbyrow(const Matrix* m1, const Matrix *m2);

#endif