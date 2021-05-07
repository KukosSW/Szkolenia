#include <matrix.h>
#include <stdlib.h>
#include <string.h>

struct Matrix
{
    size_t n;
    size_t m;
    matrix_t type;

    /* gnu99 extension, unnamed union */
    __extension__ union
    {
        int** matrix_2d;
        int*  matrix_1d;
        void* matrix;
    };
};

static void matrix_reset(Matrix* m);

static void matrix_reset(Matrix* m)
{
    switch (m->type)
    {
        case MATRIX_2D:
        {
            for (size_t i = 0; i < m->n; ++i)
                memset(m->matrix_2d[i], 0, m->m * sizeof(*m->matrix_2d[i]));

            break;
        }
        case MATRIX_1D:
        {
            memset(m->matrix_1d, 0, m->m * m->n * sizeof(*m->matrix_1d));

            break;
        }
        default:
            break;
    }
}

/*
    Dla uproszczenia kodu, nie obsluguje error w mallocu
    Normalnie trzeba zwalniac chain malloca
    Czyli jesli wysypal sie na matrix[5]
    to zwalniamy matrix[0] -- matrix[4] i sama matrix
 */
Matrix* matrix_create(size_t n, size_t m, matrix_t type)
{
    Matrix* matrix = malloc(sizeof(*matrix));

    matrix->n = n;
    matrix->m = m;
    matrix->type = type;

    switch (type)
    {
        case MATRIX_2D:
        {
            matrix->matrix_2d = malloc(n * sizeof(*matrix->matrix_2d));
            for (size_t i = 0; i < n; ++i)
                matrix->matrix_2d[i] = malloc(m * sizeof(*matrix->matrix_2d[i]));

            break;
        }
        case MATRIX_1D:
        {
            matrix->matrix_1d = malloc(n * m * sizeof(*matrix->matrix_1d));

            break;
        }
        default:
            return NULL;
    }

    return matrix;
}

void matrix_destroy(Matrix* matrix)
{
    switch (matrix->type)
    {
        case MATRIX_2D:
        {
            for (size_t i = 0; i < matrix->n; ++i)
                free(matrix->matrix_2d[i]);

            free(matrix->matrix_2d);

            break;
        }
        case MATRIX_1D:
        {
            free(matrix->matrix_1d);

            break;
        }
        default:
            break;
    }

    free(matrix);
}

void matrix_copy(Matrix* restrict dst, const Matrix* restrict src)
{
    if (dst->type != src->type || dst->n != src->n || dst->m != src->m)
        return;

    switch (src->type)
    {
        case MATRIX_2D:
        {
            for (size_t i = 0; i < src->n; ++i)
                memcpy(dst->matrix_2d[i], src->matrix_2d[i], src->m * sizeof(*src->matrix_2d[i]));

            break;
        }
        case MATRIX_1D:
        {
            memcpy(dst->matrix_1d, src->matrix_1d, src->m * src->n * sizeof(*src->matrix_1d));

            break;
        }
        default:
            break;
    }
}

void* matrix_get_array(const Matrix* m)
{
    return m->matrix;
}


int matrix_get_element(const Matrix *m, size_t i, size_t j)
{
    switch (m->type)
    {
        case MATRIX_1D:
        {
            const int (*t)[m->m] = (const int (*)[m->m])m->matrix_1d;
            return t[i][j];
        }
        case MATRIX_2D:
        {
            return m->matrix_2d[i][j];
        }
        default:
            return 0;
    }
}

void matrix_set_element(Matrix *m, size_t i, size_t j, int val)
{
    switch (m->type)
    {
        case MATRIX_1D:
        {
            int (*t)[m->m] = (int (*)[m->m])m->matrix_1d;
            t[i][j] = val;
            break;
        }
        case MATRIX_2D:
        {
            m->matrix_2d[i][j] = val;
            break;

        }
        default:
            break;
    }
}

int matrix_cmp(const Matrix *m1, const Matrix *m2)
{
    if (m1->type != m2->type || m1->n != m2->n || m1->m != m2->m)
        return -1;

    switch (m1->type)
    {
        case MATRIX_1D:
        {
            return memcmp(m1->matrix_1d, m2->matrix_1d, m1->n * m1->m * sizeof(*m1->matrix_1d));
        }
        case MATRIX_2D:
        {
            int result = 0;
            for (size_t i = 0; i < m1->n; ++i)
                result += memcmp(m1->matrix_2d[i], m2->matrix_2d[i], m1->m * sizeof(*m1->matrix_2d[i]));

            return result;
        }
        default:
            return -1;
    }
}

