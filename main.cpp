#include <iostream>

#include "HashMap.hpp"


const size_t PRIMES[]{
	 7U,
	 63U,
	 511U,
	 1023U,
	 2047U,
	 4095U,
	 8191U,
	 16383U,
	 32767U,
	 65535U,
	 131071U,
	 262143U,
	 524287U,
	 1048575U,
	 2097151U,
	 4194303U,
	 8388607U,
	 16777215U
};

int main() {

	HashMap<int, int> hashMap{PRIMES[4]};

	int k{ 3 };

	hashMap.insert(k, 15);
	hashMap.insert(k+1, 15);

	auto findRes1{ hashMap.find(k) };
	auto findRes2{hashMap.find(k+1)};

	hashMap.erase(k);

	return 0;
}