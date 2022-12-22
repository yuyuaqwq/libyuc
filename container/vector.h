#ifndef UTILS_VECTOR_H_
#define UTILS_VECTOR_H_

#include "CUtils/container/obj.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Vector {
	size_t size;
	size_t capacity;
	void** objPtrArr;
} Vector;

void VectorInit(Vector* arr, size_t capacity);

void VectorRelease(Vector* arr);

#define VectorAt(vector, objName, index) (*(objName**)(((uintptr_t)(vector)->objPtrArr) + (sizeof(void*)) * (index)))

#define VectorFindKeyM(vector, retObj, objName, keyFieldName, key) { \
	retObj = NULL; \
	for (int i = 0; i < (vector)->size; i++) { \
		objName* tempObj = VectorAt((vector), objName, i); \
		if (tempObj->keyFieldName == (key)) { \
			retObj = tempObj; \
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