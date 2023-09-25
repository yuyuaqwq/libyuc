/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/
#include <libyuc/basic.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_ALGORITHM_ARRAY_CLASS
#define LIBYUC_ALGORITHM_ARRAY_CLASS
#endif

#ifndef LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey
#define LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(main_obj, obj) (obj)
#endif

#ifndef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element int
#endif
#ifndef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element
#endif
#ifndef LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidId
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidId (-1)
#endif
#ifndef LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidDynamicArray
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidDynamicArray (NULL)
#endif
#ifndef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id size_t
#endif
#ifndef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset
#define LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id
#endif

#ifndef LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff
#define LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff ptrdiff_t
#endif
#ifndef LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp
#define LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(main_obj, obj1, obj2) ((LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff)(*(obj1) - *(obj2)))
#endif

#define ArrayFind MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayFind)
#define ArrayFind_Range MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayFind_Range)
#define ArrayInsert MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayInsert)
#define ArrayDelete MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayDelete)
#define ArrayOrderFind MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayOrderFind)
#define ArrayOrderFind_Range MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayOrderFind_Range)
#define ArrayOrderPut MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayOrderPut)
#define ArrayOrderDelete MAKE_NAME(LIBYUC_ALGORITHM_ARRAY_CLASS, ArrayOrderDelete)

LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayFind(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key);
LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayFind_Range(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key, LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff* cmp_diff);
void ArrayInsert(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id index, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* element);
void ArrayDelete(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id index);
LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayOrderFind(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key);
LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayOrderFind_Range(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key, LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff* cmp_diff);
void ArrayOrderPut(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* element);
bool ArrayOrderDelete(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key);



LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayFind(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key) {
    for (LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id i = begin; i <= end; i++) {
        if (LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(array, LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, &array[i]), key) == 0)
            return (LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id)i;
    }
    return LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidId;
}

/*
* 有序数组，但以顺序查找的形式进行范围查找
*/
LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayFind_Range(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key, LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff* cmp_diff) {
      assert(end > begin);
    if (end == begin) {
        *cmp_diff = LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(array, key, LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, &array[0]));
        return 0;
    }
    for (LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id i = begin; i <= end; i++) {
        *cmp_diff = LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(array, key, LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, &array[i]));
        if (*cmp_diff == 0) {
            return (LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id)i;
        }
        else if (*cmp_diff < 0) {
            return i;
        }
    }
    return count - 1;
}

void ArrayInsert(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id index, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* element) {
    for(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id i = count; i > index; i--) {
        array[i] = array[i - 1];
    }
    array[index] = *element;
}
void ArrayDelete(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id index) {
    for(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id i = index; i < count - 1; i++) {
        array[i] = array[i + 1];
    }
}

/* 有序数组相关 */
/* 基于BinarySearch的查找 */
/*
* 找到了返回正确下标，否则返回-1
*/
LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayOrderFind(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key) {
    LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id mid;
    while (begin <= end) {
        mid = begin + ((end - begin) >> 1);
        LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff cmp = LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(array, LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, &array[mid]), key);
        if (cmp < 0) {
            end = mid - 1;
        } else if (cmp > 0) {
            begin = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}
/*
* 找到了返回正确下标，否则返回第一个小于或大于key的元素下标
* (通常返回第一个大于等于key的元素下标，若所有元素都小于key，则返回最后一个元素的下标)
*/
LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id ArrayOrderFind_Range(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id begin, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id end, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key, LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff* cmp_diff) {
    if (begin == end) {
        *cmp_diff = LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(array, key, LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, &array[begin]));
    }
    else {
        LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id mid = 0;
        while (begin < end) {
            mid = begin + ((end - begin) >> 1);
            *cmp_diff = LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp(array, key, LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, &array[mid]));
            if (*cmp_diff > 0) begin = mid + 1;
            else end = mid;
        }
    }
    return begin;
}


void ArrayOrderPut(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* element) {
    LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff cmp_diff;
    LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id index = ArrayOrderFind_Range(array, 0, count - 1, (const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key*)LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey(array, element), &cmp_diff);
}
bool ArrayOrderDelete(LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element* array, LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset count, const LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key* key) {
    LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id id = ArrayOrderFind(array, 0, count - 1, key);
    if (id == LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidId) return false;
    ArrayDelete(array, count, id);
    return true;
}


#undef ArrayFind
#undef ArrayInsert
#undef ArrayDelete
#undef ArrayOrderFind
#undef ArrayOrderFind_Range
#undef ArrayOrderPut 
#undef ArrayOrderDelete 

#undef LIBYUC_ALGORITHM_ARRAY_CLASS
#undef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Key
#undef LIBYUC_ALGORITHM_ARRAY_ACCESSOR_GetKey
#undef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Element
#undef LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidId
#undef LIBYUC_ALGORITHM_ARRAY_INDEXER_Const_InvalidDynamicArray
#undef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Id
#undef LIBYUC_ALGORITHM_ARRAY_INDEXER_Type_Offset
#undef LIBYUC_ALGORITHM_ARRAY_COMPARER_Type_Diff
#undef LIBYUC_ALGORITHM_ARRAY_COMPARER_Cmp

#ifdef __cplusplus
}
#endif
