#include <stdlib.h>
#include <assert.h>

#include "v_table.h"
#include "method.h"

#define INITIAL_DEFAULT_CAPACITY 8
#define MAX_METHODS 24

struct v_table
{
    method_t *method_arr[MAX_METHODS];
};

static int InsertMethod(v_table_t *table, method_t *new_method);
static void VtableClear(v_table_t *table);

/************************************	API FUNCS	************************************/

v_table_t *VtableCreate(void)
{
    v_table_t *new_table = NULL;

    new_table = (v_table_t *)calloc(1, sizeof(v_table_t));

    if (NULL == new_table)
    {
        return NULL;
    }
    
    return new_table;
}

v_table_t *VtableDuplicate(v_table_t *parent_table)
{
    v_table_t *new_table = NULL;
    size_t index = 0;
    method_t *method = NULL;

    assert(parent_table);

    new_table = (v_table_t *)calloc(1, sizeof(v_table_t));

    if (NULL == new_table)
    {
        return NULL;
    }

    while (parent_table->method_arr[index])
    {
        method = MethodDuplicate(parent_table->method_arr[index]);

        InsertMethod(new_table, method);

        ++index;
    }

    return new_table;
}

void VtableDestroy(v_table_t *table)
{
    assert(table);

    VtableClear(table);

    free(table);
}

int VtableAddMethod(v_table_t *table, func_ptr func, char *func_name, char *class_name)
{
    method_t *new_method = NULL;
    method_t *old_method = NULL;

    assert(func);
    assert(func_name);
    assert(class_name);

    old_method = VtableFindMethod(table, func_name);
    
    if (NULL == old_method)
    {
        new_method = MethodCreate(func, func_name, class_name);

        if (NULL == new_method)
        {
            return 1;
        }
        if (InsertMethod(table, new_method))
        {
            MethodDestroy(new_method);
            
            return 1;
        }
        
    }
    else /* override */
    {
        MethodSetFunc(old_method, func);
        MethodSetClassName(old_method, class_name);
    }
    
    return 0;
}

method_t *VtableFindMethod(v_table_t *table, char *func_name)
{
    size_t index = 0;

    assert(table);
    assert(func_name);

    while (table->method_arr[index])
    {
        if (IsMethodMatchName(table->method_arr[index], func_name))
        {
            return table->method_arr[index];
        }

        ++index;
    }

    return NULL;
    
}

/************************************	STATIC FUNCS	************************************/

static int InsertMethod(v_table_t *table, method_t *new_method)
{
    size_t index = 0;

    while (MAX_METHODS > index && table->method_arr[index])
    {
        ++index;
    }

    if (index != MAX_METHODS)
    {
        table->method_arr[index] = new_method;
    }
    
    return 0;
}

static void VtableClear(v_table_t *table)
{
    size_t index = 0;

    assert(table);

    while (table->method_arr[index])
    {
        MethodDestroy(table->method_arr[index]);

        ++index;
    }    
}
