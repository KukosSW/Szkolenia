#include <stdio.h>
#include <string.h>

#include <normal.h>
#include <forward.h>
#include <convention.h>

int main(void)
{
    FooNormal foonormal = {1, 2, 3, 4};
    foonormal.a++;
    foonormal.b--;
    foonormal.c = 10;

    print_fooNormal(&foonormal);

    FooForward* fooforward;
    fooforward = fooforward_create(1, 2, 3, 4);
    /* fooforward->a++ blad, bo ja nie wiem co to jest fooforward, moge przekazywac tylko pointery */

    fooforward_set_a(fooforward, fooforward_get_a(fooforward) + 1);
    fooforward_set_b(fooforward, fooforward_get_b(fooforward) - 1);
    fooforward_set_c(fooforward, 10);

    fooforward_print(fooforward);
    fooforward_destroy(fooforward);

    FooConv fooconv = {1, 2, 3, 4};
    fooconv.a++; /* public wg umowy OK */
    fooconv.__b--; /* private wg umowy, ale to tylko umowa :D */
    fooconv_set_c(&fooconv, 10);

    fooconv_print(&fooconv);

    /* Kto powiedzial ze nie wolno forwarda alokowac na stacku? No przyznac sie :D */
    char buffer_vla[fooforward_sizeof()];
    fooforward = (FooForward *)&buffer_vla[0];

    fooforward_print(fooforward);
    memset(fooforward, 0, fooforward_sizeof());
    fooforward_print(fooforward);

    fooforward_set_a(fooforward, fooforward_get_a(fooforward) + 1);
    fooforward_set_b(fooforward, fooforward_get_b(fooforward) - 1);
    fooforward_set_c(fooforward, 10);

    fooforward_print(fooforward);
    return 0;
}