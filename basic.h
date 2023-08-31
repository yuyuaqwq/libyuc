/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_BASIC_H_
#define LIBYUC_BASIC_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef Painc
#define Painc(...) { printf(__VA_ARGS__); __debugbreak();  /* *(int*)0 = 0;*/ }
#endif

#ifdef NDEBUG
#define assert(expression) ((void)0)
#else
#undef assert
#define assert(expression) { if(!(expression)){ Painc("") } }
#endif

#define release_assert(expression, ...) { if(!(expression)){ Painc(__VA_ARGS__); } }


#ifdef _MSC_VER // for MSVC
#define forceinline __forceinline
#elif defined __GNUC__ // for gcc on Linux/Apple OS X
#define forceinline __inline__ __attribute__((always_inline))
#else
#define forceinline
#endif



forceinline static void MemorySwap(void* buf1_, void* buf2_, size_t size) {
    uint8_t* buf1 = (uint8_t*)buf1_;
    uint8_t* buf2 = (uint8_t*)buf2_;
    for (size_t i = 0; i < size; i++) {
        uint8_t temp = buf1[i];
        buf1[i] = buf2[i];
        buf2[i] = temp;
    }
}

forceinline static void MemoryCopy(void* dst_, const void* src_, size_t size) {
    memcpy(dst_, src_, size);
}

forceinline static void MemoryCopyR(void* dst_, const void* src_, size_t size) {
    uint8_t* dst = (uint8_t*)dst_;
    uint8_t* src = (uint8_t*)src_;
    while (size-- != 0) {
        dst[size] = src[size];
    }
}

forceinline static ptrdiff_t MemoryCmp(const void* buf1_, const void* buf2_, size_t size) {
    return memcmp(buf1_, buf2_, size);
}

forceinline static ptrdiff_t MemoryCmpR(const void* buf1_, const void* buf2_, size_t size) {
    uint8_t* buf1 = (uint8_t*)buf1_;
    uint8_t* buf2 = (uint8_t*)buf2_;
    while (size-- != 0) {
        int res = buf1[size] - buf2[size];
        if (res != 0) {
            return res;
        }
    }
    return 0;
}

forceinline static ptrdiff_t MemoryCmpR2(const void* buf1_, size_t size1, const void* buf2_, size_t size2) {
    if (size1 != size2) {
        return size1 - size2;
    }
    uint8_t* buf1 = (uint8_t*)buf1_;
    uint8_t* buf2 = (uint8_t*)buf2_;
    while (size1-- != 0) {
        int res = buf1[size1] - buf2[size1];
        if (res != 0) {
            return res;
        }
    }
    return 0;
}

forceinline static void* MemoryAlloc(size_t size) {
    return malloc(size);
}

forceinline static void MemoryFree(void* ptr) {
    return free(ptr);
}



#define ObjectCreate(OBJ_TYPE) ((OBJ_TYPE*)MemoryAlloc(sizeof(OBJ_TYPE)))
#define ObjectCreateMultiple(OBJ_TYPE, COUNT) ((OBJ_TYPE*)MemoryAlloc(sizeof(OBJ_TYPE) * (COUNT)))
#define ObjectCreateMultipleBySize(OBJ_SIZE, COUNT) (MemoryAlloc((OBJ_SIZE) * (COUNT)))
#define ObjectRelease(OBJ) (MemoryFree(OBJ))
#define ObjectMultipleAt(ARR, INDEX, OBJ_SIZE) ((void*)((((uintptr_t)ARR) + (OBJ_SIZE) * (INDEX))))
#define ObjectSwap(OBJ_TYPE, OBJ1, OBJ2) { OBJ_TYPE temp = OBJ1; OBJ1 = OBJ2; OBJ2 = temp; }
#define ObjectGetFieldOffset(OBJ_TYPE, FIELD_NAME) ( (uintptr_t)&(((OBJ_TYPE*)0)->FIELD_NAME) )
#define ObjectGetFieldSize(OBJ_TYPE, FIELD_NAME) ( sizeof(((OBJ_TYPE*)0)->FIELD_NAME) )
#define ObjectGetFieldByOffset(OBJ, FIELD_OFFSET, OBJ_TYPE) ( (OBJ_TYPE*)((uintptr_t)(OBJ) + (FIELD_OFFSET)) )
#define ObjectGetFromFieldOffset(OBJ_FIELD, FIELD_OFFSET, OBJ_TYPE) ( (OBJ_TYPE*)((uintptr_t)(OBJ_FIELD) - (FIELD_OFFSET)) )
#define ObjectGetFromField(OBJ_FIELD, OBJ_TYPE, FIELD_NAME) ( (OBJ_TYPE*)((uintptr_t)(OBJ_FIELD) - ObjectGetFieldOffset(OBJ_TYPE, FIELD_NAME)) )

/*
* C另一种实现泛型的方法：基于#include
* 通过#define指定参数(类型等)
* 再通过#include加载模板，头文件中使用宏代替被替换的类型
* 好处是方便调试
*/


/*
* 对象适配器

* 设计原则：
* 对象传递统一使用指针
* 
* 主对象：
* 对象进行某些操作时需要的关联对象，如引用器，若通过哈希表对id进行引用，仅id自然无法查找到对应的引用关系
* 通常传递的是指针
*/

/*
* id_type
* 表示引用的id，通常可以直接传值
*/

/*
* offset_type
* 表示在对象集中对对象引用的表示，可能是count，可能是index
*/


