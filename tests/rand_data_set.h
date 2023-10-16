#include <vector>
#include <string>
#include <functional>

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

static std::vector<int64_t> GenerateI64Vector(size_t count, bool dense = true) {
	std::vector<int64_t> res;
	res.reserve(count);
	for (size_t i = 0; i < count; i++) {
		int64_t data = i;
		//ReverseOrder(&data, sizeof(data));
		if (!dense) {
			data = ((int64_t)rand() << 48) + ((int64_t)rand() << 32) + ((int64_t)rand() << 16) + rand();
		}
		res.push_back(data);
	}

	for (int64_t i = 0; i < count; i++) {
		std::swap(res[i], res[RandInt() % count]);
	}

	return res;
}

static std::vector<std::string> GenerateStringVector(size_t count, size_t min_len, size_t max_len) {
	std::vector<std::string> res;
	res.reserve(count);
	for (size_t i = 0; i < count; i++) {
		size_t size = RandInt() % (max_len - min_len) + min_len;
		std::string data;
		data.resize(size);
		for (int j = 0; j < size; j++) {
			data[j] = rand() % 255 + 1 /* + ('a' - 1)*/; // rand() % 26 + 'a';
		}
		res.push_back(data);
	}

	return res;
}