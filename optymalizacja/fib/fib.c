#include <fib.h>
#include <stddef.h>
#include <string.h>

static uint64_t _fib_rec_mem(uint64_t n, uint64_t memory[]);

uint64_t fib_rec(uint64_t n)
{
    if (n <= 1)
        return n;

    /* skoro n tutaj >=2 to fib -1 i fib -2 sa bezpieczne */
    return fib_rec(n - 1) + fib_rec(n - 2);
}

uint64_t fib_rec_mem(uint64_t n)
{
    /* robimy +2 zeby moc zrobic [0] [1] nawet gdy n == 0, zawsze mozna zrobic ifa albo zrobic +1 + (n == 0) */
    uint64_t memory[n + 2];

    /* nie da sie = {0} dla VLA, wiec memset */
    memset(&memory[0], 0, sizeof(memory));
    memory[0] = 0;
    memory[1] = 1;

    return _fib_rec_mem(n, &memory[0]);
}

static uint64_t _fib_rec_mem(uint64_t n, uint64_t memory[])
{
    /* jest n == 0, bo mapowanie nie ma injekcji, memory[i] == 0 oznacza ze ity element nie zostal policzony, ale dla 0 elem oznacza tez jego wartosc, stad ten case osobno */
    if (n == 0)
        return 0;

    if (memory[n] != 0)
        return memory[n];

    /* co dla n == 1? wejdzie w memory wiec znowu n - 2 bezpieczne */
    uint64_t result = _fib_rec_mem(n - 1, memory) + _fib_rec_mem(n - 2, memory);

    memory[n] = result;

    return result;
}

uint64_t fib_ite_mem(uint64_t n)
{
    /* gdyby n == 0 to dajemy +2 zeby init dzialal zawsze */
    uint64_t fib_seq[n + 2]; /* mozna tez n + 1 + (n == 0) */
    fib_seq[0] = 0;
    fib_seq[1] = 1;

    for (size_t i = 2; i <= n; ++i)
        fib_seq[i] = fib_seq[i - 1] + fib_seq[i - 2];

    return fib_seq[n];
}

uint64_t fib_ite(uint64_t n)
{
    /* tylko 2 do tylu trzymamy */
    uint64_t cache[2] = {0, 1};

    /* podstawowy case zwroc z cache bez obliczania */
    if (n <= 1)
        return cache[n];

    /* next_fib_val to value do nastepnej petli, dlatego nasz fib[n] siedzi w ostanim cache */
    for (size_t i = 2; i <= n; ++i)
    {
        uint64_t next_fib_val = cache[0] + cache[1];
        cache[0] = cache[1];
        cache[1] = next_fib_val;
    }

    return cache[1];
}