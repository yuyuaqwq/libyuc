/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_ITER_H_
#define CUTILS_CONTAINER_ITER_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 迭代器抽象 */

// 迭代器初始化
// First - 指向第一个
// Last - 指向最后一个
// Find - 部分容器提供，直接定位到指定位置

// 迭代器状态
// Valid - 有效
// Invalid - 无效

// 迭代器迭代
// Next - 转到下一个
// Prev - 转到上一个
// Access - 直接定位，如array支持基于整数索引随机定位、map支持基于key定位

// ... 自定义方法，如树可能还会提供Up、Down等迭代方法

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LIST_H_