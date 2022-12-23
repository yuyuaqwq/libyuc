#ifndef UTILS_ARRAY_H_
#define UTILS_ARRAY_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Array {
	size_t size;
	size_t capacity;
	void* objArr;
	int objByteCount;
} Array;

void ArrayInit(Array* arr, size_t capacity, int objByteCount);

void ArrayRelease(Array* arr);

#define ArrayAt(arr, objName, index) ((objName*)(((uintptr_t)(arr)->objArr) + (arr)->objByteCount * (index)))

#define ArrayFindKeyM(arr, retObj, objName, keyFieldName, key) { \
	retObj = NULL; \
	for (int i = 0; i < (arr)->size; i++) { \
		objName* tempObj = ArrayAt((arr), objName, i); \
		if (tempObj->keyFieldName == (key)) { \
			retObj = tempObj; \
			break; \
		} \
	} \
}

void ArrayPushBack(Array* arr, void* obj);

void* ArrayPopBack(Array* arr);


#ifdef __cplusplus
}
#endif

#endif // UTILS_ARRAY_H_