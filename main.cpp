#include <string>
#include "HashMap.hpp"

#include <unordered_map>
#include <iostream>

int main() {

	std::unordered_map<int, double> map;
	
	int numInserts{ 10000000 };
	int bucketCount{ -1 };
	for (int i{ 0 }; i < numInserts; ++i) {
		map.emplace(i, i + i * 0.1);
		if (map.bucket_count() != bucketCount) {
			bucketCount = map.bucket_count();
			std::cout << "i: " << i <<" bc: " <<bucketCount - 1 << std::endl;
		}
	}

	std::cout << map.max_bucket_count() << std::endl;
	std::cout << map.bucket_count() << std::endl;
	std::cout << map.load_factor() << std::endl;

	HashMap<int, int, 3U> hashMap;

	int k{ 3 };

	hashMap.insert(k, 15);
	hashMap.insert(k+1, 15);

	auto findRes1{ hashMap.find(k) };
	auto findRes2{hashMap.find(k+1)};

	hashMap.erase(k);

	return 0;
}