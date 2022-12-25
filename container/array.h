#ifndef CUTILS_ARRAY_H_
#define CUTILS_ARRAY_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Array {
	size_t count;
	size_t capacity;
	void* objArr;
	int objSize;
} Array;

void ArrayInit(Array* arr, size_t capacity, int objByteCount);

void ArrayRelease(Array* arr);

#define ArrayAt(arr, objName, index) ((objName*)(((uintptr_t)(arr)->objArr) + (arr)->objSize * (index)))

#define ArrayFindEntryByKeyM(arr, retObj, objName, keyFieldName, key) { \
	retObj = NULL; \
	for (int i = 0; i < (arr)->size; i++) { \
		objName* tempObj = ArrayAt((arr), objName, i); \
		if (tempObj->keyFieldName == (key)) { \
			retObj = tempObj; \
			break; \
		} \
	} \
}

void ArrayPushTail(Array* arr, void* obj);

void* ArrayPopTail(Array* arr);

void ArrayExpand(Array* arr, size_t targetSize);

void ArraySwapEntry(Array* arr, int index1, int index2);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ARRAY_H_