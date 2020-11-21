#include <string>
#include "HashMap.hpp"

int main() {
	HashMap<int, int, 3U> hashMap;

	int k{ 3 };

	hashMap.insert(k, 15);
	hashMap.insert(k+1, 15);

	auto findRes1{ hashMap.find(k) };
	auto findRes2{hashMap.find(k+1)};

	return 0;
}