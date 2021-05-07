

/********************** TESTY ****************************/

#include <test_framework.h>
#include <stdlib.h>
#include <matrix.h>

static test_f test_copy(matrix_t type);
static test_f test_mul_1x1(matrix_t type, Matrix* (*matrix_mult)(const Matrix*, const Matrix*));
static test_f test_mul_2x2(matrix_t type, Matrix* (*matrix_mult)(const Matrix*, const Matrix*));
static test_f test_mul_5x5(matrix_t type, Matrix* (*matrix_mult)(const Matrix*, const Matrix*));

static void test_suite(void);


static test_f test_copy(matrix_t type)
{
#define N 3
#define M 4
    int t[N][M] =  {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    Matrix* m1 = matrix_create(N, M, type);
    Matrix* m2 = matrix_create(N, M, type);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            matrix_set_element(m1, i, j, t[i][j]);

    matrix_copy(m2, m1);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            T_EXPECT(matrix_get_element(m1, i, j), t[i][j]);

    T_EXPECT(matrix_cmp(m1, m2), 0);

    matrix_destroy(m1);
    matrix_destroy(m2);

#undef N
#undef M
}

static test_f test_mul_1x1(matrix_t type, Matrix* (*matrix_mult)(const Matrix*, const Matrix*))
{
    int A[] = {1};
    int B[] = {2};
    int C[] = {2};

    Matrix* m1 = matrix_create(1, 1, type);
    Matrix* m2 = matrix_create(1, 1, type);

    matrix_set_element(m1, 0, 0, A[0]);
    matrix_set_element(m2, 0, 0, B[0]);

    Matrix* m3 = matrix_mult(m1, m2);
    T_ERROR(m3 == NULL);

    T_EXPECT(matrix_get_element(m3, 0, 0), C[0]);

    matrix_destroy(m1);
    matrix_destroy(m2);
    matrix_destroy(m3);
}

static test_f test_mul_2x2(matrix_t type, Matrix* (*matrix_mult)(const Matrix*, const Matrix*))
{
#define N 2
#define M 3
    int A[N][M] = {{1, 2, 3}, {4, 5, 6}};
    int B[M][N] = {{1, 2}, {3, 4}, {5, 6}};
    int C[N][N] = {{22, 28}, {49, 64}};

    Matrix* m1 = matrix_create(N, M, type);
    Matrix* m2 = matrix_create(M, N, type);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
        {
            matrix_set_element(m1, i, j, A[i][j]);
            matrix_set_element(m2, j, i, B[j][i]);
        }

    Matrix* m3 = matrix_mult(m1, m2);
    T_ERROR(m3 == NULL);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            T_EXPECT(matrix_get_element(m3, i, j), C[i][j]);

    matrix_destroy(m1);
    matrix_destroy(m2);
    matrix_destroy(m3);

#undef N
#undef M
}

static test_f test_mul_5x5(matrix_t type, Matrix* (*matrix_mult)(const Matrix*, const Matrix*))
{
#define N 5
#define M 8
    int A[N][M] = { { 1,  2,  3,  4,  5,  6,  7,  8},
                    { 9, 10, 11, 12, 13, 14, 15, 16},
                    {17, 18, 19, 20, 21, 22, 23, 24},
                    {25, 26, 27, 28, 29, 30, 31, 32},
                    {33, 34, 35, 36, 37, 38, 39, 40}
                  };
    int B[M][N] = { { 1,  2,  3,  4,  5},
                    { 6,  7,  8,  9, 10},
                    {11, 12, 13, 14, 15},
                    {16, 17, 18, 19, 20},
                    {21, 22, 23, 24, 25},
                    {26, 27, 28, 29, 30},
                    {31, 32, 33, 34, 35},
                    {36, 37, 38, 39, 40}
                 };
    int C[N][N] = { { 876,  912,  948,  984, 1020},
                    {2060, 2160, 2260, 2360, 2460},
                    {3244, 3408, 3572, 3736, 3900},
                    {4428, 4656, 4884, 5112, 5340},
                    {5612, 5904, 6196, 6488, 6780}
                  };

    Matrix* m1 = matrix_create(N, M, type);
    Matrix* m2 = matrix_create(M, N, type);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
        {
            matrix_set_element(m1, i, j, A[i][j]);
            matrix_set_element(m2, j, i, B[j][i]);
        }

    Matrix* m3 = matrix_mult(m1, m2);
    T_ERROR(m3 == NULL);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            T_EXPECT(matrix_get_element(m3, i, j), C[i][j]);

    matrix_destroy(m1);
    matrix_destroy(m2);
    matrix_destroy(m3);

#undef N
#undef M
}

static void test_suite(void)
{
    TEST(test_copy(MATRIX_1D));
    TEST(test_copy(MATRIX_2D));

    TEST(test_mul_1x1(MATRIX_1D, matrix_mul));
    TEST(test_mul_1x1(MATRIX_2D, matrix_mul));

    TEST(test_mul_2x2(MATRIX_1D, matrix_mul));
    TEST(test_mul_2x2(MATRIX_2D, matrix_mul));

    TEST(test_mul_5x5(MATRIX_1D, matrix_mul));
    TEST(test_mul_5x5(MATRIX_2D, matrix_mul));

    TEST(test_mul_1x1(MATRIX_1D, matrix_mul_transpose));
    TEST(test_mul_1x1(MATRIX_2D, matrix_mul_transpose));

    TEST(test_mul_2x2(MATRIX_1D, matrix_mul_transpose));
    TEST(test_mul_2x2(MATRIX_2D, matrix_mul_transpose));

    TEST(test_mul_5x5(MATRIX_1D, matrix_mul_transpose));
    TEST(test_mul_5x5(MATRIX_2D, matrix_mul_transpose));

    TEST(test_mul_1x1(MATRIX_1D, matrix_mul_rowbyrow));
    TEST(test_mul_1x1(MATRIX_2D, matrix_mul_rowbyrow));

    TEST(test_mul_2x2(MATRIX_1D, matrix_mul_rowbyrow));
    TEST(test_mul_2x2(MATRIX_2D, matrix_mul_rowbyrow));

    TEST(test_mul_5x5(MATRIX_1D, matrix_mul_rowbyrow));
    TEST(test_mul_5x5(MATRIX_2D, matrix_mul_rowbyrow));
}

/********************************************************/

/********************* POMIARY **************************/
#include <stdio.h>
#include <time.h>

#define MEASURE_FUNCTION(func, label) \
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
        printf(label ": czas = %lf[s]\n", timeTaken); \
    } while (0)

