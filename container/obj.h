#ifndef UTILS_OBJ_H_
#define UTILS_OBJ_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

#define CreateObj(typeName) ((typeName*)malloc(sizeof(typeName)))
#define CreateMultipleObjFromCount(typeName, count) ((typeName*)malloc(sizeof(typeName) * (count)))
#define CreateMultipleObjFromByteCount(objByteCount, count) (malloc((objByteCount) * (count)))
#define DeleteObj(obj) free(obj)

#define MemoryCopy(dst, src, size) memcpy((void*)(dst), (void*)(src), (size))
#define MemorySet(dst, val, size) memset((void*)(dst), (val), (size))
#define MemoryCmp(buf1, buf2, size) memcmp((void*)(buf1), (void*)(buf2), (size))

#define GetFieldOffset(typeName, fieldName) ( (int)&(((typeName*)0)->fieldName) )
#define GetFieldFromHeadOffset(typeName, head, offset) ( (typeName*)((uintptr_t)(head) + (offset)) )
#define GetHeadFromFieldOffset(typeName, field, offset) ( (typeName*)((uintptr_t)(field) - (offset)) )
#define GetHeadFromField(field, typeName, fieldName) ( (typeName*)((uintptr_t)(field) - GetFieldOffset(typeName, fieldName)) )

#ifdef __cplusplus
}
#endif


#endif UTILS_OBJ_H_