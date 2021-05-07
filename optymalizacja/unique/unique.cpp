#include <unique.h>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>

/*
    hashujemy sobie elementy tablicy t i wstawiamy do zbioru
    set automatycznie usunie powtorzenia


    czyli to dziala w stylu

    set[hash(t[i])] = t[i];

    pozniej size zwraca liczbe elementow w set
*/
size_t unique_hashset(const int t[], size_t n)
{
    std::unordered_set<int> set(t, t + n);

    return set.size();
}

/*
    Wstawiamy elementy do zbioru na podstawie porownan z kluczem
    Czyli mamy zbior jako drzewo binarne
*/
size_t unique_set(const int t[], size_t n)
{
    std::set<int> set(t, t + n);

    return set.size();
}


/*
    Uzywamy wprost metody z c++ algorithm

    To dziala bardzo szybko,
    o wiele szybciej niz zliczanie po posorotowaniu w C
    chociaz uzywaja tego samego algo
    to w C++ sort jest szybszy niz qsort
*/
size_t unique_cpp(const int t[], size_t n)
{
    std::vector<int> vec(t, t + n);
    sort(vec.begin(), vec.end());
    return std::unique(vec.begin(), vec.end()) - vec.begin();
}