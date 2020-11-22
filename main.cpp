#include <iostream>
#include <vector>


#include "HashMap.hpp"
#include "HashMapInternalChaining.hpp"


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

template<class Container>
unsigned getSize(const Container& cont, size_t*, const std::vector<size_t>& table = PRIMES ) {
	size_t actualSize{ cont.size() };

	for (const auto& s : table) {
		if (s >= actualSize) {
			return s;
		}
	}

	return table[table.size() - 1];
}

template <class T, class K>
using MapContent = std::vector<std::pair<K, T>>;

template <class K>
using Lookups = std::vector<K>;


template <class T, class K>
void test_template(const MapContent<K ,T>& mapContent, const Lookups<K>& lookups) {
	size_t mapSize{getSize(mapContent)};
	
	chain(mapContent, lookups, mapSize);
	quadratic(mapContent, lookups, mapSize);
}



template <class T, class K>
void chain(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t mapSize, std::ostream& = std::cout) {
	HashMapInternalChaining<K, T> map{mapSize};

	for (const auto& entry : mapContent) {
		auto res{ map.insert(entry.first, entry.second) };
		out << "ty insert: (" << entry.first <<", " << entry.second <<  ") result: " << std::boolalpha << res.first;
	}
	
}

void quadratic() {

	HashMap<int, int> hashMap{PRIMES[4]};

	int k{ 3 };

	hashMap.insert(k, 15);
	hashMap.insert(k+1, 15);

	auto findRes1{ hashMap.find(k) };
	auto findRes2{hashMap.find(k+1)};

	hashMap.erase(k);

}

int main() {
	
	chain();
	
	return 0;
}
