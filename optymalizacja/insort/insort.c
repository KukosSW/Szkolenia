#include <insort.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

static inline size_t bs_find_pos(int t[], size_t n, int key);

/*
    Lekko zmodyfikowany binary search, dziala jak upper bound

    Musimy tak to zrobic, to nie szukamy klucza, ale miejsca
    gdzie nalezy wstawic klucz.
    Zatem znajdz miejsce goscia ktory jest minimalnie lepszy od klucza
    przesuniemy jego i kumpli w prawo a siebie wstawimy na jego miejsce
*/
static inline size_t bs_find_pos(int t[], size_t n, int key)
{
    size_t left = 0;
    size_t right = n;
    while (left < right)
    {
        const size_t middle = (left + right) / 2;
        if (t[middle] < key)
            left = middle + 1;
        else
            right = middle;
    }

    return left;
}

void insort_cormen(int t[], size_t n)
{
    for (size_t i = 1; i < n; i++)
    {
        const int key = t[i];
        ssize_t pos = (ssize_t)i - 1;

        // przesuwaj elementy az nie napotkasz miejsca na klucz
        while (pos >= 0 && t[pos] > key)
        {
            t[pos + 1] = t[pos];
            pos = pos - 1;
        }

        // wstaw klucz do posortowanej tablicy tak aby byla posortowana
        t[pos + 1] = key;
    }
}

void insort_memmove(int t[], size_t n)
{
    /*
        1 2 3 5 6 7 4
        i = 6 (czyli t[i] = 4)
        pos = 5 i petla while leci az do momentu t[pos] = 3
        Czyli po petli pos = 2
        Chcemy wstawic klucz na pozycje pos = 3
        Zatem ++pos (pos = 3)
        Mowimy ze przesuwamy elementy 5 6 7 o jedno miejsce w prawo, stad dst = &t[pos + 1] a src &t[pos]
        Elementow jest tyle ile odleglosc pomiedzy kluczem (i) a pierwszym kopiowanym elementem (pos)
    */
    for (size_t i = 1; i < n; i++)
    {
        const int key = t[i];
        ssize_t pos = (ssize_t)i - 1;

        // znajdz miejsce na klucz
        while (pos >= 0 && t[pos] > key)
            pos = pos - 1;

        // ustaw sie na pozycje gdzie ma byc klucz
        ++pos;

        // nie wlaczaj memmove bez potrzeby
        if (pos < (ssize_t)i)
        {
            // zrob miejsce na nowy klucz
            memmove(&t[pos + 1], &t[pos], sizeof(*t) * (i - (size_t)pos));

            // wstaw klucz do posortowanej tablicy tak aby byla posortowana
            t[pos] = key;
        }
    }
}

void insort_bs_memmove(int t[], size_t n)
{
    for (size_t i = 1; i < n; i++)
    {
        const int key = t[i];

        // znajdz pozycje za pomoca binary search
        const size_t pos = bs_find_pos(t, i, key);

        // nie wlaczaj memmove bez potrzeby
        if (pos < i)
        {
            // zrob miejsce na nowy klucz
            memmove(&t[pos + 1], &t[pos], sizeof(*t) * (i - (size_t)pos));

            // wstaw klucz do posortowanej tablicy tak aby byla posortowana
            t[pos] = key;
        }
    }
}

void insort_bs_memmove_bdir(int t[], size_t n)
{
    if (n == 0)
        return;

    int *temp = malloc(sizeof(*temp) * n);
    if (temp == NULL)
        return;

    const size_t mid = n / 2;
    size_t left = mid; // gdzie sie konczy posortowana tablica z lewej i z prawej
    size_t right = mid;

    // 1 elementowa tablica jest posortowana, mozna wstawic t[0] ale to zle sie zachowuje pozniej, problem jak z pivotem w QuickSort
    temp[mid] = t[mid];
    t[mid] = t[0];

    for (size_t i = 1; i < n; ++i)
    {
        const int key = t[i];
        size_t pos = bs_find_pos(&temp[left], right - left + 1, key);

        /* mozna zapisac to wszystko lepiej, ale chcialem zachowac strukture przypadkow (cases) */

        /* wstawiamy cos przed lefta, oznacza to ze nie robimy shifta, tylko wstawiamy */
        if (pos == 0 && left > 0)
        {
            --left;
            pos = left;
            temp[pos] = key;
            continue;

        }

        /* wstawiamy cos za righta, czyli nie ma shifta, tylko wstawiamy */
        if (pos > right - left && right < n - 1)
        {
            ++right;
            pos = right;
            temp[pos] = key;
            continue;
        }

        /* poniewaz bs_find_pos widzi &t[left] to wynik jest przesuniety o left (jesli nie jest 0), ale 0 jest obrabiane wczesniej, wiec git */
        pos += left;

        /* mozemy isc z lewej i sie nam oplaca, albo musimy isc z lewej po prawa skonczona */
        if ((left > 0 && pos - left <= right - pos) || right == n - 1)
        {
            /* bs_find_pos to taki upper bound, wiec zwroci najmniejszy wiekszy rowny od siebie, zatem jak idziemy z lewej to nie przesuwamy ostatniego goscia bo on juz jest wiekszy niz tamten i jest git */
            memmove(&temp[left - 1], &temp[left], (pos - left) * sizeof(*temp));
            --left;

            /* tylko ze skoro nie przesunelismy goscia, to musimy wstawic siebie przed niego */
            temp[pos - 1] = key;
        }
        else
        {
            /* przesuwamy gosci lacznie z typem stojacym na pozycji pos w prawo */
            memmove(&temp[pos + 1], &temp[pos], (right - pos + 1) * sizeof(*temp));
            ++right;
            temp[pos] = key;
        }
    }

    /* temp jest posortowany, czas go przepisac do t */
    memcpy(t, temp, sizeof(*t) * n);
    free(temp);
}
