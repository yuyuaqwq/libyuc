#include <vector>

static int RandInt() {
	return (rand() << 16) | rand();
}

static void ReverseOrder(void* buf, size_t size) {
	uint8_t* buf_ = (uint8_t*)buf;
	for (size_t i = 0; i < size / 2; i++) {
		uint8_t temp = buf_[i];
		buf_[i] = buf_[size - 1 - i];
		buf_[size - 1 - i] = temp;
	}
}

static std::vector<int64_t> GenerateI64Vector(size_t count) {
	std::vector<int64_t> res;
	res.reserve(count);
	for (size_t i = 0; i < count; i++) {
		int64_t data = i;
		//ReverseOrder(&data, sizeof(data));
		// data = ((int64_t)rand() << 48) + ((int64_t)rand() << 32) + ((int64_t)rand() << 16) + rand();
		res.push_back(data);
	}

	for (int64_t i = 0; i < count; i++) {
		std::swap(res[i], res[RandInt() % count]);
	}

	return res;
}