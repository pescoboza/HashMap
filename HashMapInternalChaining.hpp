#ifndef HASH_MAP_INTERNAL_CHAINING_HPP
#define HASH_MAP_INTERNAL_CHAINING_HPP

// Pedro Escoboza
// A01251531
// TCB1004.500
// 21/11/2020

#include <vector>
#include <list>
#include <memory>

/**
 * Implementation a hash table of constant size.
 *
 * @param T Type of the entry value
 * @param K Type of the entry key
 * @param Size Constant size of the hash table
 * @param Hash Struct with overloaded operator() as with hash function
*/
template <class K, class T, class Hasher = std::hash<K>>
class HashMapInternalChaining {
public:
	using Entry = std::pair<const K, T>;
	using Bucket = std::list<Entry>;
	using BucketUPtr = std::unique_ptr<Bucket>;

private:
	std::vector<BucketUPtr> m_table; // Associative table container for key value pairs
	Hasher m_hasher; // Hashing struct with overloaded operator()
	size_t m_bucketCount; // Number of buckets in the table
	size_t m_size; // Number of entries in the table

public:
	/**
	 * Default constructor for HastMap.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @return HashMapInternalChaining
	 */
	HashMapInternalChaining(size_t bucket_count) : m_table{}, m_hasher{ Hasher{} }, m_bucketCount{ bucket_count }, m_size{ 0U } {
		m_table.resize(m_bucketCount);
		m_table.shrink_to_fit();
	}

	/**
	* Copy constructor. Allows for nesting of hash maps as values themselves.
	* Time: O(n)
	* Space: O(n)
	* 
	*  @return HashMapInternalChaining
	*/
	HashMapInternalChaining(const HashMapInternalChaining& copy) : m_table{}, m_hasher{}, m_bucketCount{ copy.bucket_count() }, m_size{copy.size()}{
		m_table.resize(m_bucketCount);
		for (const auto& bucket : copy.m_table) {
			
			if (bucket == nullptr) {
				continue;
			}

			for (const auto& entry : *bucket) {
				insert(entry.first, entry.second);
			}
		}
	}


	/**
	 * Insert a new element in the hash table if no element already has the key.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key to insert
	 * @param  value Value to map to the key
	 * @return Pointer to the newly inserted pair OR the previously mapped element
	 */
	const std::pair<bool, Entry*> insert(const K& key, const T& value);


	/**
	 * Finds an element on the hash table.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key to insert
	 * @return Pointer to the found entry or nullptr if not found
	 */
	Entry* find(const K& key);


	/**
	* Erases an entry with a given key.
	* Time: O(1)
	* Space: O(1)
	*
	* @param  key Key of the entry to erase
	*/
	void erase(const K& key);


	/**
	* Returns the number of entries in the container.
	* Time: O(1)
	* Space: O(1)
	*
	* @return Container size
	*/
	size_t size() const { return m_size; }


	/**
	 * Tells if the table is empty.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @return Wether the table has elements
	 */
	bool empty() const { return m_size == 0U; }


	 /**
	  * Clears the content of the hash map.
	  * Time: O(1)
	  * Space: O(1)
	  *
	  * @return void
	  */
	 void clear() {
		 m_table.clear();
		 m_table.resize(m_bucketCount);
	 }


	/*
	 * Gets the number of filled buckets in the container.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @return Number of filled buckets
	 */
	size_t bucket_count() const { return m_bucketCount; }

	/**
	* Helper to run a callbach on each element of the hash map
	* Time: O(n)
	* Space: O(1)
	* 
	* @param func Unary function that takes a const std::pair<const K, T>& as parameter
	*/
	template <class UnaryFunction>
	void forEach(UnaryFunction func) const {
		for (const auto& bucket : m_table) {
			if (bucket != nullptr) {
				for (const auto& entry : *bucket) {
					func(entry);
				}
			}
		}
	}

private:
	/**
	 * Generates a container index mapped to the key.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to hash
	 * @return Index of the table mapped to the key
	 */
	size_t hash(const K& key) const;

	/**
	* Private helper for finding a bucket node in the 
	* given bucket.
	* Time: O(n)
	* Space: O(1)
	* 
	* @param bucket Reference to linked list bucket of pair pointers
	* @return Unique pointer to entry pair or nullptr if not found.
	*/
	static typename Bucket::iterator findNodeInBucket(const K& key, Bucket& bucket);

