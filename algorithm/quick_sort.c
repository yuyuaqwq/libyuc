/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

/*
* 快速排序
* left填0，right填数组成员数-1
*/
void QuickSort(int* arr, int left, int right) {
	// 从数组中选取一个基准值(一般是左右两端的其中一端或者是中间)，将小于基准值的所有成员放到基准值的左边，大于基准值的所有成员放到基准值的右边，再分别对这两边的数组重复上述步骤，直到排序完成。
	// 希望快速排序是原地排序算法的话，这里写两种方法我所知道的方法：
	// 1.使用双指针，起始i和j都指向下标0，取最后一个元素则作为基准值，比较j指针和基准值的大小，不需要交换时，i不动，j步进1，需要交换时，交换i和j指向的数据，i和j步进1，这样子交换后，i前面肯定是比基准值大或者小的数，一直到j跑完后，交换基准值和i指向的数据，此时基准值前面和后面都是比基准值大或者小的数值了。
	// 2.使用双指针，i指向下标0，j指向下标末端，最开始j的指向是基准值，比较i的指向和j的指向，如果不需要交换则i往中间靠一步，继续用i比较，否则交换i和j指向的数据，此时i的指向就是基准值了，用j往中间靠，比较，重复这个过程，直到两指针重合。
	// 不稳定排序算法，交换后等值的元素前后位置关系就可能发生改变。
	// 时间复杂度O(nlogn)，空间复杂度O(1)
	// 快排还有一个特性就是时间复杂度取决于基准值的选择，最坏情况的O(n^2)，也就是每次选择的基准值都无法将数组分成两份，而是其中一边没有任何成员，但平均复杂度仍是O(nlogn)
	if (left >= right) return;
	
	int key = arr[right];
	int nkey;
	
	int i = left; //指向最后会与key交换的成员，
	int j = left;
	while (j < right) {
		//原地分区
		if (arr[j] < key) {
			if (i != j) {
				nkey = arr[j];
				arr[j] = arr[i];
				arr[i] = nkey;
			}
			++i;
		}
		++j;
	}
	arr[right] = arr[i];
	arr[i] = key;
	
	QuickSort(arr, left, i - 1);
	QuickSort(arr, i + 1, right);
}