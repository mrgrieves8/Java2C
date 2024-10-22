#ifndef __METHOD_H__
#define __METHOD_H__

#define CLASS_NAME_MAX_LEN 32
#define FUNC_NAME_MAX_LEN 32

typedef void (*func_ptr)(void);
typedef struct method method_t;
						
method_t *MethodCreate(func_ptr func, char *func_name, char *class_name);
method_t *MethodDuplicate(method_t *method);
void MethodDestroy(method_t *method);
int IsMethodMatchName(method_t *method, char *func_name);
void MethodSetFunc(method_t *method, func_ptr func);
void MethodSetClassName(method_t *method, char *class_name);
func_ptr GetFuncPointer(method_t *method);

#endif /*__METHOD_H__*/