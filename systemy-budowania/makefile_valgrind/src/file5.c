#include <file5.h>
#include <file_common.h>
#include <macros.h>

void foo5(void)
{
    const int a = 5;
    const int b = 6;
	print("FOO5 max(%d, %d) = %d\n", a, b, MAX(a, b));
}
