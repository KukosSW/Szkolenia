#include <object-priv.h>
#include <stdlib.h>
#include <string.h>

struct Object
{
    get_name_f get_name;
    to_chars_f to_chars;
    delete_f   deletef;
    size_t     object_size;
    char       object[];
};

Object* object_new(void* object, size_t object_size, get_name_f get_name, to_chars_f to_chars, delete_f deletef)
{
    Object* nobject;
    nobject = malloc(sizeof(*nobject) + object_size);
    nobject->object_size = object_size;
    nobject->get_name = get_name;
    nobject->to_chars = to_chars;
    nobject->deletef = deletef;
    memcpy(&nobject->object[0], object, object_size);

    return nobject;
}

const char* object_get_name(const Object *object)
{
    return object->get_name != NULL ? object->get_name() : "";
}

const char* object_to_chars(const Object* object)
{
    return  object->to_chars != NULL ? object->to_chars(&object->object[0]) : "";
}

void object_delete(Object* object)
{
    if (object->deletef != NULL)
        object->deletef(&object->object[0]);

    free(object);
}