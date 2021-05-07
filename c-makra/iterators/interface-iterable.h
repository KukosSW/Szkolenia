#ifndef INTERFACE_ITERABLE_H
#define INTERFACE_ITERABLE_H

/*
    Interfejs pozwala na iterowanie kolekcji prostymi makrami, ktore bez sa bezpieczne ze wzgledu
    na oryginalna strukture (read only data z iteratora)
    oraz ze wzgledu na memory management (robimy raz return przez value przez to nie mamy dynamic memory)

    Do iterowania uzywamy makr
    FOR_EACH()
    FOR_EACH_REV
*/


/*
    Jesli kolekcja ma wspomagac iterator musi wystawiac nastepujace rzeczy:

    Strukture:
    struct classType_iterator;
    struktura musi byc widoczna na usera, chcemy tworzyc wszystko na stosie
    aby nie zadreczac usera zwalnianiem pamieci

    Funkcje:
    1. Funkcja do inicjalizacji naszego iteratora, bedziemy pozniej brac address, ale teraz wszystko przez value zwracamy
    struct classType_iterator className_iterator_init(const classType* ptrToClass, enum iterable_option_t start);

    2. Funkcje next i prev do chodzenia do przodu i do tylu.
    void className_iterator_next(struct classType_iterator* it)
    void className_iterator_prev(struct classType_iterator* it)

    3. Funkcje do sprawdzania czy juz jestesmy na koncu naszej drogi
    bool className_iterator_is_end(const struct classType_iterator* it)

    4. Funkcja do zwracania wartosci spod iteratora
    void className_iterator_get_value(const struct classType_iterator* it, void* local_addr);

    NA koncu headera zawsze wywolaj te 2 makra:

    INTERFACE_ITERABLE_CHECK_COMPATIBILITY(className, classType)
    INTERFACE_ITERABLE_PRIV_HEADER(className, classType)
*/

typedef enum iterable_option
{
    ITERABLE_OPTION_START_NORMAL = 1 << 0,
    ITERABLE_OPTION_START_REVERSE = 1 << 1,
} iterable_option_t;

#define FOR_EACH(ptrToStruct, classType, local) \
    _FOR_EACH(ptrToStruct, classType, local, classType ## _iterator, CONCAT(iterator_, __LINE__), ITERABLE_OPTION_START_NORMAL, next)

#define FOR_EACH_REV(ptrToStruct, classType, local) \
    _FOR_EACH(ptrToStruct, classType, local, classType ## _iterator, CONCAT(iterator_, __LINE__), ITERABLE_OPTION_START_REVERSE, prev)


#define INTERFACE_ITERABLE_CHECK_COMPATIBILITY(className, classType) \
    static inline void interface_iterable ## className ## classType(void); \
    static inline void interface_iterable ## className ## classType(void) \
    { \
        struct classType ## _iterator it = {0}; \
        (void)it; \
        struct classType ## _iterator (*init)(const classType* ptr, iterable_option_t op) = className ## _iterator_init; \
        void (*next)(struct classType ## _iterator* it) = className ## _iterator_next; \
        void (*prev)(struct classType ## _iterator* it) = className ## _iterator_prev; \
        bool (*is_end)(const struct classType ## _iterator* it) = className ## _iterator_is_end; \
        void (*get_value)(const struct classType ## _iterator* it, void* local_addr) = className ## _iterator_get_value; \
        (void)init; \
        (void)next; \
        (void)prev; \
        (void)is_end; \
        (void)get_value; \
    \
    }

#define INTERFACE_ITERABLE_PRIV_HEADER(className, classType) \
    static inline struct classType ## _iterator classType ## _iterator_init(const classType* ptr, iterable_option_t op); \
    static inline void classType ## _iterator_next(struct classType ## _iterator* it); \
    static inline void classType ## _iterator_prev(struct classType ## _iterator* it); \
    static inline bool classType ## _iterator_is_end(const struct classType ## _iterator* it); \
    static inline int classType ## _iterator_get_value(const struct classType ## _iterator* it, void* local_addr); \
    static inline struct classType ## _iterator classType ## _iterator_init(const classType* ptr, iterable_option_t op) \
    { \
        return className ## _iterator_init(ptr, op); \
    } \
    static inline void classType ## _iterator_next(struct classType ## _iterator* it) \
    { \
        className ## _iterator_next(it); \
    } \
    static inline void classType ## _iterator_prev(struct classType ## _iterator* it) \
    { \
        className ## _iterator_prev(it); \
    } \
    static inline bool classType ## _iterator_is_end(const struct classType ## _iterator* it) \
    { \
        return className ## _iterator_is_end(it); \
    } \
    static inline int classType ## _iterator_get_value(const struct classType ## _iterator* it, void* local_addr) \
    { \
        className ## _iterator_get_value(it, local_addr); \
        return 1; \
    }

#define CONCAT(x, y) _CONCAT(x, y)
#define _CONCAT(x, y) x ## y

#define _FOR_EACH(ptrToStruct, classType, local, prefix, it_name, options, direction) \
    for (struct classType ## _iterator it_name = prefix ## _init(ptrToStruct, options); \
         (!prefix ## _is_end(&it_name) && \
          prefix ## _get_value(&it_name, (void *)&(local)) \
          ); \
         prefix ## _ ## direction(&it_name) \
        )


#endif