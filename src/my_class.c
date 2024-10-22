#include <stddef.h> /* size_t */
#include <string.h> /* strncpy()*/
#include <stdlib.h> /* malloc(), free() */
#include <stdio.h>	/* sprintf() */
#include <assert.h> /* assert() */

#include "my_class.h"
#include "static_field.h"

struct my_class
{
	struct my_class *parent;	   /* pointer to the parent class */
	char name[CLASS_NAME_MAX_LEN]; /* the name of the class */
	v_table_t *v_table;
	size_t total_instance_size;
	size_t my_instance_size;
	void *instance_data_default;
	static_init_func static_init_block;
	instance_init_func instance_init_block;
	static_field_t *static_data[MAX_N_STATIC_FIELDS];
	size_t n_static_fields;
	int is_init;
};

static v_table_t *CreateVtableFromParent(class_t *parent);
static void ClearStaticFields(class_t *this);
static object_t *WriteInstanceData(class_t *this, void *instance_data, object_t *new_instance);

/************************************	API FUNCS	************************************/

class_t *ClassCreate(size_t instance_size, class_t *parent, char *name)
{
	class_t *new_class = NULL;
	size_t parent_instance_size = 0;

	assert(name);

	new_class = (class_t *)malloc(sizeof(class_t));

	if (NULL == new_class)
	{
		return NULL;
	}

	new_class->v_table = CreateVtableFromParent(parent);

	if (NULL == new_class->v_table)
	{
		free(new_class);
		return NULL;
	}

	new_class->instance_data_default = calloc(1, instance_size);
	if (NULL == new_class->instance_data_default)
	{
		VtableDestroy(new_class->v_table);
		free(new_class);
		return NULL;
	}

	if (NULL != parent)
	{
		parent_instance_size = ClassGetTotalInstanceSize(parent);
	}

	new_class->total_instance_size = instance_size + parent_instance_size;
	new_class->my_instance_size = instance_size;
	new_class->parent = parent;
	strncpy(new_class->name, name, CLASS_NAME_MAX_LEN);
	new_class->static_init_block = NULL;
	new_class->instance_init_block = NULL;
	new_class->n_static_fields = 0;
	new_class->is_init = 0;

	return new_class;
}

void ClassDestroy(class_t *this)
{
	assert(this);

	VtableDestroy(this->v_table);
	free(this->instance_data_default);
	ClearStaticFields(this);
	free(this);
}

object_t *ClassCreateNewInstance(class_t *this, void *instance_data, object_t *new_instance)
{
	assert(this);

	ClassStaticInitBlockExec(this);

	if (NULL == this->parent)
	{
		return WriteInstanceData(this, instance_data, new_instance);
	}

	new_instance = ClassCreateNewInstance(this->parent, this->parent->instance_data_default, new_instance);

	return WriteInstanceData(this, instance_data, new_instance);
}

object_t *ClassExtendNewInstance(class_t *this, void *instance_data, ctor_func super_ctor, object_t *new_instance)
{
	assert(this);
	assert(this->parent);

	ClassStaticInitBlockExec(this);

	new_instance = super_ctor(new_instance);

	return WriteInstanceData(this, instance_data, new_instance);
}

int ClassAddMethod(class_t *this, func_ptr func, char *func_name)
{
	assert(this);
	assert(func);
	assert(func_name);

	return VtableAddMethod(this->v_table, func, func_name, this->name);
}

size_t ClassGetTotalInstanceSize(class_t *this)
{
	assert(this);

	return this->total_instance_size;
}

size_t ClassGetMyInstanceSize(class_t *this)
{
	assert(this);

	return this->my_instance_size;
}

void ClassSetDefaultInstanceData(class_t *this, void *instance_data_default)
{
	assert(this);
	assert(instance_data_default);

	memcpy(this->instance_data_default, instance_data_default, this->my_instance_size);
}

func_ptr ClassGetMethod(class_t *this, char *func_name)
{
	method_t *method = NULL;

	assert(this);
	assert(func_name);

	method = VtableFindMethod(this->v_table, func_name);

	if (NULL == method)
	{
		return NULL;
	}

	return GetFuncPointer(method);
}

