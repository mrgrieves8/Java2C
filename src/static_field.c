#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "static_field.h"

struct static_field
{
    void *data;
    size_t size;
    char name[MAX_FIELD_NAME_LEN];
};

static_field_t *StaticFieldCreate(size_t size, char *name)
{
    static_field_t *new_field = NULL;

    assert(size > 0);
    assert(name);
    
    new_field = (static_field_t *)malloc(sizeof(static_field_t));
    if (NULL == new_field)
    {
        return NULL;
    }

    new_field->data = calloc(1, size);
    if (NULL == new_field->data)
    {
        free(new_field);
        return NULL;
    }

    new_field->size = size;
    strncpy(new_field->name, name, MAX_FIELD_NAME_LEN);

    return new_field;
}

void StaticFieldDestroy(static_field_t *this)
{
    assert(this);

    free(this->data);
    free(this);
}

void *StaticFieldGetData(static_field_t *this)
{
    assert(this);

    return this->data;
}

void StaticFieldSetData(static_field_t *this, void *data)
{
    assert(this);
    assert(data);

    memcpy(this->data, data, this->size);
}

int IsStaticFieldMatchStr(static_field_t *this, char *str)
{
    assert(this);
    assert(str);

    return(strncmp(this->name, str, MAX_FIELD_NAME_LEN) == 0);
}