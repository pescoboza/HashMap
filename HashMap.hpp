#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP


#include <array>
#include <memory>

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
	using EntryUPtr = std::unique_ptr<Entry>;
	
	std::array<EntryUPtr, Size> m_table;
	Hash m_hash;

public:
	/**
	 * Default constructor for HastMap.
	 * Time: O(1)
	 * Space: O(1)
	 * 
	 * @return HashMap
	 */
	HashMap() : m_table{ std::array<EntryUPtr, Size>{} }, m_hash{ Hash{} }{}

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

private:
	/**
	 * Generates a container index mapped to the key.
	 * Time: O(1)
	 * Space: O(1)
	 *
	 * @param  key Key of the entry to hash
	 * @return Index of the table mapped to the key
	 */
	size_t hash(const K& key);
	 	
};

template<class T, class K, size_t Size, class Hash>
inline const std::pair<bool, typename HashMap<T, K, Size, Hash>::Entry*> HashMap<T, K, Size, Hash>::insert(const K& key, const T& value){
	size_t i{ hash(key) };

	bool wasInserted{ false };
	if (m_table[i] == nullptr) {
		m_table[i] = std::make_unique<Entry>(key, value);
		wasInserted = true;
	}

	return { wasInserted, m_table[i].get() };
}

template<class T, class K, size_t Size, class Hash>
inline size_t HashMap<T, K, Size, Hash>::hash(const K& key){
	return m_hash(key) % Size;
}

#endif // !HASH_MAP_HPP


