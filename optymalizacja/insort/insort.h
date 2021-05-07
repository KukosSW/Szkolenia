#ifndef INSORT_H
#define INSORT_H

#include <stddef.h>

/* Wersja z ksiazki Wprowadzenie do Algorytmow Cormen */
void insort_cormen(int t[], size_t n);

/* Rozbijamy petle przepisujaca klucze, na znajdujaca miejsce, do pszesuwania uzywamy memmove */
void insort_memmove(int t[], size_t n);

/* To samo co wyzej, jednak petle wyszukujaca zamieniamy na binary search, poniewaz miejsca szukamy w podtablicy posortowanej */
void insort_bs_memmove(int t[], size_t n);

/* To samo co wyzej, jednak uzywamy dodatkowe tablicy i wstawiamy elementy od srodka, przesuwamy ja w lewo albo w prawo w zaleznosci co sie bardziej oplaca (czyli bidirectional) */
void insort_bs_memmove_bdir(int t[], size_t n);

#endif