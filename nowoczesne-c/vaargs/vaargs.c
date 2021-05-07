#include <vaargs.h>
#include <stdio.h>

/*
        Jak dziala ... w skrocie?
        Ogolnie to trzeba rozwazyc wiele rzeczy.
        Po 1. Jak sie uklada stos? Do gory / w dol?
        Po 2. Jak caller wklada nam itemki? Normalnie czy odwrotnie?
        Po 3. Kompilator musi wylaczyc przekazywanie przez rejestr

        Skoro musimy miec 1 argument to wiemy jaki jest address stosu.
        Mozemy zrobic:
        va_start robi
        char *pointer_to_args = &n + sizeof(n)

        Jesli wiemy ze tam sa tylko inty no to va_arg robi:
        int a = *(int *)pointer_to_args;
        pointer_to_args += sizeof(int)

        va_end zrobi:
        pointer_to_args = NULL
*/
int sum(size_t n, ...)
{
    int _sum = 0;
    va_list args;
    va_start(args, n);

    for (size_t i = 0; i < n; ++i)
        _sum += va_arg(args, int);

    va_end(args);

    return _sum;
}

/*
    A co jesli ktos chce wywolac sume juz rozwijajac argsy???
    Musimy zrobic druga funkcje, gdzie mozna dac va_list
    Konwencja nazwa mowi abysmy dali przedrostek v i tyle
*/
int vsum(size_t n, va_list args)
{
    int _sum = 0;

    for (size_t i = 0; i < n; ++i)
        _sum += va_arg(args, int);

    return _sum;
}

/* Hmm no dobra ale to w takim razie suma powinna wygladac tak: */
// int sum(size_t n, ...)
// {
//     va_list args;
//     va_start(args, n);

//     const int _sum = vsum(n, args);

//     va_end(args);

//     return _sum;
// }

void vprintf_simply(const char* fmt, va_list args)
{
    const char* current_fmt = fmt;

    while (*current_fmt != '\0')
    {
        /* Jesli ktos chcial jakis char ok print go */
        while (*current_fmt != '\0' && *current_fmt != '%')
        {
            putchar(*current_fmt);
            ++current_fmt;
        }

        if (*current_fmt == '\0')
            break;

        /* Jestesmy na %, wiec +1 */
        ++current_fmt;

        /* Obslugujemy %c %d %l %p */
        switch (*current_fmt)
        {
            case 'c':
            {
                /* va_arg robi promocje char, short do inta */
                const char c = (char)va_arg(args, int);
                putchar(c);
                break;
            }
            case 'd':
            {
                const int i = va_arg(args, int);

                /* Dobra bez jaj nie chce mi sie pisac printfa aby pokazac jak dzialaja va_argsy */
                printf("%d", i);
                break;
            }
            case 'l':
            {
                const long l = va_arg(args, long);
                printf("%ld", l);
                break;
            }
            case 'p':
            {
                const void* p = va_arg(args, void*);
                printf("%p", p);

                break;
            }
            default:
            {
                putchar(*current_fmt);
                break;
            }
        }
        ++current_fmt;
    }
}

void printf_simply(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    vprintf_simply(fmt, args);

    va_end(args);
}

int sum_with_debug(size_t n, ...)
{
    va_list args_print;
    va_list args_sum;

    va_start(args_print, n);

    /*
        The object ap may be passed as an argument to another function;
        if that function invokes the va_arg macro with parameter ap,
        the value of ap in the calling function is indeterminate and shall be passed
         to the va_end macro prior to any further reference to ap.
    */
    va_copy(args_sum, args_print);

    char fmt[n * 3 + 1 + 1];
    for (size_t i = 0; i < n; ++i)
    {
        fmt[i * 3] = '%';
        fmt[i * 3 + 1] = 'd';
        fmt[i * 3 + 2] = ' ';
    }
    fmt[sizeof(fmt) - 2] = '\n';
    fmt[sizeof(fmt) - 1] = '\0';

    vprintf_simply(&fmt[0], args_print);
    va_end(args_print);

    const int _sum = vsum(n, args_sum);
    va_end(args_sum);

    return _sum;
}

void struct_passed_to_valist(size_t n, ...)
{
    va_list args;
    va_start(args, n);


    for (size_t i = 0; i < n; ++i)
    {
        const Foo foo = va_arg(args, Foo);
        printf("{%d, %ld} ", foo.a, foo.b);
    }

    va_end(args);
    printf("\n");
}