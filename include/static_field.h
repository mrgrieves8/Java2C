#ifndef __STATIC_FIELD_H__
#define __STATIC_FIELD_H__

#include <stddef.h>	/* size_t */

#define MAX_FIELD_NAME_LEN 60


typedef struct static_field static_field_t;

static_field_t *StaticFieldCreate(size_t size, char *name);
void StaticFieldDestroy(static_field_t *this);
void *StaticFieldGetData(static_field_t *this);
void StaticFieldSetData(static_field_t *this, void *data);
int IsStaticFieldMatchStr(static_field_t *this, char *str);

#endif /*__STATIC_FIELD_H__*/