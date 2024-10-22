#include <stddef.h> /* size_t, NULL */
#include <stdio.h>

#include "object.h"
#include "my_class.h"

class_t *object_class;
class_t *mutuble_int_cls;

typedef int (*equals_func_ptr)(object_t *, object_t *);

struct mutuble_int
{
	int value;
};

int Get_obj(object_t *this)
{
	struct mutuble_int *instance_data = (struct mutuble_int *)ObjectGetInstanceData(this, mutuble_int_cls);

	return instance_data->value;
}

void Set_obj_i(object_t *this, int new_value)
{
	struct mutuble_int *instance_data = (struct mutuble_int *)ObjectGetInstanceData(this, mutuble_int_cls);

	instance_data->value = new_value;
}

int MutIsEquals(object_t *this, object_t *other)
{
	struct mutuble_int *this_instance_data = (struct mutuble_int *)ObjectGetInstanceData(this, mutuble_int_cls);
	struct mutuble_int *other_instance_data = (struct mutuble_int *)ObjectGetInstanceData(other, mutuble_int_cls);

	return (this_instance_data->value == other_instance_data->value);
}

int main(void)
{
	struct mutuble_int five = {0};
	object_t *num1 = NULL;
	object_t *num2 = NULL;
	equals_func_ptr equals = NULL;

	five.value = 5;

	object_class = GetObjectClass();

	if (!object_class)
	{
		printf("Get Object class failed!\n");
	}

	mutuble_int_cls = ClassCreate(sizeof(int), object_class, "MutubleInt");

	ClassAddMethod(mutuble_int_cls, (func_ptr)Get_obj, "get_obj");
	ClassAddMethod(mutuble_int_cls, (func_ptr)Set_obj_i, "set_obj_i");
	ClassAddMethod(mutuble_int_cls, (func_ptr)MutIsEquals, "equals_obj_obj");

	num1 = ObjectAllocate(mutuble_int_cls);
	num2 = ObjectAllocate(mutuble_int_cls);

	num1 = ClassCreateNewInstance(mutuble_int_cls, &five, num1);
	num2 = ClassCreateNewInstance(mutuble_int_cls, &five, num2);

	equals = (equals_func_ptr)ClassGetMethod(mutuble_int_cls, "equals_obj_obj");

	if(equals(num1, num2))
	{
		printf("YES!\n");
	}
	else
	{
		printf("NO!\n");
	}

	if(num1 == num2)
	{
		printf("YES!\n");
	}
	else
	{
		printf("NO!\n");
	}

	ObjectDestroy(num1);
	ObjectDestroy(num2);
	
	ClassDestroy(mutuble_int_cls);
	
	ClassDestroy(object_class);

	return 0;
}
