/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/algorithm/binary_search.h>

/*
* 找到了返回正确下标，否则返回-1
*/
int BinarySearch(int* array, int first, int last, int key) {
    int mid;
    while (first <= last) {
        mid = first + (last - first) / 2;
        if (array[mid] > key) {
            last = mid - 1;
        }
        else if (array[mid] < key) {
            first = mid + 1;
        }
        else {
            return mid;
        }
    }
    return -1;
}

/*
* 找到了返回正确下标，否则返回第一个小于或大于key的元素下标，(通常返回第一个大于key的元素下标，若所有元素都小于key，则返回最后一个元素的下标)
*/
int BinarySearch_Range(int* array, int first, int last, int key) {
    int mid = 0;
    while (first < last) {
        mid = first + (last - first) / 2;
        if (array[mid] < key) first = mid + 1;
        else last = mid;
    }
    return first;
}