#ifndef __MY_CLASS_H__
#define __MY_CLASS_H__

#include <stddef.h>	/* size_t */

#include "v_table.h"

typedef struct my_class class_t;


#define MAX_N_STATIC_FIELDS 20 

#include "object.h"

typedef object_t *(*ctor_func)(object_t *);
typedef void (*static_init_func)(class_t *);
typedef void (*instance_init_func)(object_t *);

/******************************************* 
                ClassCreate
creates a new class.
    instance_size:  the size needed for the
    fields of an instance of the new class.
    parent: a class pointer to the parent class.
    name:   the name of the new class.

every class created via this function should
be destroyed by calling ClassDestroy when done.
********************************************/
class_t *ClassCreate(size_t instance_size, class_t *parent, char *name);

/******************************************* 
                ClassDestroy
destroys a class
********************************************/			
void ClassDestroy(class_t *this);

/******************************************* 
                ClassAddMethod
adds method to a class's v_table.
    class: the class to add the method to.
    func: a function pointer casted to (func_ptr).
    func_name: the name that will identify
    the method, for method equals that recieves
    two obj, write: "equals_obj_obj". 
********************************************/
int ClassAddMethod(class_t *this, func_ptr func,char *func_name);

/******************************************* 
                ClassGetTotalInstanceSize
returns the total size allocated for a instanca_data
of an instance of that class, including the space
needed for it's ancestors instance data.
********************************************/
size_t ClassGetTotalInstanceSize(class_t *this);

/******************************************* 
                ClassGetTotalInstanceSize
returns the size allocated for a instanca_data
of an instance of that class alone, excluding the space
needed for it's ancestors instance data.
********************************************/
size_t ClassGetMyInstanceSize(class_t *this);

/******************************************* 
                ClassCreateNewInstance
writes instance data to instance. calling the default super ctor
and the init blocks.

recieves the relevant class, initial instance data
and an allocated object (with class metadata).

instance_data can be null which will use the
default instance data for creation (like calling
no-args constructor) or a pointer to specific 
instance data that will be copied into the
instance data of the new instance. 

every object created via this function should
be destroyed by calling ObjectDestroy or
by using the finalize_obj method when done.
********************************************/
object_t *ClassCreateNewInstance(class_t *this, void *instance_data, object_t *new_instance);

/******************************************* 
                ClassExtendNewInstance
writes instance data to instance. calling a specific super ctor
and the init blocks.

recieves the relevant class, initial instance data,
a pointer to the super constructor and an allocated 
object (with class metadata).

instance_data can be null which will use the
default instance data for creation (like calling
no-args constructor) or a pointer to specific 
instance data that will be copied into the
instance data of the new instance. 

the ctor_func function pointer type returns an
object and takes an object. to use an ctor that
take other args, warp the ctor with a func that takes only an object
and returns a call to the ctor with the args it requires

every object created via this function should
be destroyed by calling ObjectDestroy or
by using the finalize_obj method when done.
********************************************/
object_t *ClassExtendNewInstance(class_t *this, void *instance_data, ctor_func super_ctor, object_t *new_instance);

/******************************************* 
                ClassSetDefaultInstanceData
the default instance data of a new instance
is zero. this function sets a new default for
the creation of a new instance. the data in 
the recieved instance_data_default is cppied
into the class metadata.
********************************************/
void ClassSetDefaultInstanceData(class_t *this, void *instance_data_default);

/******************************************* 
                ClassGetMethod
returns a function pointer to a function from
the class v_table that matches the func_name given.

if no such method is found it returns NULL 
********************************************/
func_ptr ClassGetMethod(class_t *this, char *func_name);

/******************************************* 
                ClassGetName
returns a function pointer to a string containing
the class name 
********************************************/
char *ClassGetName(class_t *this);

/******************************************* 
                ClassAddNewStaticField
allocates a new static field with ther given size
and name. returns MAX_N_STATIC_FIELDS on failure, 
otherwise returns the index of the static field
in the static data
********************************************/
size_t ClassAddNewStaticField(class_t *this, char *field_name, size_t size);

/******************************************* 
                ClassGetStaticFieldByName
recieves a class and a name of a static field
returns a pointer to the static field data
or NULL if no such field exists.
********************************************/
void *ClassGetStaticFieldByName(class_t *this, char *field_name);

/******************************************* 
                ClassSetStaticFieldByName
recieves a class and a name of a static field
and a pointer to data.

the function copies the data inside of the 
recieved data to the static fields data
********************************************/
void ClassSetStaticFieldByName(class_t *this, char *field_name, void *data);

/******************************************* 
                ClassGetStaticFieldByName
recieves a class and a index of a static field
(the one that returned from ClassAddNewStaticField()) 

returns a pointer to the static field data
or NULL if no such field exists.
********************************************/
void *ClassGetStaticFieldByIndex(class_t *this, size_t index);

/******************************************* 
                ClassGetStaticFieldByName
recieves a class and a index of a static field
(the one that returned from ClassAddNewStaticField()) 

the function copies the data inside of the 
recieved data to the static fields data
********************************************/
void ClassSetStaticFieldByIndex(class_t *this, size_t index, void *data);

/******************************************* 
                ClassSetStaticInitBlock
sets a function as the static init block of a class

a static init block function recieves a class and
returns nothing.

it wiil be called with the class it was defined for
********************************************/
void ClassSetStaticInitBlock(class_t *this, static_init_func static_init_block);

/******************************************* 
                ClassSetInstanceInitBlock
sets a function as the instance init block of a class

a instance init block function recieves an object and
returns nothing.

it wiil be called from
ClassCreateNewInstance/ClassExtendNewInstance
with the new instance created
********************************************/
void ClassSetInstanceInitBlock(class_t *this, instance_init_func instance_init_block);

/******************************************* 
                ClassGetParent
returns the parent class of this.
********************************************/
class_t *ClassGetParent(class_t *this);

/******************************************* 
                ClassGetDefaultInstanceData
returns default instance data of this.
********************************************/
void *ClassGetDefaultInstanceData(class_t *this);

/******************************************* 
                ClassInstanceInitBlockExec
Executes the instacne init block of a class
if it exists.
********************************************/
void ClassInstanceInitBlockExec(class_t* this, object_t *new_object);

/******************************************* 
                ClassStaticInitBlockExec
Executes the static init block of a class parent
recursively if needed and then executes the 
static init block of the calling class
if it exists.
********************************************/
void ClassStaticInitBlockExec(class_t *this);

#endif /*__MY_CLASS_H__*/
