#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "method.h"

struct method
{
    func_ptr func;
    char func_name[FUNC_NAME_MAX_LEN];
    char class_name[CLASS_NAME_MAX_LEN];
};

method_t *MethodCreate(func_ptr func, char *func_name, char *class_name)
{
    method_t *new_method = NULL;

    assert(func);
    assert(func_name);
    assert(class_name);

    new_method = (method_t *)malloc(sizeof(method_t));

    if (NULL == new_method)
    {
        return NULL;
    }

    new_method->func = func;
    strncpy(new_method->func_name, func_name, FUNC_NAME_MAX_LEN);
    strncpy(new_method->class_name, class_name, CLASS_NAME_MAX_LEN);

    return new_method;
}

method_t *MethodDuplicate(method_t *method)
{
    method_t *new_method = NULL;

    assert(method);

    new_method = (method_t *)malloc(sizeof(method_t));

    if (NULL == new_method)
    {
        return NULL;
    }

    new_method->func = method->func;
    strncpy(new_method->func_name, method->func_name, FUNC_NAME_MAX_LEN);
    strncpy(new_method->class_name, method->class_name, CLASS_NAME_MAX_LEN);

    return new_method;
}

void MethodDestroy(method_t *method)
{
    assert(method);

    free(method);
}

int IsMethodMatchName(method_t *method, char *func_name)
{
    int cmp_result = 0;
    assert(method);
    assert(func_name);

    cmp_result = strncmp(method->func_name, func_name, strlen(func_name));
    return (cmp_result == 0);
}

void MethodSetFunc(method_t *method, func_ptr func)
{
    assert(method);
    assert(func);

    method->func = func;
}

void MethodSetClassName(method_t *method, char *class_name)
{
    assert(method);
    assert(class_name);

    strncpy(method->class_name, class_name, CLASS_NAME_MAX_LEN);
}

func_ptr GetFuncPointer(method_t *method)
{
    return method->func;
}