#include <foo.h>
#include <unistd.h>

int foo_get_n(void)
{
    sleep(1);
    return 41; /* last lucky number */
}