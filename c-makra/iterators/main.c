#include <stdio.h>
#include <array.h>

int main(void)
{
    const size_t n = 10;
    Array* a = array_create(n);
    for (size_t i = 0; i < n; ++i)
        array_set(a, i, (int)i + 1);

    printf("Od poczatku\n");
    int local;
    FOR_EACH(a, Array, local)
    {
        FOR_EACH(a, Array, local)
            printf("Local = %d\n", local);
    }

    printf("Od konca\n");
    FOR_EACH_REV(a, Array, local)
    {
        printf("Local = %d\n", local);

        /* Sprawdzmy, ze jak jest break to nic sie nie dzieje */
        if (local == 4)
            break;
    }

    array_destroy(a);

    return 0;
}