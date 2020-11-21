#ifndef HASH_MAP_INTERNAL_CHAINING_HPP
#define HASH_MAP_INTERNAL_CHAINING_HPP

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
template <class T, class K, class Hasher = std::hash<K>>
class HashMapInternalChaining {
	using Entry = std::pair<const K, T>;
	using EntryUPtr = std::unique_ptr<Entry>;
	using Bucket = std::list<EntryUPtr>;

	std::vector<Bucket> m_table; // Associative table container for key value pairs
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
	static EntryUPtr* findNodeInBucket(const K& key, Bucket& bucket);

	/**
	 * Finds the node element of the given key.
	 * Protected helper for other functions
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to hash
	 * @param [out] Node found found at the position
	 * @return Index of the table mapped to the key
	 */
	size_t findNode(const K& key, EntryUPtr*& node);

};

template<class T, class K, class Hasher>
inline const std::pair<bool, typename HashMapInternalChaining<T, K, Hasher>::Entry*> HashMapInternalChaining<T, K, Hasher>::insert(const K& key, const T& value) {
	EntryUPtr* res{ nullptr };
	size_t i{ findNode(key, res) };

	// Check if the given position is 
	if (res != nullptr && *res != nullptr) {
		// The key is occupied, return the element
		return { false, m_table[i].get() };

	}
	else {
		// Position was empty, insert
		m_table[i].emplace_back(std::make_unique<Entry>(key, value));
		m_size++;
		return { true, m_table[i].get() };
	}
}

template<class T, class K, class Hasher>
inline typename HashMapInternalChaining<T, K, Hasher>::Entry* HashMapInternalChaining<T, K, Hasher>::find(const K& key) {
	EntryUPtr* res{ nullptr };
	findNode(key, res);
	return ((res != nullptr && *res != nullptr) ? res->get() : nullptr);
}

template<class T, class K, class Hasher>
inline void HashMapInternalChaining<T, K, Hasher>::erase(const K& key) {
	// Find the node
	EntryUPtr* res{ nullptr };
	findNode(key, res);

	// If it is found, delete it
	if (res != nullptr && *res != nullptr) {
		m_size--;
		res->reset();
	}
}

template<class T, class K, class Hasher>
inline size_t HashMapInternalChaining<T, K, Hasher>::hash(const K& key) const {
	return m_hasher(key) % (m_bucketCount - 1);
}

template<class T, class K, class Hasher>
inline typename HashMapInternalChaining<T, K, Hasher>::EntryUPtr* HashMapInternalChaining<T, K, Hasher>::findNodeInBucket(const K& key, Bucket& bucket){

	// Find the node in the linked list
	auto it{ std::find_if(bucket.begin(), bucket.end(),
		[&key](const EntryUPtr& entryNode) {
			return (entryNode != nullptr &&
				   entryNode->first == key);
		})
	};

	return (it != bucket.end() ? &(*it) : nullptr);
}

template<class T, class K, class Hasher>
inline size_t HashMapInternalChaining<T, K, Hasher>::findNode(const K& key, EntryUPtr*& node) {

	size_t startPos{ hash(key) };
	if (m_table[startPos] == nullptr) {
		// Not found
		node = nullptr;
		return startPos;
	}

	// Something found, compare the key to check for match
	if (m_table[startPos]->first == key) {
		// The X marks the spot!
		node = &m_table[startPos];
		return startPos;
	}

	// We got a collision, iterate until you get the key or null
	// Get the starting position and wrap around the array looking for the key
	for (size_t i{ 0U }; i < m_bucketCount; ++i) {
		size_t wrapPos{ (startPos + i) % m_bucketCount };

		if (m_table[wrapPos] == nullptr) {
			// Not found
			node = nullptr;
			return wrapPos;
		}

		if (m_table[wrapPos]->first == key) {
			// The mark was off, still got the treasure
			node = &m_table[wrapPos];
			return wrapPos;
		}
	}

	// Worst case: full iteration
	node = nullptr;
	return -1;
}

#endif // !HASH_MAP_INTERNAL_CHAINING_HPP



