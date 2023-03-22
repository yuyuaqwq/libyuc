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
#define MemoryCopy(DST, SRC, SIZE) memcpy((void*)(DST), (void*)(SRC), (SIZE))
void MemoryCopyR(void* dst_, void* src_, size_t size);
#define MemorySet(DST, VAL, SIZE) memset((void*)(DST), (VAL), (SIZE))
ptrdiff_t MemoryCmp(const void* buf1_, const void* buf2_, size_t size);
ptrdiff_t MemoryCmpR(const void* buf1_, const void* buf2_, size_t size);
ptrdiff_t MemoryCmpR2(const void* buf1_, size_t size1, const void* buf2_, size_t size2);
void MemorySwap(void* buf1_, void* buf2_, size_t size);


#define ObjectCreate(OBJ_TYPE) ((OBJ_TYPE*)MemoryAlloc(sizeof(OBJ_TYPE)))
#define ObjectCreateMultiple(OBJ_TYPE, COUNT) ((OBJ_TYPE*)MemoryAlloc(sizeof(OBJ_TYPE) * (COUNT)))
#define ObjectCreateMultipleBySize(OBJ_SIZE, COUNT) (MemoryAlloc((OBJ_SIZE) * (COUNT)))
#define ObjectDelete(OBJ) (MemoryFree(OBJ))
#define ObjectMultipleAt(ARR, INDEX, OBJ_SIZE) ((void*)((((uintptr_t)ARR) + (OBJ_SIZE) * (INDEX))))
#define ObjectSwap(OBJ_TYPE, OBJ1, OBJ2) { OBJ_TYPE temp = OBJ1; OBJ1 = OBJ2; OBJ2 = temp; }
#define ObjectGetFieldOffset(OBJ_TYPE, FIELD_NAME) ( (int)&(((OBJ_TYPE*)0)->FIELD_NAME) )
#define ObjectGetFieldSize(OBJ_TYPE, FIELD_NAME) ( sizeof(((OBJ_TYPE*)0)->FIELD_NAME) )
#define ObjectGetFieldByOffset(OBJ, FIELD_OFFSET, OBJ_TYPE) ( (OBJ_TYPE*)((uintptr_t)(OBJ) + (FIELD_OFFSET)) )
#define ObjectGetFromFieldOffset(OBJ_FIELD, FIELD_OFFSET, OBJ_TYPE) ( (OBJ_TYPE*)((uintptr_t)(OBJ_FIELD) - (FIELD_OFFSET)) )
#define ObjectGetFromField(OBJ_FIELD, OBJ_TYPE, FIELD_NAME) ( (OBJ_TYPE*)((uintptr_t)(OBJ_FIELD) - ObjectGetFieldOffset(OBJ_TYPE, FIELD_NAME)) )



/*
* 对象：
* 对象操作器间传递(参数、返回值)的对象以对象本身进行
* 外部函数间传递的对象以指针进行(参考C++引用)
* 
* 主对象：
* 对象进行某些操作时需要的关联对象，如引用器，若通过哈希表对id进行引用，仅id自然无法查找到对应的引用关系
*/

/*
* 默认分配器
* 负责管理对象的分配与释放，与引用器结合时返回id
*/
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT_Create(MAIN_OBJ, OBJ_TYPE) ObjectCreate(OBJ_TYPE) 
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT_CreateMultiple(MAIN_OBJ, OBJ_TYPE, COUNT) ObjectCreateMultiple(OBJ_TYPE, COUNT) 
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT_Delete(MAIN_OBJ, OBJ) ObjectDelete(OBJ) 
#define CUTILS_OBJECT_ALLOCATOR_DEFALUT CUTILS_OBJECT_ALLOCATOR_DEFALUT		// 同名以支持嵌套

/*
* 默认比较器
* 负责对象与对象的大小比较，相当于重载 < > ==
*/
#define CUTILS_OBJECT_COMPARER_DEFALUT_Equal(MAIN_OBJ, OBJ1, OBJ2) ((OBJ1) == (OBJ2))
#define CUTILS_OBJECT_COMPARER_DEFALUT_Greater(MAIN_OBJ, OBJ1, OBJ2) ((OBJ1) > (OBJ2))
#define CUTILS_OBJECT_COMPARER_DEFALUT_Less(MAIN_OBJ, OBJ1, OBJ2) ((OBJ1) < (OBJ2))
#define CUTILS_OBJECT_COMPARER_DEFALUT_Subrrac(MAIN_OBJ, OBJ1, OBJ2) ((OBJ1) - (OBJ2))
#define CUTILS_OBJECT_COMPARER_DEFALUT CUTILS_OBJECT_COMPARER_DEFALUT

/*
* 默认字段访问器
* 负责对象指定字段的访问，相当于重载 ->
*/
#define CUTILS_OBJECT_FIELD_ACCESSOR_DEFALUT_GetTest(MAIN_OBJ, OBJ_PTR) ((OBJ_PTR)->test)
#define CUTILS_OBJECT_FIELD_ACCESSOR_DEFALUT_SetTest(MAIN_OBJ, OBJ_PTR, NEW_TEST) ((OBJ_PTR)->test = (NEW_TEST))
#define CUTILS_OBJECT_FIELD_ACCESSOR_DEFALUT CUTILS_OBJECT_FIELD_ACCESSOR_DEFALUT

/*
* 默认索引器
* 负责数组对象的元素访问，相当于重载[]
*/
#define CUTILS_OBJECT_INDEXER_DEFALUT(MAIN_OBJ, OBJ_ARR, INDEX) ((OBJ_ARR)[INDEX])

/*
* 默认引用器
* 负责引用对象，将id转换成对象，通常是映射关系，默认id即对象的指针
*/
#define CUTILS_OBJECT_REFERENCER_DEFALUT_Reference(MAIN_OBJ, OLD_ID) (OLD_ID)
#define CUTILS_OBJECT_REFERENCER_DEFALUT_Dereference(MAIN_OBJ, OBJ)
#define CUTILS_OBJECT_REFERENCER_DEFALUT_InvalidId (NULL)
#define CUTILS_OBJECT_REFERENCER_DEFALUT CUTILS_OBJECT_REFERENCER_DEFALUT

/*
* 默认哈希器
* 负责计算对象的哈希值
*/
#define CUTILS_OBJECT_HASHER_DEFALUT(MAIN_OBJ, OBJ) 0

/*
* 默认传输器
* 负责对象之间的传输、拷贝等操作，相当于重载 =
*/
#define CUTILS_OBJECT_MOVER_DEFALUT_Assignment(MAIN_OBJ, OBJ1, OBJ2) ((OBJ1) = (OBJ2))
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