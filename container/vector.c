/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/vector.h>


void VectorExpand(Vector* vector, size_t addCount) {
    ArrayExpand(&vector->array, addCount);
}

void VectorInit(Vector* vector, size_t capacity) {
    ArrayInit(&vector->array, capacity, sizeof(void*));
}

void VectorRelease(Vector* vector, bool deleteObj) {
    if (vector->array.objArr && deleteObj) {
        for (int i = 0; i < vector->array.count; i++) {
            DeleteObject_(((void**)vector->array.objArr)[i]);
        }
    }
    ArrayRelease(&vector->array);
}

int VectorPushTail(Vector* vector, void* obj) {
    return ArrayPushTail(&vector->array, &obj);
}

void* VectorPopTail(Vector* vector) {
    return *(void**)ArrayPopTail(&vector->array);
}

size_t VectorGetCount(Vector* vector) {
    return ArrayGetCount(&vector->array);
}

void VectorSetCount(Vector* vector, size_t count) {
    ArraySetCount(&vector->array, count);
}

size_t VectorGetCapacity(Vector* vector) {
    return ArrayGetCapacity(&vector->array);
}

void VectorSetCapacity(Vector* vector, size_t capacity) {
    ArraySetCapacity(&vector->array, capacity);
}