/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_ARRAY_H_
#define LIBYUC_ALGORITHM_ARRAY_H_

#include <libyuc/basic.h>


#ifdef __cplusplus
extern "C" {
#endif


#define LIBYUC_ALGORITHM_ARRAY_DEFINE(array_name, offset_type, id_type, element_type, key_type, accessor, comparer, referencer) \
    id_type array_name##ArrayFind(element_type* array, offset_type count, const key_type* key) { \
        for (offset_type i = 0; i < count; i++) { \
            if (comparer##_Equal(array, accessor##_GetKey(array, &array[i]), key)) return (id_type)i; \
        } \
        return referencer##_InvalidId; \
    } \
    void array_name##ArrayInsert(element_type* array, offset_type count, offset_type index, element_type* element) { \
        for(offset_type i = count; i > index; i--) { \
            array[i] = array[i - 1]; \
        } \
        array[index] = *element; \
    } \
    void array_name##ArrayDelete(element_type* array, offset_type count, offset_type index) { \
        for(offset_type i = index; i < count - 1; i++) { \
            array[i] = array[i + 1]; \
        } \
    }


#define LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT_GetKey(ARR, OBJ) (OBJ)
#define LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT

#define LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID_Equal(ARR, OBJ1, OBJ2) (false)
#define LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID LIBYUC_ALGORITHM_ARRAY_COMPARER_INVALID

#define LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT_InvalidId (-1)
#define LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT LIBYUC_ALGORITHM_ARRAY_REFERENCER_DEFALUT

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_INSERT_SORT_H_