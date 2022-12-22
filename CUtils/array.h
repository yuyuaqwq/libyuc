#ifndef UTILS_ARRAY_H_
#define UTILS_ARRAY_H_

#include "CUtils/obj.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Array {
	size_t size;
	size_t capacity;
	void* objArr;
	int objByteCount;
} Array;

void ArrayInit(Array* arr, int typeSize, size_t capacity);

void ArrayRelease(Array* arr);

#define ArrayAt(arr, typeName, index) ((typeName*)(((uintptr_t)(arr)->objArr) + (arr)->objByteCount * (index)))

#define ArrayFindKey_Macro(arr, retEntry, typeName, keyField, key) { \
	retEntry = NULL; \
	for (int i = 0; i < (arr)->size; i++) { \
		typeName* temp = ArrayAt((arr), typeName, i); \
		if (temp->keyField == key) { \
			retEntry = temp; \
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