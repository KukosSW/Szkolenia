#include <msearch.h>

ssize_t search_linear(const int t[], size_t n, int key, cmp_int_f cmp)
{
    for (size_t i = 0; i < n; ++i)
        if (cmp(&t[i], &key) == 0)
            return (ssize_t)i;

    return (ssize_t)-1;
}

/*
    t[] = {1, 5, 6, 10, 15, 20, 100, 500}

    Szukamy 20
    Bierzemy srodek t[mid] = 10.
    Wiemy ze 20 > 10, wiesz na bank bedzie w podtablicy {15, 20, 100, 500}
    Bierzemy srodek t[mid] = 20
    I juz!
*/
ssize_t search_binary(const int t[], size_t n, int key, cmp_int_f cmp)
{
    ssize_t left = 0;
    ssize_t right = (ssize_t)(n - 1);
    while (left <= right)
    {
        const ssize_t middle = (left + right) / 2;
        const int cmp_res = cmp(&t[middle], &key);

        if (cmp_res == 0)
            return middle;
        else if (cmp_res < 0)
            left = middle + 1;
        else
            right = middle - 1;
    }

    return (ssize_t)-1;
}

/*
    Opieramy sie na prostej intuicji.
    Zakladamy ze rozklad liczb jest jednostajny.
    Wiec jesli wiemy ze srednio przeskok jest o X,
    A szukamy wartosci Key, no to musimy zaczac od (Key - t[left]) * 1/X
    Zatem w kazdym ruchu obliczamy srednie tempo wzrostu w podtablicy
    i probujemy przeskoczyc na miejsce wedlug proporcji

    t[] = {1, 5, 6, 7, 10, 14, 20, 22}

    Szukamy 20.
    Liczymy srednio tempo: (22 - 1) / 7 = 3
    My jestesmy oddaleni o 19 od wiec ustawiamy sie na 19 / 3 = 6
    t[6] = 20. Mamy to!

    Wyszlo bardzo dobrze bo liczby rosly w miare rowno, gdyby tak nie bylo, bedziemy gorsi od binarnego
*/
ssize_t search_interpolation(const int t[], size_t n, int key, cmp_int_f cmp)
{
    ssize_t left = 0;
    ssize_t right = (ssize_t)(n - 1);

    /*
        musimy wiedziec ze szukana wartosc jest w zakresie tablicy,
        jesli jest poza to proporcja nam kaze isc poza tablice

        Np. mamy t[0] = 0, t[10] = 9, Key = 100
        L = 0, R = 9
        Proporcja wyjdzie ((9 - 0) / (9 - 0)) * (100 - 0) = 1 * 100 = 100, a przeciez to jest poza tablica
    */
    if (n == 0 || cmp(&key, &t[0]) < 0 || cmp(&key, &t[n - 1]) > 0)
        return (ssize_t)-1;

    while (left <= right)
    {
        ssize_t middle;
        /* a co jesli dziele przez 0? No bedzie lipa, dlatego to sprawdzam i pomijam wzor */
        if (left == right || t[right] == t[left])
            middle = left;
        else
            middle = left + (ssize_t)(((double)(right - left) / (double)(t[right] - t[left])) * (double)(key - t[left]));

        const int cmp_res = cmp(&t[middle], &key);

        if (cmp_res == 0)
            return middle;
        else if (cmp_res < 0)
            left = middle + 1;
        else
            right = middle - 1;
    }

    return (ssize_t)-1;
}
