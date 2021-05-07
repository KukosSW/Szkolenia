#include <object-legacy.h>

const char* legacy_get_name(void)
{
    return "To jest obiekt z czystego C, nie wspomaga tego interfejsu";
}

const char* legacy_to_chars(const void* object)
{
    (void)object;
    return "";
}