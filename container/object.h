#ifndef UTILS_OBJECT_H_
#define UTILS_OBJECT_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


#define CreateObject(objName) ((objName*)malloc(sizeof(objName)))
#define CreateMultipleObjFromCount(objName, count) ((objName*)malloc(sizeof(objName) * (count)))
#define CreateMultipleObjFromByteCount(objByteCount, count) (malloc((objByteCount) * (count)))
#define DeleteObject(obj) (free(obj))

#define MemoryCopy(dst, src, size) memcpy((void*)(dst), (void*)(src), (size))
#define MemorySet(dst, val, size) memset((void*)(dst), (val), (size))
#define MemoryCmp(buf1, buf2, size) memcmp((void*)(buf1), (void*)(buf2), (size))

#define GetFieldOffset(objName, fieldName) ( (int)&(((objName*)0)->fieldName) )
#define GetFieldFromObjOffset(objName, obj, offset) ( (objName*)((uintptr_t)(obj) + (offset)) )
#define GetObjFromFieldOffset(objName, field, offset) ( (objName*)((uintptr_t)(field) - (offset)) )
#define GetObjFromField(field, objName, fieldName) ( (objName*)((uintptr_t)(field) - GetFieldOffset(objName, fieldName)) )


#ifdef __cplusplus
}
#endif


#endif UTILS_OBJECT_H_