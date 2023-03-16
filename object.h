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


#define ObjectCreate(obj_type) ((obj_type*)MemoryAlloc(sizeof(obj_type)))
#define ObjectCreateMultiple(obj_type, count) ((obj_type*)MemoryAlloc(sizeof(obj_type) * (count)))
#define ObjectCreateMultipleBySize(obj_size, count) (MemoryAlloc((obj_size) * (count)))
#define ObjectDelete(obj) (MemoryFree(obj))
#define ObjectMultipleAt(arr, index, objSize) ((void*)((((uintptr_t)arr) + (objSize) * (index))))
#define ObjectSwap(obj_type, obj1, obj2) { obj_type temp = obj1; obj1 = obj2; obj2 = temp; }
#define ObjectGetFieldOffset(obj_type, field_name) ( (int)&(((obj_type*)0)->field_name) )
#define ObjectGetFieldSize(obj_type, field_name) ( sizeof(((obj_type*)0)->field_name) )
#define ObjectGetFieldByOffset(obj, field_offset, obj_type) ( (obj_type*)((uintptr_t)(obj) + (field_offset)) )
#define ObjectGetFromFieldOffset(field, field_offset, obj_type) ( (obj_type*)((uintptr_t)(field) - (field_offset)) )
#define ObjectGetFromField(field, obj_type, field_name) ( (obj_type*)((uintptr_t)(field) - ObjectGetFieldOffset(obj_type, field_name)) )


/*
* 对象：接口皆为指针，优化交给编译器
*/

/*
* 默认分配器
* 负责对象的分配与释放
*/
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT_Create(obj_type) ObjectCreate(obj_type) 
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT_CreateMultiple(obj_type, count) ObjectCreateMultiple(obj_type, count) 
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT_Delete(obj) ObjectDelete(obj) 
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT CUTILS_OBJECT_ALLOCATOR_DEFALUT		// 同名以支持嵌套

/*
* 默认比较器
* 负责对象与对象的大小比较
*/
#define CUTILS_OBJECT_COMPARER_DEFALUT_Equal(obj1, obj2) (*(obj1) == *(obj2))
#define CUTILS_OBJECT_COMPARER_DEFALUT_Greater(obj1, obj2) (*(obj1) > *(obj2))
#define CUTILS_OBJECT_COMPARER_DEFALUT_Less(obj1, obj2) (*(obj1) < *(obj2))
#define CUTILS_OBJECT_COMPARER_DEFALUT CUTILS_OBJECT_COMPARER_DEFALUT

/*
* 默认字段访问器
* 负责对象指定字段的访问
*/
#define CUTILS_OBJECT_FIELD_ACCESSOR_DEFALUT(obj, field_name) (&(obj)->field_name)

/*
* 默认引用器
* 负责引用对象，将id转换成对象，通常是映射关系，默认id即对象的指针
*/
#define CUTILS_OBJECT_REFERENCER_DEFALUT_Reference(main_obj, obj_id) (obj_id)
#define CUTILS_OBJECT_REFERENCER_DEFALUT_Dereference(main_obj, obj) (0)
#define CUTILS_OBJECT_REFERENCER_DEFALUT_InvalidId (NULL)
#define CUTILS_OBJECT_REFERENCER_DEFALUT CUTILS_OBJECT_REFERENCER_DEFALUT

/*
* 默认哈希器
* 负责计算对象的哈希值
*/
#define CUTILS_OBJECT_HASHER_DEFALUT(key) (0)

/*
* 默认传输器
* 负责对象之间的传输、拷贝等操作
*/
#define CUTILS_OBJECT_MOVER_DEFALUT_Assignment(obj1, obj2) (*(obj1) = *(obj2))
#define CUTILS_OBJECT_MOVER_DEFALUT CUTILS_OBJECT_MOVER_DEFALUT


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