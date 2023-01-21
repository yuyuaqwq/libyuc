/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_VECTOR_H_
#define CUTILS_CONTAINER_VECTOR_H_

#include "CUtils/object.h"
#include "CUtils/container/array.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Vector {
    Array array;
} Vector;

void VectorInit(Vector* vector, size_t capacity);

void VectorRelease(Vector* vector, bool deleteObj);

#define VectorAt(vector, index, objName) (ArrayAt(&(vector)->array, index, objName*))

#define VectorFindEntryByField(vector, retObj, objName, keyFieldName, key) { \
    retObj = NULL; \
    for (int i = 0; i < (vector)->size; i++) { \
        objName* tempObj = VectorAt((vector), objName, i); \
        if (tempObj->keyFieldName == (key)) { \
            retObj = tempObj; \
            break; \
        } \
    } \
}

int VectorPushTail(Vector* vector, void* obj);

void* VectorPopTail(Vector* vector);

size_t VectorGetCount(Vector* vector);

void VectorSetCount(Vector* vector, size_t count);

size_t VectorGetCapacity(Vector* vector);

void VectorSetCapacity(Vector* vector, size_t capacity);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_VECTOR_H_