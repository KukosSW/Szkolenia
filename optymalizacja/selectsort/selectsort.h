#ifndef SELECTSORT_H
#define SELECTSORT_H

#include <stddef.h>

/* Wersja z ksiazki Wprowadzenie do Algorytmow Cormen */
void selectsort_cormen_max(int t[], size_t n);

/* Wersja z ksiazki Wprowadzenie do Algorytmow Cormen */
void selectsort_cormen_min(int t[], size_t n);

/* To samo co min, tylko inny zapis liczenia minimum */
void selectsort_cormen_min2(int t[], size_t n);

/* Wersja w ktorej jednoczesnie szukamy min i max za pomoca wagi szalkowej czyli zawsze 3/2n */
void selectsort_minmax(int t[], size_t n);

/* Wersja w ktorej jednoczesnie szukamy min i max algorytmem o przypadku najgorszym 2n ale srednim 3/2n */
void selectsort_minmax2(int t[], size_t n);

/* Wersja ze zbiorem podzielonym na podzbiory tak aby zapamietac max z niezmiennego podzbioru */
void selectsort_subarrays(int t[], size_t n);
#endif