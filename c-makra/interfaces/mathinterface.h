#ifndef MATHINTERFACE_H
#define MATHINTERFACE_H

/*
    Chcemy zobaczyc czy jakas klasa jest kompatybilna z naszym interfejsem
    Powiedzmy ze nasz interfejs to libka matematyczna:
    Kazdy kompatybilny z nami musi miec funkcje:

    type add(type a, type b)
    type sub(type a, type b)
    type mul(type a, type b)
    type div(type a, type b)
*/

#define MATHINTERFACE_CHECK_COMPATIBILITY(className, classType) \
    static inline void mathinterface_check_## className ## classType(void) \
    { \
        classType (*add_f)(classType a, classType b) = className##_add; \
        classType (*sub_f)(classType a, classType b) = className##_sub; \
        classType (*mul_f)(classType a, classType b) = className##_mul; \
        classType (*div_f)(classType a, classType b) = className##_div; \
        (void)add_f; \
        (void)sub_f; \
        (void)mul_f; \
        (void)div_f; \
    }


#endif