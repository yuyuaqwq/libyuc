/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_OBJECT_H_
#define CUTILS_OBJECT_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

void* MemoryAlloc(size_t size);
void MemoryFree(void* ptr);
#define MemoryCopy(dst, src, size) memcpy((void*)(dst), (void*)(src), (size))
void MemoryCopyR(void* dst_, void* src_, size_t size);
#define MemorySet(dst, val, size) memset((void*)(dst), (val), (size))
ptrdiff_t MemoryCmp(const void* buf1_, const void* buf2_, size_t size);
ptrdiff_t MemoryCmpR(const void* buf1_, const void* buf2_, size_t size);
ptrdiff_t MemoryCmpR2(const void* buf1_, size_t size1, const void* buf2_, size_t size2);
void MemorySwap(void* buf1_, void* buf2_, size_t size);


#define CreateObject(objName) ((objName*)MemoryAlloc(sizeof(objName)))
#define CreateObjArr(objName, count) ((objName*)MemoryAlloc(sizeof(objName) * (count)))
#define CreateObjArrByObjSize(objSize, count) (MemoryAlloc((objSize) * (count)))
#define DeleteObject_(obj) (MemoryFree(obj))
#define ObjArrAt(arr, index, objSize) ((void*)((((uintptr_t)arr) + (objSize) * (index))))

#define GetFieldOffset(objName, fieldName) ( (int)&(((objName*)0)->fieldName) )
#define GetFieldSize(objName, fieldName) ( sizeof(((objName*)0)->fieldName) )
#define GetFieldByFieldOffset(obj, fieldOffset, objName) ( (objName*)((uintptr_t)(obj) + (fieldOffset)) )
#define GetObjByFieldOffset(field, fieldOffset, objName) ( (objName*)((uintptr_t)(field) - (fieldOffset)) )
#define GetObjByField(field, objName, fieldName) ( (objName*)((uintptr_t)(field) - GetFieldOffset(objName, fieldName)) )


typedef void* (*MemAllocFunc)(size_t size);
typedef void (*MemFreeFunc)(void* ptr);
typedef struct _MemAllocTor {
	MemAllocFunc Alloc;
	MemFreeFunc Free;
} MemAllocTor;

typedef int (*CmpFunc)(const void* buf1, const void* buf2, size_t size);
typedef int (*CmpFunc2)(const void* buf1, size_t size1, const void* buf2, size_t size2);
typedef struct _CmpTor {
	union {
		CmpFunc cmp;
		CmpFunc2 cmp2;
	};
} CmpTor;

typedef uint32_t(*HashU32Func)(const void* buf, size_t size);
typedef struct _HashTor {
	HashU32Func HashU32;
} HashTor;

typedef void* (*RandomAtFunc)(void* arr, int i);
typedef struct _RandomAtTor {
	RandomAtFunc ArrAt;
} RandomAtTor;

#ifdef _MSC_VER // for MSVC
#define forceinline __forceinline
#elif defined __GNUC__ // for gcc on Linux/Apple OS X
#define forceinline __inline__ __attribute__((always_inline))
#else
#define forceinline
#endif

#ifdef __cplusplus
}
#endif


#endif CUTILS_OBJECT_H_