/*
    Ta metoda jest bardzo slaba pod katem odczytu pamieci
    Jesli mamy macierz, to elementy drugiego wymiaru leza obok siebie
    Zatem jesli bedziemy zmieniac w petli pierwszy wymiar, mamy zawsze doczytywanie  RAM do cache,
    jesli po drugim wymiarze, to czesto mamy juz elementu w cache

    A[i][k] jest ok, bo zmienia sie co chwila k,
    B[k][j] jest nok

    res += A[i][k] * B[k][j];
*/
Matrix* matrix_mul(const Matrix* m1, const Matrix *m2)
{
    if (m1->m != m2->n)
        return NULL;

    if (m1->type != m2->type)
        return NULL;

    Matrix* m3 = matrix_create(m1->n, m2->m, m1->type);

    if (m1->type == MATRIX_1D)
    {
        int (*A)[m1->m] = (int (*)[m1->m])m1->matrix_1d;
        int (*B)[m2->m] = (int (*)[m2->m])m2->matrix_1d;
        int (*C)[m3->m] = (int (*)[m3->m])m3->matrix_1d;

        for (size_t i = 0; i < m1->n; ++i)
            for (size_t j = 0; j < m2->m; ++j)
            {
                int res = 0;
                for (size_t k = 0; k < m1->m; ++k)
                    res += A[i][k] * B[k][j];

                C[i][j] = res;
            }
    }
    else
    {
        int** A = m1->matrix_2d;
        int** B = m2->matrix_2d;
        int** C = m3->matrix_2d;

        for (size_t i = 0; i < m1->n; ++i)
            for (size_t j = 0; j < m2->m; ++j)
            {
                int res = 0;
                for (size_t k = 0; k < m1->m; ++k)
                    res += A[i][k] * B[k][j];

                C[i][j] = res;
            }
    }

    return m3;
}

/*
    W poprzedniej metodzie byl problem z B[k][j]

    Chcemy miec B[j][k]
    Jak to zrobic?

    Jesli B byla n x m
    To tworzymy B' m x n
    przed odpaleniem algo zrobimy B'[i][j] = B[j][i]

    Mamy zamienione wymiary, wiec w petli mozemy robic B[j][k] zamiast B[k][j]

    To podejscie czysto programistyczne.

    Matematycznie wystarczy powiedziec ze B^T^T = B
    Czyli Transpozycja transponowanej macierzy jest ta sama macierza

    W tej metodzie obie macierze sa cache friendly
*/
Matrix* matrix_mul_transpose(const Matrix* m1, const Matrix *m2)
{
    if (m1->m != m2->n)
        return NULL;

    if (m1->type != m2->type)
        return NULL;

    Matrix* m3 = matrix_create(m1->n, m2->m, m1->type);
    Matrix* temp = matrix_create(m2->m, m2->n, m2->type);

    /* zamien [i][j] na [j][i] */
    for (size_t i = 0; i < m2->n; ++i)
        for (size_t j = 0; j < m2->m; ++j)
            matrix_set_element(temp, j, i, matrix_get_element(m2, i, j));

    if (m1->type == MATRIX_1D)
    {
        int (*A)[m1->m] = (int (*)[m1->m])m1->matrix_1d;
        int (*B)[temp->m] = (int (*)[temp->m])temp->matrix_1d;
        int (*C)[m3->m] = (int (*)[m3->m])m3->matrix_1d;

        for (size_t i = 0; i < m1->n; ++i)
            for (size_t j = 0; j < m2->m; ++j)
            {
                int res = 0;
                for (size_t k = 0; k < m1->m; ++k)
                    res += A[i][k] * B[j][k]; // tutaj [j][k] zamiast [k][j]

                C[i][j] = res;
            }
    }
    else
    {
        int** A = m1->matrix_2d;
        int** B = temp->matrix_2d;
        int** C = m3->matrix_2d;

         for (size_t i = 0; i < m1->n; ++i)
            for (size_t j = 0; j < m2->m; ++j)
            {
                int res = 0;
                for (size_t k = 0; k < m1->m; ++k)
                    res += A[i][k] * B[j][k]; // tutaj [j][k] zamiast [k][j]

                C[i][j] = res;
            }
    }

    matrix_destroy(temp);
    return m3;
}

/*
    Nadal naiwna metoda, ale inaczej zapisujemy wzor,
    dzieki temu mamy odwolania cache friendly (po drugim wymiarze)
    bez koniecznosci transpozycji, zatem mamy O(1) pamiec a nie O(n^2)

    Minusem jest to, ze mamy duzo odwolan do tablicy C,
    ale one tez sa ok (2nd wymiar) wiec sie nie przejmujemy zbytnio

    Porzadne wytlumaczenie mozecie znalezc na prezentacji
*/
Matrix* matrix_mul_rowbyrow(const Matrix* m1, const Matrix *m2)
{
    if (m1->m != m2->n)
        return NULL;

    if (m1->type != m2->type)
        return NULL;

    Matrix* m3 = matrix_create(m1->n, m2->m, m1->type);
    matrix_reset(m3);

    if (m1->type == MATRIX_1D)
    {
        int (*A)[m1->m] = (int (*)[m1->m])m1->matrix_1d;
        int (*B)[m2->m] = (int (*)[m2->m])m2->matrix_1d;
        int (*C)[m3->m] = (int (*)[m3->m])m3->matrix_1d;

        for (size_t i = 0; i < m1->n; ++i)
            for (size_t j = 0; j < m1->m; ++j)
            {
                const int a = A[i][j];
                for (size_t k = 0; k < m2->m; ++k)
                    C[i][k] += a * B[j][k];
            }
    }
    else
    {
        int** A = m1->matrix_2d;
        int** B = m2->matrix_2d;
        int** C = m3->matrix_2d;

        for (size_t i = 0; i < m1->n; ++i)
            for (size_t j = 0; j < m1->m; ++j)
            {
                const int a = A[i][j];
                for (size_t k = 0; k < m2->m; ++k)
                    C[i][k] += a * B[j][k];
            }
    }

    return m3;
}