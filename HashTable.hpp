#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

// https://github.com/aozturk/HashMap/blob/master/hashmap/HashMap.h
// https://github.com/aozturk/HashMap/blob/master/hashmap/HashNode.h
// https://github.com/aozturk/HashMap/blob/master/hashmap/KeyHash.h


#include <array>

/**
 * Implementation a hash table of constant size.
 *
 * @param T Type of the entry value
 * @param K Type of the entry key
 * @param Size Constant size of the hash table
 * @param Hash Struct with overloaded operator() as with hash function
*/
template <class T, class K, size_t Size, class Hash = std::hash<K>>
class HashMap {
	using Entry = std::pair<const K, T>;
	
	std::array<Entry, Size> m_table;

public:
	HashMap() : m_table{ std::array<Entry, Size>{} } {}

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
	 	
};
#endif // !HASH_MAP_HPP