#ifndef UTILS_VECTOR_H_
#define UTILS_VECTOR_H_

#include "CUtils/obj.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Vector {
	size_t size;
	size_t capacity;
	void** objPtrArr;
} Vector;

void VectorInit(Vector* arr, int typeSize, size_t capacity);

void VectorRelease(Vector* arr);

#define VectorAt(vector, typeName, index) (*(typeName**)(((uintptr_t)(vector)->objPtrArr) + (sizeof(void*)) * (index)))

#define VectorFindKey_Macro(vector, retEntry, typeName, keyField, key) { \
	retEntry = NULL; \
	for (int i = 0; i < (vector)->size; i++) { \
		typeName* temp = VectorAt((vector), typeName, i); \
		if (temp->keyField == key) { \
			retEntry = temp; \
			break; \
		} \
	} \
}

void VectorPushBack(Vector* vector, void* obj);

void* VectorPopBack(Vector* vector);


#ifdef __cplusplus
}
#endif

#endif // UTILS_VECTOR_H_