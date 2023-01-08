#ifndef CUTILS_OBJECT_H_
#define CUTILS_OBJECT_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MemoryAlloc(size) malloc((size))
#define MemoryFree(ptr) free((ptr))
#define MemoryCopy(dst, src, size) memcpy((void*)(dst), (void*)(src), (size))
void MemoryCopyR(void* dst_, void* src_, size_t size);
#define MemorySet(dst, val, size) memset((void*)(dst), (val), (size))
int MemoryCmp(const void* buf1_, const void* buf2_, size_t size);
int MemoryCmpR(const void* buf1_, const void* buf2_, size_t size);
void MemorySwap(void* buf1_, void* buf2_, size_t size);


#define CreateObject(objName) ((objName*)MemoryAlloc(sizeof(objName)))
#define CreateMultipleObjByCount(objName, count) ((objName*)MemoryAlloc(sizeof(objName) * (count)))
#define CreateMultipleObjByByteCount(objByteCount, count) (MemoryAlloc((objByteCount) * (count)))
#define DeleteObject_(obj) (MemoryFree(obj))


#define GetFieldOffset(objName, fieldName) ( (int)&(((objName*)0)->fieldName) )
#define GetFieldSize(objName, fieldName) ( sizeof(((objName*)0)->fieldName) )
#define GetFieldByFieldOffset(obj, fieldOffset, objName) ( (objName*)((uintptr_t)(obj) + (fieldOffset)) )
#define GetObjByFieldOffset(field, fieldOffset, objName) ( (objName*)((uintptr_t)(field) - (fieldOffset)) )
#define GetObjByField(field, objName, fieldName) ( (objName*)((uintptr_t)(field) - GetFieldOffset(objName, fieldName)) )

// 自定义比较函数对int等可以直接比较的类型来说会有较大的开销，可以考虑优化成没有自定义比较函数就把指针当成值来比较
typedef int (*CmpFunc)(const void* buf1_, const void* buf2_, size_t size);
typedef uint32_t(*HashFunc)(const void* buf1_, size_t size);
#define KEY_STRING_SIZE (-1)

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