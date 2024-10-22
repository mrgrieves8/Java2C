#ifndef __VTABLE_H__
#define __VTABLE_H__

#include "method.h"


typedef struct v_table v_table_t;

v_table_t *VtableCreate(void);
v_table_t *VtableDuplicate(v_table_t *parent_table);
void VtableDestroy(v_table_t *table);
int VtableAddMethod(v_table_t *table, func_ptr func, char *func_name, char *class_name);
method_t *VtableFindMethod(v_table_t *table, char *func_name);

#endif /* __VTABLE_H__ */


