#include <stddef.h> /* size_t */
#include <string.h> /* strncpy()*/
#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() */
#include <stdatomic.h>	/* atomic_fetch_add(), atomic_store() */
#include <stdio.h>

#include "object.h"

#define INSTANCE_OFFSET(class) (ClassGetTotalInstanceSize(class) - ClassGetMyInstanceSize(class))
#define MY_INSTANCE_OFFSET(this) (INSTANCE_OFFSET((this)->class_data))
#define PARENT_INSTANCE_OFFSET(this) INSTANCE_OFFSET(ClassGetParent((this)->class_data))

static class_t *object_class;

static int InitObjectClass(void);

struct object
{
    class_t *class_data; /* contains static class data and method table */
    void *instance_data;
};

/************************************	OBJECT METHODS	************************************/

int ObjectIsEquals(object_t *this, object_t *other)
{
    return (this == other);
}

int ObjectHashCode(object_t *this)
{
    return (int)(size_t)this;
}

class_t *ObjectGetClass(object_t *this)
{
    assert(this);

    return this->class_data;
}

void ObjectToString(object_t *this, char *buffer)
{
    sprintf(buffer, "%s@%d", ClassGetName(this->class_data), ObjectHashCode(this));
}

void ObjectFinalize(object_t *this)
{
    ObjectDestroy(this);
}

/************************************	API FUNCS	************************************/

object_t *ObjectAllocate(class_t *class_data)
{
    object_t *new_object = NULL;

    assert(class_data);

    new_object = (object_t *)malloc(sizeof(object_t));
    if (NULL == new_object)
    {
        return NULL;
    }

    new_object->instance_data = malloc(ClassGetTotalInstanceSize(class_data));

    if (NULL == new_object->instance_data)
    {
        free(new_object);
        return NULL;
    }

    new_object->class_data = class_data;

    return new_object;
}

void ObjectDestroy(object_t *this)
{
    assert(this);

    free(this->instance_data);
    free(this);
}

class_t *GetObjectClass(void)
{
    InitObjectClass();
    return object_class;
}

void *ObjectGetInstanceData(object_t *this, class_t *class)
{
    assert(this);

    return (void *)((char *)this->instance_data + INSTANCE_OFFSET(class));
}

void ObjectSetInstanceData(object_t *this, void *instance_data, class_t *class)
{
    memcpy((char *)(this->instance_data) + INSTANCE_OFFSET(class), 
            instance_data, ClassGetMyInstanceSize(class));
}

func_ptr ObjectGetMethod(object_t *this, char *func_name)
{
    return ClassGetMethod((ObjectGetClass)(this), func_name);
}
/************************************	STATIC FUNCS	************************************/

static int InitObjectClass(void)
{
    int status = 0;

    if (object_class)
    {
        return 0;
    }

    object_class = ClassCreate(sizeof(object_t), NULL, "Object");

    if (NULL == object_class)
    {
        return 1;
    }

    status = ClassAddMethod(object_class, (func_ptr)ObjectIsEquals, "equals_obj_obj");
    if (0 != status)
    {
        ClassDestroy(object_class);
        return status;
    }

    status = ClassAddMethod(object_class, (func_ptr)ObjectHashCode, "hashCode_obj");
    if (0 != status)
    {
        ClassDestroy(object_class);
        return status;
    }

    status = ClassAddMethod(object_class, (func_ptr)ObjectGetClass, "getClass_obj");
    if (0 != status)
    {
        ClassDestroy(object_class);
        return status;
    }
    
    status = ClassAddMethod(object_class, (func_ptr)ObjectToString, "toString_obj_str");
    if (0 != status)
    {
        ClassDestroy(object_class);
        return status;
    }

    status = ClassAddMethod(object_class, (func_ptr)ObjectFinalize, "finalize_obj");
    if (0 != status)
    {
        ClassDestroy(object_class);
        return status;
    }

    return 0;

}
