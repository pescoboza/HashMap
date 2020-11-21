#include <string>
#include "HashMap.hpp"

int main() {
	HashMap<int, int, 100U> hashMap;
	auto result{ hashMap.insert(3, 15) };

	return 0;
}