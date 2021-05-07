#include <file4.h>
#include <file_common.h>
#include <macros.h>

void foo4(void)
{
    const int a = 5;
    const int b = 6;
	print("FOO4 min(%d, %d) = %d\n", a, b, MIN(a, b));
}
