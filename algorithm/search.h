/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_ALGORITHM_SEARCH_H_
#define CUTILS_ALGORITHM_SEARCH_H_

#include "CUtils/object.h"

#ifdef __cplusplus
extern "C" {
#endif

int BinarySearch(int* array, int low, int high, int key);

int BinarySearch_Range(int* array, int low, int high, int key);

int BinarySearch_Custom(void* objArr, int objSize, int keyFieldOffset, int keyFieldSize, int low, int high, void* key, CmpFunc cmpFunc);
#define BinarySearchByField_Custom(objArr, objName, keyFieldName, low, high, key, cmpFunc) BinarySearch_Custom((objArr), sizeof(objName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), (low), (high), (key), (cmpFunc));

int BinarySearch_Range_Custom(void* objArr, int objSize, int keyFieldOffset, int keyFieldSize, int low, int high, void* key, CmpFunc cmpFunc);
#define BinarySearchByField_Range_Custom(objArr, objName, keyFieldName, low, high, key, cmpFunc) BinarySearch_Range_Custom((objArr), sizeof(objName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), (low), (high), (key), (cmpFunc));


#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_SEARCH_H_