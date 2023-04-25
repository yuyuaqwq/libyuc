

/* complete binary tree - 完全二叉树 */


#define ZERO_GET_PARENT(index) (((index) + 1) / 2 - 1)
#define ZERO_GET_LEFT(index) (((index) * 2 + 1)
#define ZERO_GET_RIGHT(index) (((index) * 2 + 2)

#define ONE_GET_PARENT(index) ((index) / 2)
#define ONE_GET_LEFT(index) (((index) * 2)
#define ONE_GET_RIGHT(index) (((index) * 2 + 1)