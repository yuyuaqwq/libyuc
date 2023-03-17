/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/bitmap.h>

CUTILS_CONTAINER_VECTOR_DEFINE(uint8_t, Byte, CUTILS_OBJECT_ALLOCATOR_DEFALUT)

const ptrdiff_t kBitmapInvalidIndex = -1;

static size_t BitmapGetBitIndex(ptrdiff_t byte_idx, ptrdiff_t bit_off) {
	return byte_idx * 8 + bit_off;
}

static size_t BitmapGetByteIndex(ptrdiff_t bit_idx, ptrdiff_t* bit_off) {
	if (bit_off) {
		*bit_off = bit_idx % 8;
	}
	return bit_idx / 8;
}

static uint8_t BitmapGetByte(Bitmap* bitmap, ptrdiff_t byte_idx) {
	return bitmap->arr.obj_arr[byte_idx];
}

static void BitmapSetByte(Bitmap* bitmap, ptrdiff_t byte_idx, uint8_t val) {
	bitmap->arr.obj_arr[byte_idx] = val;
}

static void BitmapSetMultiple(Bitmap* bitmap, ptrdiff_t bit_idx, size_t bit_count, bool val) {
	ptrdiff_t byte_idx = BitmapGetByteIndex(bit_idx, NULL);
	bit_idx += (bit_count / 8) * 8;
	while (bit_count > 8 && byte_idx < BitmapGetByteCount(bitmap)) {
		BitmapSetByte(bitmap, byte_idx, val ? 0xff : 0);
		bit_count--;
		byte_idx++;
	}
	while (bit_count > 0 && bit_idx < BitmapGetBitCount(bitmap)) {
		BitmapSet(bitmap, bit_idx, val);
		bit_count--;
		bit_idx++;
	}
}







size_t BitmapGetBitCount(Bitmap* bitmap) {
	return bitmap->arr.count * 8;
}

size_t BitmapGetByteCount(Bitmap* bitmap) {
	return bitmap->arr.count;
}


ptrdiff_t BitmapFindBit(Bitmap* bitmap, ptrdiff_t bit_start, bool val) {
	size_t bit_end = bit_start % 8;
	if (bit_end > 0) {
		bit_end = bit_start - bit_end + 8;
	} else {
		bit_end = bit_start;
	}
	ptrdiff_t byte_start = bit_start / 8;
	if (bit_end > bit_start) {
		byte_start++;
	}
	// 先单独比较前面的位
	while (bit_start < bit_end) {
		if (BitmapGet(bitmap, bit_start) == val) {
			return bit_start;
		}
		bit_start++;
	}
	
	size_t byte_end = BitmapGetByteCount(bitmap);
	// 再进行字节比较
	while (byte_start < byte_end) {
		if (BitmapGetByte(bitmap, byte_start) != (val ? 0 : 0xff)) {
			break;
		}
		byte_start++;
	}
	if (byte_start == byte_end) {
		return -1;
	}
	ptrdiff_t bit_off = 0;
	uint8_t byte = BitmapGetByte(bitmap, byte_start);
	while (((uint8_t)(0x80 >> bit_off) & byte) == (val ? 0 : (0x80 >> bit_off))) {		// 不断移位有效位，并比较，直到找到第一个有/无效位(从左往右)
		bit_off++;
	}
	bit_start = BitmapGetBitIndex(byte_start, bit_off);
	return bit_start;
}

ptrdiff_t BitmapAlloc(Bitmap* bitmap, size_t bit_count) {
	ptrdiff_t bit_start = 0;
	bit_start = BitmapFindBit(bitmap, bit_start, false);
	if (bit_start == -1) {
		return -1;
	}
	if (bit_count == 1) {
		BitmapSet(bitmap, bit_start, true);
		return bit_start;
	}
	size_t bit_end = BitmapGetBitCount(bitmap) - bit_start;		// 剩余可检查位数
	ptrdiff_t bit_cur = bit_start;
	size_t free_count = 1;
	// 简化实现的算法，逐位比较，性能较低
	while (bit_end-- > 0) {
		if (BitmapGet(bitmap, ++bit_cur) == false) {
			free_count++;
		} else {
			free_count = 0;		// 被有效位阻断，重新开始记录
		}
		if (bit_count == free_count) {
			bit_start = bit_cur - bit_count + 1;
			BitmapSetMult(bitmap, bit_start, bit_count, true);
			return bit_start;
		}
	}
	return -1;
}

void BitmapFree(Bitmap* bitmap, ptrdiff_t bit_idx, size_t bit_count) {
	BitmapSetMult(bitmap, bit_idx, bit_count, false);
}

size_t BitmapGetMaxFreeCount(Bitmap* bitmap) {
	ptrdiff_t bit_start = 0;
	bit_start = BitmapFindBit(bitmap, bit_start, false);
	if (bit_start == -1) {
		return 0;
	}
	size_t bit_end = BitmapGetBitCount(bitmap) - bit_start;		// 剩余可检查位数
	ptrdiff_t bit_cur = bit_start;
	size_t free_count = 1;
	size_t max_free_count = 1;
	// 简化实现的算法，逐位比较，性能较低
	while (bit_end-- > 0) {
		if (BitmapGet(bitmap, ++bit_cur) == false) {
			free_count++;
		}
		else {
			if (free_count > max_free_count) {
				max_free_count = free_count;
			}
			free_count = 0;		// 被有效位阻断，重新开始记录
		}
	}
	if (free_count > max_free_count) {
		max_free_count = free_count;
	}
	return max_free_count;
}