/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include "bitmap.h"

void BitmapInit(Bitmap* bitmap, size_t bitCount) {
	int byteCount = bitCount / 8 + (bitCount % 8 ? 1 : 0);
	ArrayInit(&bitmap->arr, byteCount, sizeof(uint8_t));
	for (int i = 0; i < byteCount; i++) {
		*ArrayAt(&bitmap->arr, i, uint8_t) = 0;
	}
}

bool BitmapGet(Bitmap* bitmap, int bitPos) {
	uint8_t entry = ArrayAt(&bitmap->arr, bitPos / 8, uint8_t);
	uint8_t pos = bitPos % 8;
	return entry & ((uint8_t)1 << pos);
}

void BitmapSet(Bitmap* bitmap, int bitPos, bool value) {
	uint8_t* entry = ArrayAt(&bitmap->arr, bitPos / 8, uint8_t);
	uint8_t pos = bitPos % 8;
	if (value) {
		*entry |= (uint8_t)1 << pos;
	} else {
		*entry &= ~((uint8_t)1 << pos);
	}
}