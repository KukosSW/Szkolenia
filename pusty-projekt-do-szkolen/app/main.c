#include <stdio.h>
#include <experiments.h>
#include <foo.h>

int main(void)
{
    printf("MAIN\n");

    MEASURE_FUNCTION(foo_get_n());
    MEASURE_FUNCTION_LABEL(foo_get_n(), "Lucky Number");

    return 0;
}