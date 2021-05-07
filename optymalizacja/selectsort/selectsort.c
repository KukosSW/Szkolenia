#include <selectsort.h>
#include <sys/types.h>
#include <math.h>
#include <stdlib.h>

/*
    Design tego pliku moglby byc o wiele lepszy
    Pointery do funkcji szukajacych, comparatory i inne tricki zaoszczedzilyby kodu
    Jednak kod jest przygotowany pod odcinek gdzie mamy kodzic nowe metody
    Zatem nie wiemy jak zrobic design zanim nie nakodzimy ich wszystkich
    Dodatkowo tutaj latwiej zobaczyc roznice pomiedzy algo i performance z tym zwiazany
*/

#define _SWAP(a, b, type) \
    do { \
        type ____temp = (a); \
        a = (b); \
        b = ____temp; \
    } while (0)

#define SWAP(a, b) _SWAP(a, b, int)

static inline size_t array_find_max(const int t[], size_t n);
static inline size_t array_find_max2(const int t[], size_t start, size_t end);
static inline size_t array_find_min(const int t[], size_t n);
static inline size_t array_find_min2(const int t[], size_t start, size_t end);
static inline void array_find_min_max(const int t[], size_t start, size_t end, size_t* min_pos, size_t* max_pos);
static inline void array_find_min_max2(const int t[], size_t start, size_t end, size_t* min_pos, size_t* max_pos);

static inline size_t array_find_max(const int t[], size_t n)
{
    size_t max_pos = 0;
    for (size_t i = 1; i < n; ++i)
        if (t[i] > t[max_pos])
            max_pos = i;

    return max_pos;
}

static inline size_t array_find_max2(const int t[], size_t start, size_t end)
{
    size_t max_pos = start;
    for (size_t i = start + 1; i <= end; ++i)
        if (t[i] > t[max_pos])
            max_pos = i;

    return max_pos;
}

static inline size_t array_find_min(const int t[], size_t n)
{
    size_t min_pos = 0;
    for (size_t i = 1; i < n; ++i)
        if (t[i] < t[min_pos])
            min_pos = i;

    return min_pos;
}

static inline size_t array_find_min2(const int t[], size_t start, size_t end)
{
    size_t min_pos = start;
    for (size_t i = start + 1; i <= end; ++i)
        if (t[i] < t[min_pos])
            min_pos = i;

    return min_pos;
}

/* Algorytm wagi szalkowej, zawsze 3/2n jednak kod jest skomplikowany, mozliwe ze zaburzylismy stale (czynniki zalezne od HW, kompi) */
static inline void array_find_min_max(const int t[], size_t start, size_t end, size_t* min_pos, size_t* max_pos)
{
    size_t _min_pos;
    size_t _max_pos;
    size_t i;

    /* Jest nieparzysta liczba, wiec tutaj ustaw min = max wtedy petla przejdzie po wszystkich, bo idzie parzysta liczbe razy */
    if ((end - start + 1) % 2 == 1)
    {
        _min_pos = start;
        _max_pos = start;
        i = start + 1;
    }
    else
    {
        if (t[start] > t[start + 1])
        {
            _min_pos = start + 1;
            _max_pos = start;
        }
        else
        {
            _min_pos = start;
            _max_pos = start + 1;
        }
        i = start + 2;
    }

    for (; i < end; i += 2)
    {
        /*
            Bierzemy kolejnych 2 gosci i sprawdzamy ktory jest ciezszy na wadze
            Ten ciezszy nie moze juz byc najlzejszy, bo jest ciezszy przynajmniej od 1 goscia
            Dlatego ciezszy z nich rywalizuje z max, lzejszy z min
         */
        if (t[i] > t[i + 1])
        {
            if (t[i] > t[_max_pos])
                _max_pos = i;

            if (t[i + 1] < t[_min_pos])
                _min_pos = i + 1;
        }
        else
        {
            if (t[i + 1] > t[_max_pos])
                _max_pos = i + 1;

            if (t[i] < t[_min_pos])
                _min_pos = i;
        }
    }

    *min_pos = _min_pos;
    *max_pos = _max_pos;
}

