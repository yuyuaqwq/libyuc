/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BITMAP_H_
#define CUTILS_CONTAINER_BITMAP_H_

#include <CUtils/object.h>
#include <CUtils/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif


	/*
	* 位图
	*/
#define CUTILS_CONTAINER_BIT_MAP_DECLARATION(bit_map_type_name, id_type) \
	typedef _##bit_map_type_name##Bitmap { \
		id_type count; \
		uint8_t byte_arr[1]; \
	} bit_map_type_name##Bitmap; \

#define CUTILS_CONTAINER_BIT_MAP_DEFINE(bit_map_type_name) \
    /*
	* 必须是8的倍数
	*/ \
	void bit_map_type_name##BitmapInit(bit_map_type_name##Bitmap* bitmap, size_t bit_count) { \
		size_t byte_count = bit_count / 8 + (bit_count % 8 ? 1 : 0); \
		for (size_t i = 0; i < byte_count; i++) { \
			bitmap->byte_arr[i] = 0; \
		} \
	} \
	bool bit_map_type_name##BitmapGet(bit_map_type_name##Bitmap* bitmap, size_t bit_idx) { \
		uint8_t offset = bit_idx % 8; \
		return bitmap->byte_arr[bit_idx / 8] & ((uint8_t)0x80 >> offset); \
	} \
	void bit_map_type_name##BitmapSet(bit_map_type_name##Bitmap* bitmap, size_t bit_idx, bool value) { \
		uint8_t offset = bit_idx % 8; \
		if (value) { \
			bitmap->byte_arr[bit_idx / 8] |= (uint8_t)0x80 >> pos; \
		} else { \
			bitmap->byte_arr[bit_idx / 8] &= ~((uint8_t)0x80 >> pos); \
		} \
	} \
	void bit_map_type_name##BitmapSetMultiple(bit_map_type_name##Bitmap* bitmap, size_t bit_idx, size_t bit_count, bool val) { \
		size_t byte_idx = bit_idx / 8; \
		bit_idx += (bit_count / 8) * 8; \
		while (bit_count > 8 && byte_idx < bitmap->count) { \
			BitmapSetByte(bitmap, byte_idx, val ? 0xff : 0); \
			bit_count--; \
			byte_idx++; \
		} \
		while (bit_count > 0 && bit_idx < BitmapGetBitCount(bitmap)) {
			BitmapSet(bitmap, bit_idx, val);
			bit_count--;
			bit_idx++;
		}
	}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BITMAP_H_