char *ClassGetName(class_t *this)
{
	assert(this);

	return this->name;
}

size_t ClassAddNewStaticField(class_t *this, char *field_name, size_t size)
{
	static_field_t *new_field = NULL;
	char field_full_name[MAX_FIELD_NAME_LEN] = {'\0'};
	size_t ret = 0;

	assert(this);
	assert(field_name);
	assert(size > 0);

	sprintf(field_full_name, "%s,%s", this->name, field_name);
	new_field = StaticFieldCreate(size, field_full_name);

	if (NULL == new_field)
	{
		return MAX_N_STATIC_FIELDS;
	}

	this->static_data[this->n_static_fields] = new_field;

	ret = this->n_static_fields++;

	return ret;
}

void *ClassGetStaticFieldByName(class_t *this, char *field_name)
{
	size_t index = 0;
	char field_full_name[MAX_FIELD_NAME_LEN] = {'\0'};

	assert(this);
	assert(field_name);

	for (index = 0; index < this->n_static_fields; index++)
	{
		sprintf(field_full_name, "%s,%s", this->name, field_name);
		if (IsStaticFieldMatchStr(this->static_data[index], field_full_name))
		{
			return StaticFieldGetData(this->static_data[index]);
		}
	}

	return NULL;
}

void ClassSetStaticFieldByName(class_t *this, char *field_name, void *data)
{
	size_t index = 0;
	char field_full_name[MAX_FIELD_NAME_LEN] = {'\0'};

	assert(this);
	assert(field_name);
	assert(data);

	for (index = 0; index < MAX_N_STATIC_FIELDS; index++)
	{
		sprintf(field_full_name, "%s,%s", this->name, field_name);
		if (IsStaticFieldMatchStr(this->static_data[index], field_full_name))
		{
			StaticFieldSetData(this->static_data[index], data);
			return;
		}
	}
}

void *ClassGetStaticFieldByIndex(class_t *this, size_t index)
{
	assert(this);
	assert(this->n_static_fields > index);

	return StaticFieldGetData(this->static_data[index]);
}

void ClassSetStaticFieldByIndex(class_t *this, size_t index, void *data)
{
	assert(this);
	assert(this->n_static_fields > index);
	assert(data);

	StaticFieldSetData(this->static_data[index], data);
}

void ClassSetStaticInitBlock(class_t *this, static_init_func static_init_block)
{
	assert(this);

	this->static_init_block = static_init_block;
}

void ClassSetInstanceInitBlock(class_t *this, instance_init_func instance_init_block)
{
	assert(this);

	this->instance_init_block = instance_init_block;
}

class_t *ClassGetParent(class_t *this)
{
	assert(this);

	return this->parent;
}

void *ClassGetDefaultInstanceData(class_t *this)
{
	assert(this);

	return this->instance_data_default;
}

void ClassInstanceInitBlockExec(class_t *this, object_t *new_object)
{
	assert(this);

	if (this->instance_init_block)
	{
		this->instance_init_block(new_object);
	}
}

void ClassStaticInitBlockExec(class_t *this)
{
	assert(this);
	if (this->parent && !this->parent->is_init)
	{
		ClassStaticInitBlockExec(this->parent);
	}
	
	if (this->static_init_block && !this->is_init)
	{
		this->static_init_block(this);
		this->is_init = 1;
	}
}


/************************************	STATIC FUNCS	************************************/

static v_table_t *CreateVtableFromParent(class_t *parent)
{
	if (NULL == parent)
	{
		return VtableCreate();
	}
	else
	{
		return VtableDuplicate(parent->v_table);
	}
}

static void ClearStaticFields(class_t *this)
{
	size_t index = 0;

	assert(this);

	for (index = 0; index < this->n_static_fields; index++)
	{
		StaticFieldDestroy(this->static_data[index]);
	}
}

static object_t *WriteInstanceData(class_t *this, void *instance_data, object_t *new_instance)
{
	assert(this);
	assert(new_instance);

	if (NULL == instance_data)
	{
		instance_data = this->instance_data_default;
	}

	ObjectSetInstanceData(new_instance, instance_data, this);

	ClassInstanceInitBlockExec(this, new_instance);

	return new_instance;
}