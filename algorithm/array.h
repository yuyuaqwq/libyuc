/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_ALGORITHM_ARRAY_H_
#define LIBYUC_ALGORITHM_ARRAY_H_

#include <libyuc/basic.h>


#ifdef __cplusplus
extern "C" {
#endif


#define LIBYUC_ALGORITHM_ARRAY_DEFINE(array_name, element_type, id_type, referencer, comparer) \
    id_type array_name##ArrayFind(element_type* array, id_type count, element_type* element) { \
        for(id_type i = 0; i < count; i++) { \
            if (comparer##_Equal(array, &array[i], element)) return i; \
        } \
        return referencer##_InvalidId; \
    } \
    void array_name##ArrayInsert(element_type* array, id_type count, id_type index, element_type* element) { \
        for(id_type i = count; i > index; i--) { \
            array[i] = array[i - 1]; \
        } \
        array[index] = *element; \
    } \
    void array_name##ArrayDelete(element_type* array, id_type count, id_type index) { \
        for(id_type i = index; i < count - 1; i++) { \
            array[i] = array[i + 1]; \
        } \
    } \

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_ALGORITHM_INSERT_SORT_H_