/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_POOL_H_
#define CUTILS_CONTAINER_POOL_H_

#include <CUtils/object.h>
#include <CUtils/container/static_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 对象池
*/

#define CUTILS_CONTAINER_POOL_DECLARATION(pool_name, obj_type) \
	typedef struct _##pool_name##Pool { \
		 \
	}; \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_POOL_H_