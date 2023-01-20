/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BITMAP_H_
#define CUTILS_CONTAINER_BITMAP_H_

#include "CUtils/object.h"
#include "CUtils/container/array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Bitmap {
	Array arr;
} Bitmap;

void BitmapInit(Bitmap* bitmap, size_t bitCount);

bool BitmapGet(Bitmap* bitmap, offset_t bitPos);

void BitmapSet(Bitmap* bitmap, offset_t bitPos, bool value);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BITMAP_H_