/* Najgorzej 2n, najlepiej 1n,  srednio 3/2n a mniejszy narzut na stale HW i kompi jak w szalkowej */
static inline void array_find_min_max2(const int t[], size_t start, size_t end, size_t* min_pos, size_t* max_pos)
{
    size_t _min_pos = start;
    size_t _max_pos = start;

    for (size_t i = start + 1; i <= end; ++i)
        if (t[i] > t[_max_pos])
            _max_pos = i;
        else if (t[i] < t[_min_pos])
            _min_pos = i;

    *min_pos = _min_pos;
    *max_pos = _max_pos;
}

void selectsort_cormen_max(int t[], size_t n)
{
    if (n == 0)
        return;

    for (size_t i = 0; i < n - 1; ++i)
    {
        const size_t max_pos = array_find_max(t, n - i);
        SWAP(t[max_pos], t[n - i - 1]);
    }
}

void selectsort_cormen_min(int t[], size_t n)
{
    if (n == 0)
        return;

    for (size_t i = 0; i < n - 1; ++i)
    {
        /*
            array_find_min widzi t[i] jako t[0], wiec przesuwamy o i
            Taki sposob jest bardzo czytelny, jednak mocno spada performance
            mamy operacje &t[i] oraz + i
         */
        const size_t min_pos = array_find_min(&t[i], n - i) + i;
        SWAP(t[min_pos], t[i]);
    }
}

void selectsort_cormen_min2(int t[], size_t n)
{
    if (n == 0)
        return;

    for (size_t i = 0; i < n - 1; ++i)
    {
        /* Uzywamy innego sposobu aby nie dokladac dodatkowych operacji */
        const size_t min_pos = array_find_min2(t, i, n - 1);
        SWAP(t[min_pos], t[i]);
    }
}

void selectsort_minmax(int t[], size_t n)
{
    if (n == 0)
        return;

    ssize_t j = (ssize_t)(n - 1);
    for (size_t i = 0; (ssize_t)i < j; ++i, --j)
    {
        size_t min_pos;
        size_t max_pos;

        array_find_min_max(t, i, (size_t)j, &min_pos, &max_pos);

        SWAP(t[i], t[min_pos]);

        /* jesli min == max, to wtedy 1 zamiana stracza, wiec nie zamieniaj w tym przypadku */
        if (min_pos != max_pos)
        {
            /*
                mozliwe ze max == t[i]
                jest tak to poszla zamiana t[i], t[min]
                czyli max znajduje sie na t[min]
            */
            if (i == max_pos)
                SWAP(t[j], t[min_pos]);
            else
                SWAP(t[j], t[max_pos]);
        }
    }
}

void selectsort_minmax2(int t[], size_t n)
{
    if (n == 0)
        return;

    ssize_t j = (ssize_t)(n - 1);
    for (size_t i = 0; (ssize_t)i < j; ++i, --j)
    {
        size_t min_pos;
        size_t max_pos;

        array_find_min_max2(t, i, (size_t)j, &min_pos, &max_pos);

        SWAP(t[i], t[min_pos]);

        /* jesli min == max, to wtedy 1 zamiana stracza, wiec nie zamieniaj w tym przypadku */
        if (min_pos != max_pos)
        {
            /*
                mozliwe ze max == t[i]
                jest tak to poszla zamiana t[i], t[min]
                czyli max znajduje sie na t[min]
            */
            if (i == max_pos)
                SWAP(t[j], t[min_pos]);
            else
                SWAP(t[j], t[max_pos]);
        }
    }
}

/* niebezpieczne, ale daje rade */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* sufit z dzielenia intow, normalnie w C zawsze jest podloga, dlatego potrzebuje makra, bo ceil nie zadziala */
#define INT_DIV_CEIL(n, k) (((n) + (k) - 1) / (k))

/* ile chunkow mamy */
#define CHUNKS_N(n) (size_t)floor(sqrt((double)n))

/* ile jest liczb w jednym chunku (tym nie ostatnim) */
#define CHUNK_ENTRIES_IN(chunks, n) INT_DIV_CEIL(n, chunks)

/* jesli mamy tablice n elementowa i mamy entries_in_chunk liczb w chunku, to gdzie zaczyna sie ity chunk */
#define CHUNK_START(i, n, entries_in_chunk) ((i) * (entries_in_chunk))

/* jesli mamy tablice n elementowa i mamy entries_in_chunk liczb w chunku, to gdzie konczy sie ity chunk */
#define CHUNK_END(i, n, entries_in_chunk) MIN((n) - 1, (((i) + 1) * (entries_in_chunk) - 1))

