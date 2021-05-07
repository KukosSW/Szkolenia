#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void test_famincorrect(void);
static void test_famcorrect(void);
static void test_famextension(void);

static void sandbox(void);

typedef struct FAMIncorrect
{
    size_t len;
    int payload[1];
} FAMIncorrect;

static void test_famincorrect(void)
{
    /* Mozemy na stosie, bedzie wtedy char[1] */
    FAMIncorrect fam1;
    (void)fam1;

    printf("sizeof(FAMIncorrect) = %zu\n", sizeof(FAMIncorrect));

    /* Probujemy recznie rozszerzyc, ale kompi wie ze to tablica [1] jest warning  */
    FAMIncorrect fam2 = {5, {1, 2, 3, 4, 5}};
    printf("FAMIncorrect stos\t");
    for (size_t i = 0; i < fam2.len; ++i)
    /* Co wiecej odwolanie sie do elementow spoza tablicy w taki sposob to UB */
        printf("%4d", fam2.payload[i]);
    printf("\n");

    /* Zobaczmy ze jak chcemy miec 5 elementow w paylaod to sizeof(struct) juz da na z 1 elementem */
    FAMIncorrect* fam3 = malloc(sizeof(*fam3) + (sizeof(fam3->payload[0]) * (5 - 1)));
    fam3->len = 5;
    for (size_t i = 0; i < fam3->len; ++i)
        fam3->payload[i] = (int)(i + 1);

    /* To dalej jest UB, mamy fart ze to dziala */
    printf("FAMIncorrect sterta\t");
    for (size_t i = 0; i < fam3->len; ++i)
        printf("%4d", fam3->payload[i]);
    printf("\n");


    printf("\n\n");
    free(fam3);
}

typedef struct FAMCorrect
{
    size_t len;
    int payload[];
} FAMCorrect;

/* Co ciekawe mozna FAM z palca inicjalizowac, ale to musi byc statyczne */
/* FAMCorrect f = {5, {1, 2, 3, 4, 5}}; */


static void test_famcorrect(void)
{
    /* Mozemy na stosie, bedzie wtedy samo len */
    FAMCorrect fam1;
    (void)fam1;

    printf("sizeof(FAMCorrect) = %zu\n", sizeof(FAMCorrect));

    /* non-static initialization of flexible array member is not allowed */
    /* FAMCorrect fam2 = {5, {1, 2, 3, 4, 5}}; */
    /* Tak mozna */
    /* static FAMCorrect f = {5, {1, 2, 3, 4, 5}}; */

    /* sizeof(fam3->payload) to blad bo nie znamy size typu int[], ale juz fam3->payload[0] to zwykly int wiec OK! */
    FAMCorrect* fam3 = malloc(sizeof(*fam3) + (sizeof(fam3->payload[0]) * 5));
    fam3->len = 5;

    /* To juz nie jest UB, wszystko jest OK, bo mamy syntax ficzera FAM z C99 */
    for (size_t i = 0; i < fam3->len; ++i)
        fam3->payload[i] = (int)(i + 1);

    printf("FAMCorrect sterta\t");
    for (size_t i = 0; i < fam3->len; ++i)
        printf("%4d", fam3->payload[i]);
    printf("\n");


    printf("\n\n");
    free(fam3);
}

/* int [0] to takie FAM dla gnu89, jesli chodzi o sizeof to taki jak FAM, ale inne mechanizmy ma jak int t[1] */
typedef struct FamExtension
{
    size_t len;
    __extension__ int payload[0];
} FamExtension;

static void test_famextension(void)
{
    /* Mozemy na stosie, bedzie wtedy samo len */
    FamExtension fam1;
    (void)fam1;

    printf("sizeof(FamExtension) = %zu\n", sizeof(FamExtension));

    /* Probujemy recznie rozszerzyc, ale kompi wie ze to tablica [0] jest warning  */
    FamExtension fam2 = {5, {1, 2, 3, 4, 5}};
    printf("FamExtension stos\t");
    for (size_t i = 0; i < fam2.len; ++i)
        printf("%4d", fam2.payload[i]);
    printf("\n");

    FamExtension* fam3 = malloc(sizeof(*fam3) + (sizeof(fam3->payload[0]) * 5));
    fam3->len = 5;

    /* To juz nie jest UB, wszystko jest OK, bo mamy syntax ficzera FAM z C99 */
    for (size_t i = 0; i < fam3->len; ++i)
        fam3->payload[i] = (int)(i + 1);

    printf("FamExtension sterta\t");
    for (size_t i = 0; i < fam3->len; ++i)
        printf("%4d", fam3->payload[i]);
    printf("\n");

    printf("\n\n");
    free(fam3);
}

typedef struct NormalStruct
{
    size_t len;
    int *payload;
} NormalStruct;

static void sandbox(void)
{
    NormalStruct* normal;
    FAMCorrect* fam;

    const size_t n = 5;

    /* Tworzenie 1x malloc zamiast 2x malloc  */
    normal = malloc(sizeof(*normal));
    normal->payload = malloc(sizeof(*normal->payload) * n);
    normal->len = n;

    fam = malloc(sizeof(*fam) + sizeof(fam->payload[0]) * n);
    fam->len = n;

    /* Kopiowanie to samo 1x memcpy zamiast 2x */

    /* Cache, tez wygrywamy, wczytujac len mozliwe ze mamy w cacheline tez kawalek payload */
    /*
        Dlatego wazne ze jesli chcemy miec 2x FAM to jako FAM robimy ten element do ktorego sie czesciej odwolujemy
        Ten drugi jako zwykly pointer, bo przeciez nie mozna robic 2x FAM
    */

   free(normal->payload);
   free(normal);
   free(fam);
}

/* Czego nie wolno z FAM */

/* FAM jest tylko 1, jesli chcemy kilka to trzeba wybrac ten na ktorym wiecej zyskamy */
struct Foo1
{
    size_t len;
    /* int t[]; flexible array member not at end of struct */
    int t2[];
};

/* FAM musi miec wczesniej jakis normalny typ */
/*
struct Foo2
{
    flexible array member in a struct with no named members
    int t[];
};
*/

/* Struktura z FAM nie moze byc struktura w innej strukturze */
struct Foo3
{
    int a;
    int b;
    /* invalid use of structure with flexible array member */
    FAMCorrect fam;
};

/* Nie mozna miec tablicy struktur typu FAM */
FAMCorrect t[100];  /* invalid use of structure with flexible array member */

int main(void)
{
    test_famincorrect();
    test_famcorrect();
    test_famextension();

    sandbox();

    return 0;
}