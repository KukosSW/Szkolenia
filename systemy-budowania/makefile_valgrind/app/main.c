#include <file.h>
#include <file2.h>
#include <file3.h>
#include <file4.h>
#include <file5.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *t = malloc(sizeof(*t));
	printf("MAIN\n");
	foo();
    foo2();
    foo3();
    foo4();
    foo5();
    printf("%p\n", t);
    //t[2] = 0;
    free(t);
	return 0;
}
