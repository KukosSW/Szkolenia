#ifndef DYNAMIC_POLY_H
#define DYNAMIC_POLY_H

typedef const char* (*get_name_f)(void);
typedef void (*print_object_f)(const void *object);
typedef void (*delete_f)(void* object);

typedef struct Object
{
    void* object;
    get_name_f get_name;
    print_object_f print_object;
    delete_f deletef;
} Object;

Object* object_create(void* object, get_name_f get_name, print_object_f print_f, delete_f deletef);
void object_destroy(Object* obj);

static inline const char* object_get_name(const Object* obj)
{
    return obj->get_name();
}

static inline void object_print(const Object *obj)
{
    obj->print_object(obj->object);
}

#endif