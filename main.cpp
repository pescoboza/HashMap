#include <iostream>
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
template <class K, class T, class Hasher = std::hash<K>>
void chain(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t bucketCount, std::ostream& out = std::cout) {
	HashMapInternalChaining<K, T, Hasher> map{bucketCount};

	out << " ====== INSERTIONS =======\n";
	for (const auto& entry : mapContent) {
		auto res{ map.insert(entry.first, entry.second) };
		out << "try insert: (" << entry.first <<", " << entry.second <<  ") result: " << std::boolalpha << res.first << '\n';
	}
	
	out << "\n\n\n";
	out << " ====== LOOKUPS =======\n";

	for (const auto& key : lookups) {
		auto res{ map.find(key) };
		
		out << "looked up: " << key << " result: ";
		if (res == nullptr) {
			out << " NULL";
		}
		else {
			out << res->second;
		}
		out << '\n';
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
template <class K, class T, class Hasher = std::hash<K>>
void quadratic(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, size_t bucketCount, std::ostream& out  = std::cout) {

	HashMap<K, T, Hasher> hashMap{bucketCount};


	out << " ====== INSERTIONS =======\n";
		for (const auto& entry : mapContent) {
			auto res{ hashMap.insert(entry.first, entry.second) };
			out << "try insert: (" << entry.first << ", " << entry.second << ") result: " << std::boolalpha << res.first << '\n';
		}

	out << "\n\n\n";
	out << " ====== LOOKUPS =======\n";

		for (const auto& key : lookups) {
			auto res{ hashMap.find(key) };

			out << "looked up: " << key << " result: ";
			if (res == nullptr) {
				out << " NULL";
			}
			else {
				out << res->second;
			}
			out << '\n';
		}
}

/**
* Runs a test for quadratic and chaining collision resolution hash maps.
* Calls each test function and print the time
* 
* @param mapContent Vector of entry pair to enter the hash map
* @param lookups Vector of keys to search in the hash map
* @param out Output stream reference to log the results to 
*/
template <class K, class T, class Hasher = std::hash<K>>
void test_template(const MapContent<K, T>& mapContent, const Lookups<K>& lookups, std::ostream& out = std::cout) {
	size_t bucketCount{ getBucketCount(mapContent.size()) };

	
	out << "QUADRATIC - BEGIN\n" << std::endl;
	Timer timer;
	quadratic<K, T, Hasher>(mapContent, lookups, bucketCount, out);
	out << "QUADRATIC - END - Elapsed: " << timer.elapsed() << " s" << std::endl;
	out << '\n';
	out << "CHAINING - BEGIN\n" << std::endl;
	timer.reset();
	chain<K, T, Hasher>(mapContent, lookups, bucketCount, out);
	out << "CHAINING - END - Elapsed: " << timer.elapsed() << " s" << std::endl;

}

/**
* Basic test of functionality with integers as keys and values.
* 
* @param out Output stream reference to log the results to
*/
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

/**
* Test with complex data type: std::string
*
* @param out Output stream reference to log the results to
*/
void test2(std::ostream& out = std::cout) {
	std::cout << "IMPLEMENT TEST 2" << std::endl;
}







struct Book {
	std::string m_name;
	std::string m_author;

	friend std::ostream& operator << (std::ostream& out, const Book& b) {
		out << b.m_name;
		return out;
	}
};

struct BookKey {
	std::string m_key;
	BookKey(const Book& book) :  m_key{ book.m_name + " by " + book.m_author }{}
	friend bool operator==(const BookKey& l, const BookKey& r) {
		return l.m_key == r.m_key;
	}
	
	friend std::ostream& operator << (std::ostream& out, const BookKey& k) {
		out << k.m_key;
		return out;
	}
};

struct BookKeyHasher {
	static const std::hash<std::string> s_hasher;

	BookKeyHasher() {}
	size_t operator()(const BookKey& bk) const {
		return s_hasher(bk.m_key);
	}
};
const std::hash<std::string> BookKeyHasher::s_hasher{};


/**
* Test with custom abstract data types.
*
* @param out Output stream reference to log the results to
*/
void test3(std::ostream& out = std::cout) {

	std::vector<Book> books{
		{"Dune", "Frank Herbert"},
		{"Ender's Game", "Orson Scott Card"},
		{"The Hitchhicker's Guide to the Galaxy", "Douglas Adams"},
		{"1948", "George Orwell"},
		{"Fahrenheit 451", "Ray Bradbury"},
		{"Brave New World", "Aldous Huxley"},
		{"Foundation", "Isaac Asimov"},
		{"Childhood's End", "Arthur C.Clarke"},
		{"Ubik", "Phillip K. Dick"}
	};

	Lookups<BookKey> lookups{
		books[0],
		books[4],
		books[8]
	};

	MapContent<BookKey, Book> mapContent;
	for (const auto& book : books) {
		mapContent.emplace_back(BookKey{book}, book);
	}

	test_template<BookKey, Book, BookKeyHasher>(mapContent, lookups, out);
	

};


/**
* Volume test
*
* @param out Output stream reference to log the results to
*/
void test4(std::ostream& out = std::cout) {
	std::cout << "IMPLEMENT THIS!" << '\n';
}


int main() {
	std::ostream& out{ std::cout };
	
	out << "\n\n=========== TEST 1 ==================" << std::endl;
	test1(out);
	
	out << "\n\n=========== TEST 2 ==================" << std::endl;
	test2(out);

	out << "\n\n=========== TEST 3 ==================" << std::endl;
	test3(out);
	
	out << "\n\n=========== TEST 4 ==================" << std::endl;
	test4(out);

	out << "Press enter to exit.";
	std::cin.get();
}