static void meas(void);

static void meas_copy(matrix_t type, size_t n);
static void meas_mul(matrix_t type, size_t n, Matrix* (*matrix_mult)(const Matrix*, const Matrix*));

static void meas_copy(matrix_t type, size_t n)
{
    Matrix* m1;
    Matrix* m2;

    m1 = matrix_create(n, n, type);
    m2 = matrix_create(n, n, type);

    matrix_copy(m2, m1);

    matrix_destroy(m1);
    matrix_destroy(m2);
}

static void meas_mul(matrix_t type, size_t n, Matrix* (*matrix_mult)(const Matrix*, const Matrix*))
{
    Matrix* m1;
    Matrix* m2;

    m1 = matrix_create(n, n, type);
    m2 = matrix_create(n, n, type);

    Matrix* m3 = matrix_mult(m1, m2);

    matrix_destroy(m1);
    matrix_destroy(m2);
    matrix_destroy(m3);
}


void meas(void)
{
    size_t n = 10000;
    printf("Kopiowanie %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_copy(MATRIX_2D, n), "2D");
    MEASURE_FUNCTION(meas_copy(MATRIX_1D, n), "1D");

    n = 1000;
    printf("Kopiowanie %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_copy(MATRIX_2D, n), "2D");
    MEASURE_FUNCTION(meas_copy(MATRIX_1D, n), "1D");

    printf("Mnozenie klasyczne %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_mul(MATRIX_2D, n, matrix_mul), "2D");
    MEASURE_FUNCTION(meas_mul(MATRIX_1D, n, matrix_mul), "1D");

    printf("Mnozenie klasyczne z transpozycja %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_mul(MATRIX_2D, n, matrix_mul_transpose), "2D");
    MEASURE_FUNCTION(meas_mul(MATRIX_1D, n, matrix_mul_transpose), "1D");

    n = 50;
    printf("Mnozenie klasyczne z transpozycja %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_mul(MATRIX_2D, n, matrix_mul_transpose), "2D");
    MEASURE_FUNCTION(meas_mul(MATRIX_1D, n, matrix_mul_transpose), "1D");

    n = 1000;
    printf("Mnozenie wierszowo %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_mul(MATRIX_2D, n, matrix_mul_rowbyrow), "2D");
    MEASURE_FUNCTION(meas_mul(MATRIX_1D, n, matrix_mul_rowbyrow), "1D");

    n = 50;
    printf("Mnozenie wierszowo %zu x %zu\n", n, n);
    MEASURE_FUNCTION(meas_mul(MATRIX_2D, n, matrix_mul_rowbyrow), "2D");
    MEASURE_FUNCTION(meas_mul(MATRIX_1D, n, matrix_mul_rowbyrow), "1D");
}

/********************************************************/

int main(int argc, const char* argv[])
{
    /*  test mode */
    if (argc == 2 && strcmp(argv[1], "test") == 0)
    {
        TEST_INIT("Macierze");
        test_suite();
        TEST_SUMMARY();
    }

    /*
        Polecam spojrzec na te wyniki: https://github.com/edufgf/CodeOptimizationTechniques
        Widac ze Strassen nie ma sensu, jesli macierze ze sa super duze, kodu jest okolo 400 linii a dla macierzy do 2000x2000 dziala gorzej niz nasze optymalizacje
    */
    if (argc == 2 && strcmp(argv[1], "pomiary") == 0)
    {
        meas();
        return 0;
    }

    /* tutaj mozecie sie pobawic, jakies wlasne testy, printf, scanf itp */

    return 0;
}