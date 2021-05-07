#include <unique.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static int cmp_int(const void* a, const void* b);

static int cmp_int(const void* a, const void* b)
{
    const int _a = *(const int *)a;
    const int _b = *(const int *)b;

    return _a - _b;
}

size_t unique_count_erase(const int t[], size_t n)
{
#define ERASED_VALUE 0

    int *temp = malloc(sizeof(*temp) * n);
    memcpy(temp, t, sizeof(*temp) * n);

    size_t counter = 0;

    /* sprawdzamy czy ERASED_VALUE jest u nas w tablicy */
    for (size_t i = 0; i < n; ++i)
        if (temp[i] == ERASED_VALUE)
        {
            counter = 1;
            break;
        }

    /*
        Uznajemy wszystkie ERASED_VALUE jako usuniete, wiec uzywamy wykreslania
        Bierzemy liczbe, i wykreslami wszystkie powtorzenia, pozniej zliczymy
        liczbe wartosci nie skreslonych
    */
    for (size_t i = 0; i < n; ++i)
        if (temp[i] != ERASED_VALUE)
            for (size_t j = i + 1; j < n; ++j)
                if (temp[j] == temp[i])
                    temp[j] = ERASED_VALUE;

    /* liczymy ile zostalo normalnych wartosci */
    for (size_t i = 0; i < n; ++i)
        if (temp[i] != ERASED_VALUE)
            ++counter;

    free(temp);
    return counter;

#undef ERASED_VALUE
}

size_t unique_count(const int t[], size_t n)
{
    if (n == 0)
        return 0;

    size_t counter = 1;  /* pierwszy element zawsze jest unikatowy */

    /* pierwszy juz wzielismy, wiec zaczynamy od 1 */
    for (size_t i = 1; i < n; ++i)
    {
        /*
            patrzymy czy dany element jest unikalny w subarray, jesli byl unikalny to counter++
            Dzieki temu jak mamy 10 elementow, to tylko ostatni element bedzie unikalny w subarrau
            (od niego do konca tablicy), wiec to dobry sposob na zliczanie unikatow

        */
        bool is_unique = true;
        for (size_t j = 0; j < i; ++j)
            if (t[i] == t[j])
            {
                is_unique = false;
                break;
            }

        counter += is_unique;
    }

    return counter;
}

size_t unique_count_sort(const int t[], size_t n)
{
    int *temp = malloc(sizeof(*temp) * n);
    memcpy(temp, t, sizeof(*temp) * n);

    /* posortuj, aby elementy byly obok siebie */
    qsort(temp, n, sizeof(*temp), cmp_int);

    size_t counter = 0;
    for (size_t i = 0; i < n; ++i)
    {
        /* musimy przejsc na koniec tych samych elementow */
        while (i < n - 1 && temp[i] == temp[i + 1])
            ++i;

        /* przeszlismy po wszystkich elementach o tej samej wartosci, wiec mamy kolejna unikatowa za soba */
        ++counter;
    }

    free(temp);
    return counter;
}