// Pedro Escoboza
// A01251531
// TCB1004.500
// 21/11/2020

#include <iostream>
#include <fstream>
#include <vector>

#include "Timer.hpp"
#include "HashMap.hpp"
#include "HashMapInternalChaining.hpp"

template <class K, class T>
using MapContent = std::vector<std::pair<K, T>>;

template <class K>
using Lookups = std::vector<K>;

const std::vector<size_t> PRIMES{
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

/**
* Calculates the size needed for the bucket count.
* Time: O(n)
* Space: O(1)
* 
* @param size Number of elements to store in the hash map
* @param table Vector of prime sizes for bucket count
* @return Minimum prime bucket count from the table given that matches the size
*/
unsigned getBucketCount(size_t size, const std::vector<size_t>& table = PRIMES ) {

	for (const auto& s : table) {
		if (s >= size) {
			return s;
		}
	}

	return table[table.size() - 1];
}




int main() {


	std::cout << "Tests done. Press enter to exit.";
	std::cin.get();
}
