#include <dynamic_poly.h>
#include <stdlib.h>

Object* object_create(void* object, get_name_f get_name, print_object_f print_f, delete_f deletef)
{
    Object* obj = malloc(sizeof(*obj));
    obj->object = object;
    obj->get_name = get_name;
    obj->print_object = print_f;
    obj->deletef = deletef;

    return obj;
}

void object_destroy(Object* obj)
{
    obj->deletef(obj->object);
    free(obj);
}