	/**
	 * Finds the node element of the given key.
	 * Private helper for other functions
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to hash
	 * @param [out] bucketPos Bucket index in the table
	 * @return Pair with result and iteartor of node in bucket
	 */
	std::pair<bool, typename Bucket::iterator > findNode(const K& key, size_t& bucketPos);


	/**
	 * Helper to print the hash map.
	 * Time: O(n)
	 * Space: O(n)
	 *
	 * @param  [out] out Ostream to print out the hash map
	 * @param  hm Const reference to the hash map to print
	 * @return Ostream reference after the insertion
	 */
	friend std::ostream& operator<<(std::ostream& out, const HashMapInternalChaining& hm) {
		for (const auto& bucket : hm.m_table) {
			if (bucket == nullptr) {
				continue;
			}

			for (const auto& entry : *bucket) {
				out << entry.first << " : " << entry.second << '\n';
			}

		}
		return out;
	}

};

template<class K, class T, class Hasher>
inline const std::pair<bool, typename HashMapInternalChaining<K, T, Hasher>::Entry*> HashMapInternalChaining<K, T, Hasher>::insert(const K& key, const T& value) {
	// Get the bucket at the given key position
	BucketUPtr& bucketSlot{ m_table[hash(key)] };
	
	// If no bucket is found, create it and isert the element
	if (bucketSlot == nullptr) {
		bucketSlot = std::make_unique<Bucket>();
		bucketSlot->emplace_back(key, value);
		m_size++;
		return {true, &bucketSlot->back()};
	}
	
	// The node was full, look for the entry node in the bucket
	auto nodeIt{findNodeInBucket(key, *bucketSlot)};

	// Check the result of the lookup
	if (nodeIt != bucketSlot->end()) {
		// The key was occupied
		return {false, &*nodeIt};
	}
	else {
		// The bucket did not container the key, emplace it
		bucketSlot->emplace_back(key, value);
		m_size++;
		return { true, &bucketSlot->back() };
	}

}

template<class K, class T, class Hasher>
inline typename HashMapInternalChaining<K, T, Hasher>::Entry* HashMapInternalChaining<K, T, Hasher>::find(const K& key) {
	// Look for the node
	size_t i{ 0U };
	auto res{ findNode(key, i) };

	// If the bucket does not exist, or the bucket does not contain the key
	if (!res.first) {
		// Return nothing
		return nullptr;
	}

	// The result iterator is valid, return its content
	return &*res.second;
}

template<class K, class T, class Hasher>
inline void HashMapInternalChaining<K, T, Hasher>::erase(const K& key) {
	// Look for the node
	size_t bucketPos;
	auto res{ findNode(key, bucketPos) };

	// Check that the bucket is valid and that the node exists in the bucket
	if (res.first) {
		// Erase the node from the bucket
		m_size--;
		m_table[bucketPos]->erase(res.second);
	}
}

template<class K, class T, class Hasher>
inline size_t HashMapInternalChaining<K, T, Hasher>::hash(const K& key) const {
	return m_hasher(key) % m_bucketCount;
}

template<class K, class T, class Hasher>
inline typename HashMapInternalChaining<K, T, Hasher>::Bucket::iterator HashMapInternalChaining<K, T, Hasher>::findNodeInBucket(const K& key, Bucket& bucket){
	// Find the node in the linked list
	return std::find_if(bucket.begin(), bucket.end(),
		[&key](const Entry& entry) {
			return (entry.first == key);
		}
	);
}

template<class K, class T, class Hasher>
inline std::pair<bool , typename HashMapInternalChaining<K, T, Hasher>::Bucket::iterator> HashMapInternalChaining<K, T, Hasher>::findNode(const K& key, size_t& bucketPos) {
	// Look for the node in the bucket list of the index mapped to the key
	size_t i{ hash(key) };
	bucketPos = i;
	BucketUPtr& bucketPtr{ m_table[i] };
	
	
	if (bucketPtr != nullptr) {
	
		// The bucket is valid
		auto it{ findNodeInBucket(key, *bucketPtr) };
		
		if (it != bucketPtr->end()) {
			// The bucket contains the key
			return { true, it };
		}
		else {
			// The bucket does NOT contain the key
			return { false, it };
		}

	}

	// The bucket was empty
	return { false, typename Bucket::iterator{} };
}

#endif // !HASH_MAP_INTERNAL_CHAINING_HPP