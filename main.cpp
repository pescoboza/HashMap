#include <iostream>
#include <vector>


#include "HashMap.hpp"
#include "HashMapInternalChaining.hpp"

template <class T, class K>
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


/**
* Runs a test with the hashmap implementation by internal chaining.
* Time: O(n)
* Space: O(n)
*
* @param mapContent Vector of pairs of entries of the hash map
* @param lookups Vector of keys to lookup in the hash map
* @param bucketCount Number of buckets in the hash map
* @param out Output stream reference to log the results to
*/
template <class T, class K>
void chain(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t bucketCount, std::ostream& out = std::cout) {
	HashMapInternalChaining<K, T> map{bucketCount};

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

/**
* Runs a test with the hashmap implementation by quadratic open addressing.
* Time: O(n)
* Space: O(n)
* 
* @param mapContent Vector of pairs of entries of the hash map
* @param lookups Vector of keys to lookup in the hash map
* @param bucketCount Number of buckets in the hash map
* @param out Output stream reference to log the results to
*/
template <class T, class K>
void quadratic(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t bucketCount, std::ostream& out  = std::cout) {

	HashMap<int, int> hashMap{bucketCount};


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

/**
* Runs a test for quadratic and chaining collision resolution hash maps.
* 
* @param mapContent Vector of entry pair to enter the hash map
* @param lookups Vector of keys to search in the hash map
* @param out Output stream reference to log the results to 
*/
template <class T, class K>
void test_template(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, std::ostream& out = std::cout) {
	size_t bucketCount{ getBucketCount(mapContent.size()) };

	chain(mapContent, lookups, bucketCount, out);
	quadratic(mapContent, lookups, bucketCount, out);
}


void test1(std::ostream& out = std::cout) {
	MapContent<int, int> data{
		{1, 4},
		{2, 8},
		{3, 12},
		{4, 16},
		{5, 20},
		{6, 24},
		{7, 28},
		{8, 32},
		{9, 36},
		{10,40},
		{11,44},
		{12,48},
		{13,52},
		{14,56},
		{15,60},
		{16,64}
	};

	Lookups <int> lookups{
		1,3,6,16,14,8,9,20
	};

	test_template(data, lookups, out);
}
void test2(std::ostream& out = std::cout);
void test3(std::ostream& out = std::cout);
void test4(std::ostream& out = std::cout);


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
