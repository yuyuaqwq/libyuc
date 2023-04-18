/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_ALGORITHM_ARRAY_H_
#define CUTILS_ALGORITHM_ARRAY_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif


#define CUTILS_ALGORITHM_ARRAY_DEFINE(array_name, element_type, id_type) \
    void array_name##ArrayInsert(element_type* array, id_type count, id_type index, element_type element) { \
        for(id_type i = count; i > index; i--) { \
            array[i] = array[i - 1]; \
        } \
        array[index] = element; \
    } \
    void array_name##ArrayDelete(element_type* array, id_type count, id_type index) { \
        for(id_type i = index; i < count - 1; i++) { \
            array[i] = array[i + 1]; \
        } \
    } \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_INSERT_SORT_H_