/*
* 默认分配器
* 负责管理对象的分配与释放，与引用器结合时返回id
*/
#define LIBYUC_BASIC_ALLOCATOR_DEFALUT_Create(MAIN_OBJ, OBJ_TYPE) ObjectCreate(OBJ_TYPE)
#define LIBYUC_BASIC_ALLOCATOR_DEFALUT_CreateBySize(MAIN_OBJ, OBJ_SIZE) ObjectCreateMultipleBySize(OBJ_SIZE, 1)
#define LIBYUC_BASIC_ALLOCATOR_DEFALUT_CreateMultiple(MAIN_OBJ, OBJ_TYPE, COUNT) ObjectCreateMultiple(OBJ_TYPE, COUNT) 
#define LIBYUC_BASIC_ALLOCATOR_DEFALUT_Release(MAIN_OBJ, OBJ) ObjectRelease(OBJ) 
#define LIBYUC_BASIC_ALLOCATOR_DEFALUT LIBYUC_BASIC_ALLOCATOR_DEFALUT        // 同名以支持嵌套

/*
* 默认比较器
* 负责对象与对象的大小比较，相当于重载 < > ==
*/
#define LIBYUC_BASIC_COMPARER_DEFALUT_Equal(MAIN_OBJ, OBJ1, OBJ2) (*(OBJ1) == *(OBJ2))
#define LIBYUC_BASIC_COMPARER_DEFALUT_Greater(MAIN_OBJ, OBJ1, OBJ2) (*(OBJ1) > *(OBJ2))
#define LIBYUC_BASIC_COMPARER_DEFALUT_Less(MAIN_OBJ, OBJ1, OBJ2) (*(OBJ1) < *(OBJ2))
#define LIBYUC_BASIC_COMPARER_DEFALUT_Subrrac(MAIN_OBJ, OBJ1, OBJ2) (*(OBJ1) - *(OBJ2))
#define LIBYUC_BASIC_COMPARER_DEFALUT LIBYUC_BASIC_COMPARER_DEFALUT

/*
* 默认字段访问器
* 负责对象指定字段的访问，相当于重载 ->
*/
#define LIBYUC_BASIC_FIELD_ACCESSOR_DEFALUT_GetTest(MAIN_OBJ, OBJ_PTR) (&(OBJ_PTR)->test)
#define LIBYUC_BASIC_FIELD_ACCESSOR_DEFALUT_SetTest(MAIN_OBJ, OBJ_PTR, NEW_TEST) ((OBJ_PTR)->test = *(NEW_TEST))
#define LIBYUC_BASIC_FIELD_ACCESSOR_DEFALUT LIBYUC_BASIC_FIELD_ACCESSOR_DEFALUT

/*
* 默认索引器
* 负责数组对象的元素访问，相当于重载[]
*/
#define LIBYUC_BASIC_INDEXER_DEFALUT_Get(MAIN_OBJ, OBJ_ARR, INDEX) ((*(OBJ_ARR))[INDEX])
#define LIBYUC_BASIC_INDEXER_DEFALUT_Set(MAIN_OBJ, OBJ_ARR, INDEX, NEW_VALUE) ((*(OBJ_ARR))[INDEX] = NEW_VALUE)
#define LIBYUC_BASIC_INDEXER_DEFALUT_GetPtr(MAIN_OBJ, OBJ_ARR, INDEX) (&(*(OBJ_ARR))[INDEX])
#define LIBYUC_BASIC_INDEXER_DEFALUT LIBYUC_BASIC_INDEXER_DEFALUT

/*
* 默认引用器
* 负责引用对象，将id转换成对象，通常是映射关系，默认id即对象的指针
* Reference和Dereference不需要检查id/obj是否有效(通过断言避免意外问题)
*/
#define LIBYUC_BASIC_REFERENCER_DEFALUT_Reference(MAIN_OBJ, OLD_ID) (OLD_ID)
#define LIBYUC_BASIC_REFERENCER_DEFALUT_Dereference(MAIN_OBJ, OBJ)
#define LIBYUC_BASIC_REFERENCER_DEFALUT_InvalidId (NULL)
#define LIBYUC_BASIC_REFERENCER_DEFALUT LIBYUC_BASIC_REFERENCER_DEFALUT

/*
* 默认哈希器
* 负责计算对象的哈希值
*/
#define LIBYUC_BASIC_HASHER_DEFALUT(MAIN_OBJ, OBJ) (0)

/*
* 默认传输器
* 负责对象之间的拷贝、移动等操作，相当于重载 =
*/
#define LIBYUC_BASIC_MOVER_DEFALUT_Copy(MAIN_OBJ, OBJ1, OBJ2) (*(OBJ1) = *(OBJ2))
#define LIBYUC_BASIC_MOVER_DEFALUT_Move(MAIN_OBJ, OBJ1, OBJ2) (*(OBJ1) = *(OBJ2))
#define LIBYUC_BASIC_MOVER_DEFALUT LIBYUC_BASIC_MOVER_DEFALUT

/*
* 默认回调器
* 在某个时刻回调某个方法
*/
#define LIBYUC_BASIC_CALLBAKCER_DEFALUT

/* 
* 迭代器
* 容器的迭代处理
* 具体说明参考iterator.h
*/
#define LIBYUC_BASIC_ITERATOR_DEFALUT


/*
* 继承示例
#define NEW_REFERENCER_InvalidId LIBYUC_BASIC_REFERENCER_DEFALUT_InvalidId
#define NEW_REFERENCER_Reference(MAIN_OBJ, OLD_ID) LIBYUC_BASIC_REFERENCER_DEFALUT_Reference(MAIN_OBJ, OLD_ID)
#define NEW_REFERENCER_Dereference(MAIN_OBJ, OLD_ID) LIBYUC_BASIC_REFERENCER_DEFALUT_Dereference(MAIN_OBJ, OLD_ID)
#define NEW_REFERENCER LIBYUC_BASIC_REFERENCER_DEFALUT
*/




#ifdef __cplusplus
}
#endif


#endif LIBYUC_BASIC_H_