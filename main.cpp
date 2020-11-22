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
void chain(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t mapSize, std::ostream& out = std::cout) {
	HashMapInternalChaining<K, T> map{mapSize};

	out << " ====== INSERTIONS =======\n"
	for (const auto& entry : mapContent) {
		auto res{ map.insert(entry.first, entry.second) };
		out << "try insert: (" << entry.first <<", " << entry.second <<  ") result: " << std::boolalpha << res.first;
	}
	
	out << "\n\n\n";
	out << " ====== LOOKUPS =======\n"

	for (const auto& key : lookups) {
		auto res{ map.find(key) };
		
		out << "looked up: " << key << " result: ";
		if (res == nullptr) {
			out << " NULL"
		}
		else {
			out << res->second;
		}

	}
	
}

template <class T, class K>
void quadratic(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t mapSize, std::ostream& out  = std::cout) {

	HashMap<int, int> hashMap{PRIMES[4]};


	out << " ====== INSERTIONS =======\n"
		for (const auto& entry : mapContent) {
			auto res{ map.insert(entry.first, entry.second) };
			out << "try insert: (" << entry.first << ", " << entry.second << ") result: " << std::boolalpha << res.first;
		}

	out << "\n\n\n";
	out << " ====== LOOKUPS =======\n"

		for (const auto& key : lookups) {
			auto res{ map.find(key) };

			out << "looked up: " << key << " result: ";
			if (res == nullptr) {
				out << " NULL"
			}
			else {
				out << res->second;
			}

		}
}

template <class T, class K>
void test_template(const MapContent<K, T>& mapContent, const Lookups<K>& lookups) {
	size_t mapSize{ getSize(mapContent) };

	chain(mapContent, lookups, mapSize);
	quadratic(mapContent, lookups, mapSize);
}


// TODO: Iplement the tests
// TODO: Change linear to quadratic probing
void test1();
void test2();
void test3();
void test4();


int main() {
	std::cout << "\n\n=========== TEST 1 ==================" << std::endl;
	test1();
	
	std::cout << "\n\n=========== TEST 2 ==================" << std::endl;
	test2();

	std::cout << "\n\n=========== TEST 3 ==================" << std::endl;
	test3();
	
	std::cout << "\n\n=========== TEST 4 ==================" << std::endl;
	test4();

	std::cout << "Press enter to exit.";
	std::cin.get();
}
