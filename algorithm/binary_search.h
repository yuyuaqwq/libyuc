/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_ALGORITHM_SEARCH_H_
#define CUTILS_ALGORITHM_SEARCH_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

int BinarySearch(int* array, int low, int high, int key);
int BinarySearch_Range(int* array, int low, int high, int key);

int BinarySearch_Object(void* objArr, int keyFieldSize, int low, int high, void* key, int objSize, int keyFieldOffset, CmpFunc cmpFunc);
#define BinarySearchByField_Object(objArr, objName, keyFieldName, low, high, key, cmpFunc) BinarySearch_Object((objArr), sizeof(objName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), (low), (high), (key), (cmpFunc));
int BinarySearch_Object_Range(void* objArr, int objSize, int low, int high, void* key, int keyFieldOffset, int keyFieldSize, CmpFunc cmpFunc);
#define BinarySearchByField_Object_Range(objArr, objName, keyFieldName, low, high, key, cmpFunc) BinarySearch_Object_Range((objArr), sizeof(objName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), (low), (high), (key), (cmpFunc));

int BinarySearch_KeyAtCallback(void* objArr, ArrAtFunc keyAt, int first, int last, void* key, int keySize, CmpFunc cmpFunc);
int BinarySearch_KeyAtCallback_Range(void* objArr, ArrAtFunc keyAt, int first, int last, void* key, int keySize, CmpFunc cmpFunc);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_ALGORITHM_SEARCH_H_