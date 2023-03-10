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
#include <assert.h>

#ifdef NDEBUG
#define assert(expression) ((void)0)
#else
#undef assert
#define assert(expression) { if(!(expression)){ *(int*)0 = 0; } }
#endif

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


#define ObjectCreate(objName) ((objName*)MemoryAlloc(sizeof(objName)))
#define ObjectCreateArray(objName, count) ((objName*)MemoryAlloc(sizeof(objName) * (count)))
#define ObjectCreateArrayBySize(objSize, count) (MemoryAlloc((objSize) * (count)))
#define ObjectDelete(obj) (MemoryFree(obj))
#define ObjectArrayAt(arr, index, objSize) ((void*)((((uintptr_t)arr) + (objSize) * (index))))
#define ObjectSwap(objName, obj1, obj2) { objName temp = obj1; obj1 = obj2; obj2 = temp; }
#define ObjectGetFieldOffset(objName, fieldName) ( (int)&(((objName*)0)->fieldName) )
#define ObjectGetFieldSize(objName, fieldName) ( sizeof(((objName*)0)->fieldName) )
#define ObjectGetFieldByOffset(obj, fieldOffset, objName) ( (objName*)((uintptr_t)(obj) + (fieldOffset)) )
#define ObjectGetFromFieldOffset(field, fieldOffset, objName) ( (objName*)((uintptr_t)(field) - (fieldOffset)) )
#define ObjectGetFromField(field, objName, fieldName) ( (objName*)((uintptr_t)(field) - ObjectGetFieldOffset(objName, fieldName)) )


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