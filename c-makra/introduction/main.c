#include <stdio.h>

/*
PREPROCESSOR:

# linenum filename flags
‘1’
This indicates the start of a new file.
‘2’
This indicates returning to a file (after having included another file).
‘3’
This indicates that the following text comes from a system header file, so certain warnings should be suppressed.
‘4’
This indicates that the following text should be treated as being wrapped in an implicit extern "C" block.
*/

#include <file2.h>
#include <file3.h>
#include <file5.h>
int main(void)
{
    printf(DEF1_MSG "\n");
    printf(DEF2_MSG "\n");

    printf(TOSTRING(DEF1) "\n");
    printf(TOSTRING_VAL(DEF1) "\n");
    printf(MACRO_DEBUG(DEF3) "\n");

/*
    int a = FOO1(10);
    int b = FOO(5);
    int c = BAR(2);
*/

    printf(MACRO_DEBUG(A) "\n");
    printf(MACRO_DEBUG(TMP_A) "\n");
    return 0;
}