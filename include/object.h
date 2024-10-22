#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef struct object object_t;

#include "my_class.h"


/**************************************************************************************** 
------------------------------------instance methods-------------------------------------
****************************************************************************************/

/******************************************* 
                ObjectIsEquals
name: "equals_obj_obj"
an instance method for objects, takes two 
object_t pointers and returns 1 if they're 
equal and 0 othewrwise.
********************************************/
int ObjectIsEquals(object_t *this, object_t *other);

/******************************************* 
                ObjectHashCode
name: "hashCode_obj"
a instance method for objects, takes a 
object_t pointer and returns an int hashcode 
for it.
********************************************/
int ObjectHashCode(object_t *this);

/******************************************* 
                ObjectGetClass
name: "getClass_obj"
a instance method for objects, takes a 
object_t pointer and returns the class it
was created by.
********************************************/
class_t *ObjectGetClass(object_t *this);

/******************************************* 
                ObjectToString
name: "toString_obj_str"
writes a string representation of the object
to a buffer
********************************************/
void ObjectToString(object_t *this, char *buffer);

/******************************************* 
                Finalize
name: "finalize_obj"
cleans up object
********************************************/
void ObjectFinalize(object_t *this);

/**************************************************************************************** 
------------------------------------API functions-------------------------------------
****************************************************************************************/

/******************************************* 
                GetObjectClass
returns the Object class. can be used to 
create an instance of Object or as a
parent class when creating a new class
********************************************/
class_t *GetObjectClass(void);

/******************************************* 
                ObjectAllocate
allocates memory on the heap for a new 
instance of a certain class, and storing the class data in it.
********************************************/
object_t *ObjectAllocate(class_t *class_data);

/******************************************* 
                ObjectDestroy
deallocates memory on the heap for an instance.
********************************************/
void ObjectDestroy(object_t *this);

/******************************************* 
                ObjectGetInstanceData
returns a pointer to the instace data, at the
correct offset of the requested class.

for an instance of Class B that extends A,
the instance data will look like:

-------------------------------------------------------------
|           |           |           |           |           |
| A field1  | A field2  | A field3  | B field1  | B field2  |
|           |           |           |           |           |
-------------------------------------------------------------

calling: ObjectGetInstanceData(instance, B)
will return a pointer to the start of B's instance data.

                        <returns a pointer to this address>
                                    |
                                    |
------------------------------------V------------------------
|           |           |           |           |           |
| A field1  | A field2  | A field3  | B field1  | B field2  |
|           |           |           |           |           |
-------------------------------------------------------------

calling: ObjectGetInstanceData(instance, A)
will return a pointer to the start of A's instance data.

<returns a pointer to this address>                        
|                                   
|                                    
V------------------------------------------------------------
|           |           |           |           |           |
| A field1  | A field2  | A field3  | B field1  | B field2  |
|           |           |           |           |           |
-------------------------------------------------------------
********************************************/
void *ObjectGetInstanceData(object_t *this, class_t *class);

/******************************************* 
                ObjectSetInstanceData
copies the given instance_data to the 
instance_data of the object, at the
correct offset of the requested class.

for a instance of class C that extends B that extends A
(let's assume A has 2 fields, B has 1 field and C has 2 fields)
the instance data will look like:

-------------------------------------------------------------
|           |           |           |           |           |
| A field1  | A field2  | B field1  | C field1  | C field2  |
|           |           |           |           |           |
-------------------------------------------------------------

calling ObjectSetInstanceData(instance, &some_data, C);
will copy the some data into the C class fields.
now the instance data will look like:

------------------------------------------------------------------------------
|           |           |           |                   |                    |
| A field1  | A field2  | B field1  | some_data field1  | some_data field2   |
|           |           |           |                   |                    |
------------------------------------------------------------------------------

to use it safely:
    1. get current instance data into a local var (let's call it data)
            data = *(instance_data_type *)ObjectGetInstanceData(instance, relevant_class);
    2. change the relevant fields you want to set:
            data.x = 14;
    3. set the data into the instance:
            ObjectSetInstanceData(instance, &data, relevant_class);
********************************************/
void ObjectSetInstanceData(object_t *this, void *instance_data, class_t *class);

/******************************************* 
                ClassGetMethod
returns a function pointer to a function from
the object's class v_table that matches the func_name given.

if no such method is found it returns NULL 
********************************************/
func_ptr ObjectGetMethod(object_t *this, char *func_name);

#endif /*__OBJECT_H__*/
