/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BITMAP_H_
#define CUTILS_CONTAINER_BITMAP_H_

#include <CUtils/object.h>
#include <CUtils/container/array.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Bitmap {
	Array arr;
} Bitmap;

void BitmapInit(Bitmap* bitmap, size_t bit_count);
bool BitmapGet(Bitmap* bitmap, ptrdiff_t bit_idx);
void BitmapSet(Bitmap* bitmap, ptrdiff_t bitPos, bool value);
size_t BitmapGetBitCount(Bitmap* bitmap);
size_t BitmapGetByteCount(Bitmap* bitmap);
ptrdiff_t BitmapFindBit(Bitmap* bitmap, ptrdiff_t bit_start, bool val);
ptrdiff_t BitmapAlloc(Bitmap* bitmap, size_t bit_count);
void BitmapFree(Bitmap* bitmap, ptrdiff_t bit_idx, size_t bit_count);

extern const ptrdiff_t kBitmapInvalidIndex;

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BITMAP_H_