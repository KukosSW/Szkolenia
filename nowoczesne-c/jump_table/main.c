#include <stdio.h>

#define PRINT_FUNC() printf("%s:\t", __func__)

/* Taka uwaga, kod wygenerowalem GCC, clang wszedzie ogarnal ze mozna zrobic jump table */

void func1_ifelse(int n);
void func1_switch(int n);
void func1_trick(int n);

void func2_ifelse(int n);
void func2_switch(int n);
void func2_trick(int n);

/*
	mov	edx, 2
	cmp	edi, 1
	je	.L2
	xor	edx, edx # mov edx, 0
	cmp	edi, 2
	je	.L2
	mov	edx, 2
	cmp	edi, 3
	je	.L2
	xor	edx, edx # mov edx, 0
	cmp	edi, 4
*/
void func1_ifelse(int n)
{
    PRINT_FUNC();

    int k = 1;
    if (n == 1)
        k = k + 1;
    else if (n == 2)
        k = k - 1;
    else if (n == 3)
        k = k * 2;
    else if (n == 4)
        k = k / 2;

    printf("K = %d\n", k);
}

/*
    sub	edi, 1
	mov	edx, 1
	cmp	edi, 3
	ja	.L11
	lea	rax, CSWTCH.5[rip]
	mov	edx, DWORD PTR [rax+rdi*4]

    CSWTCH.5:
	.long	2
	.long	0
	.long	2
	.long	0
*/
void func1_switch(int n)
{
    PRINT_FUNC();

    int k = 1;

    switch (n)
    {
        case 1:
            k = k + 1;
            break;
        case 2:
            k = k - 1;
            break;
        case 3:
            k = k * 2;
            break;
        case 4:
            k = k / 2;
            break;
        default:
            break; /* Jesli bylby default, to zaladuj default a potem nadpisz jesli wejdziesz w case */
    }

    printf("K = %d\n", k);
}

void func1_trick(int n)
{
    PRINT_FUNC();

    int k = 1;
    const int t[] = {1, 2, 0, 2, 0};

    if (n > 0 && n < 4)
        k = t[n];

    printf("K = %d\n", k);
}

/*
    cmp	edi, 1
	je	.L26
	cmp	edi, 2
	je	.L27
	cmp	edi, 3
	je	.L28
	cmp	edi, 4
	je	.L29
*/
void func2_ifelse(int n)
{
    PRINT_FUNC();

    if (n == 1)
        printf("N = 1\n");
    else if (n == 2)
        printf("N = 2\n");
    else if (n == 3)
        printf("N = 3\n");
    else if (n == 4)
        printf("N = 4\n");
}

/*
    cmp	edi, 3
	je	.L31
	jg	.L32
	cmp	edi, 1
	je	.L33
	cmp	edi, 2
	jne	.L37
	lea	rdi, .LC3[rip]
	jmp	puts@PLT
*/
void func2_switch(int n)
{
    PRINT_FUNC();

    switch (n)
    {
        case 1:
            printf("N = 1\n");
            break;
        case 2:
            printf("N = 2\n");
            break;
        case 3:
            printf("N = 3\n");
            break;
        case 4:
            printf("N = 4\n");
            break;
        default:
            break;
    }
}

void __p1(void);
void __p1(void)
{
    printf("N = 1\n");
}

void __p2(void);
void __p2(void)
{
    printf("N = 2\n");
}

void __p3(void);
void __p3(void)
{
    printf("N = 3\n");
}

void __p4(void);
void __p4(void)
{
    printf("N = 4\n");
}

void func2_trick(int n)
{
    PRINT_FUNC();

    /* Moglbym zrobic const char* strings[] = { } */
    /* Korzystaja z okacji pokaze bardziej generyczny patenciak */
    void (* const func_p[])(void) = {__p1, __p2, __p3, __p4};
    if (n > 0 && n < 5)
        func_p[n - 1]();
}

#ifdef __GNUC__
void func3_trick(int n );
/* gcc oferuje nam && jako adres labelki, mozna recznie robic jump table */
void func3_trick(int n )
{
    PRINT_FUNC();

    const void* label_p[] = {&&N1, &&N2, &&N3, &&N4};

    if (n < 1 || n > 4)
        goto RET;

    goto *label_p[n - 1];

    N1:
        printf("N = 1\n");
        goto RET;
    N2:
        printf("N = 2\n");
        goto RET;
    N3:
        printf("N = 3\n");
        goto RET;
    N4:
        printf("N = 4\n");
        goto RET;

    RET:
        return;
}
#endif

int main(void)
{
    volatile int n = 2;

    func1_ifelse(n);
    func1_switch(n);
    func1_trick(n);

    func2_ifelse(n);
    func2_switch(n);
    func2_trick(n);

#ifdef __GNUC__
    func3_trick(n);
#endif

    return 0;
}