/* mamy n_chunks chunkow, tablica zmniejszyla sie do k elementowej, ile teraz mamy chunkow jesli per chunks jest entries liczb */
#define CHUNKS_N_AFTER_UPDATE(k, entries_in_chunk) INT_DIV_CEIL(k, entries_in_chunk)

/*
    Jako ze to dosc trickowe pokaze na przykladzie:
    Niech n = 13
    t[] = {5, 2, 1, 4, 6, 9, 7, 8, 3, 12, 11, 12, 15};
    liczymy ile jest chunkow, floor(sqrt(13)) = 3
    Zatem w chunku mamy ceil(13/3) = 5
    Czyli chunki to: {5, 2, 1, 4, 6}, {9, 7, 8, 3, 12}, {11, 12, 15} (to podzial wirtualny)
    Liczymy max z chunka i zapisujemy do array (dodatkowo zamieniamy na 0 miejsce chunka)
    max={6, 12, 15}, chunki: {6, 2, 1, 4, 5}, {12, 7, 8, 3, 9}, {15, 12, 11}
    Liczymy maxa z max={6, 12, 15} = 15.
    Wiemy ze to chunk 3 wiec to pierwsza liczba z chunka, zamieniamy go z koncem tablicy i mowimy ze tablica zmniejszyla sie o 1
    mamy: {6, 2, 1, 4, 5}, {12, 7, 8, 3, 9}, {12, 11}, czesc tablicy t juz posortowana {15}
    Teraz pora na glowna zalete patenciaka.
    Max zmienil sie tylko w chunku 3, wiec tylko tam szukamy nowego maxa
    Robimy analogicznie, update tablicy max oraz zamiane, dostajemy
    max={6, 12, 12}, chunki: {6, 2, 1, 4, 5}, {12, 7, 8, 3, 9}, {12, 11}
    Robimy tak n - 1 razy.
*/
void selectsort_subarrays(int t[], size_t n)
{
    if (n == 0)
        return;

    size_t n_chunks = CHUNKS_N(n);
    const size_t entries_in_chunk = CHUNK_ENTRIES_IN(n_chunks, n);

    /* bedziemy przechowywac maxa z kazdego chunka, mozna to zrobic bez tablicy, ale z tablica mamy wyszukiwania max cache friendly */
    int *max_from_chunks = malloc(sizeof(*max_from_chunks) * n_chunks);
    if (max_from_chunks == NULL)
        return;

    /* Znajdz max w kazdym chunku i przepisz go tutaj */
    for (size_t i = 0; i < n_chunks; ++i)
    {
        const size_t pos = array_find_max2(t, CHUNK_START(i, n, entries_in_chunk), CHUNK_END(i, n, entries_in_chunk));
        max_from_chunks[i] = t[pos];

        /* Lepiej wiedziec gdzie jest max chunka, poniewaz ostatni chunk bedzie powoli zamienial sie w tablice, to najlepiej poczatek chunka miec jako max */
        SWAP(t[CHUNK_START(i, n, entries_in_chunk)], t[pos]);
    }

    /* glowne sortowanie, sortujemy n elementow wiec petla n - 1 (po 1 element jest posortowany) */
    for (size_t i = 0; i < n - 1; ++i)
    {
        /* Oblicz na nowo liczbe chunkow */
        n_chunks = CHUNKS_N_AFTER_UPDATE(n - i, entries_in_chunk);

        /* Znajdz maxa w chunkach */
        const size_t chunk_with_max = array_find_max(max_from_chunks, n_chunks);

        /* wiemy ze max jest na 0 pozycji chunka, zamieniamy go z ostatnim w tablicy */
        SWAP(t[CHUNK_START(chunk_with_max, n - i, entries_in_chunk)], t[n - i - 1]);

        /* Znajdz nowego maxa w tym chunku */
        const size_t pos = array_find_max2(t, CHUNK_START(chunk_with_max, n - i - 1, entries_in_chunk), CHUNK_END(chunk_with_max, n - i - 1, entries_in_chunk));
        max_from_chunks[chunk_with_max] = t[pos];

        SWAP(t[CHUNK_START(chunk_with_max, n - i - 1, entries_in_chunk)], t[pos]);
    }

    free(max_from_chunks);
}